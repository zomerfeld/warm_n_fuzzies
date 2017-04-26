// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_LSM303_U.h>
#include <Particle-GPS.h> 
#include <Adafruit_Sensor.h>
#include <Wire.h>


/* Assign a unique ID to the LSM sensor at */
Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);


void displaySensorDetails(void) // **** DISPLAYS SENSOR DETAILS ON STARTUP - VIA SERIAL ****
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


// **** CREATES THE FUNCTION VARIABLES - used to call these functions from the dashboard or other devices****

int heat2(String command); //function that turns Pin 2 on and off
int antStat(String command); //function that Returns GPS antenna status
int getGPS(String command); //function that Returns GPS data
int getComp(String command); //function that Returns Compass data
int target(String command); //function that gives the target location


//**** PORJECT VARIABLES ****
int heaters[]={A0,A1,A2,2,3,4,5,6,}; //list all of the heaters pins in an array. If you add or change a heater pin, add it to here. 
int onOff; //saves the command for the heaters as an integer
int tHeat = 1; //toggle for heating - if this is set to 0 the heaters will not go on. 
int coldPin = A5; //a pin that lights up an LED when all heating elements are off
unsigned long bootupTime;
unsigned long lastGPStime = 0;
String heater; // what heater is supposed to be 
String location; //current public location

float lat; //Latitude 
float lon; //longitute 
double latPublic; //Latitude casted for variable publishing 
double lonPublic; //Longitude casted for variable publishing 
String oldLoc; // Stores the previous target location
float targetLat; //The Latitude for the target location
float targetLon; //The Longitude for the target location

double compassPublic; //Compass heading
String directionality = "xx"; //The expression of cardinal direction (NE/SW) from GPS location to target 
double bearing; //the compass angle calculated between location and target 
// float magxOffset = 2.55; //offsets if needed
// float magyOffset = 27.95;
float magxOffset = 0;
float magyOffset = 0;

// **** SET GPS ***
// The RX an TX pins are connected to the TX and RX pins on the device (Serial1).
Gps _gps = Gps(&Serial1);

// **** CREATE TIMERS *** 
Timer _timer = Timer(1, onSerialData); // This timer will fire every 1 ms to receive data from the GPS (you can't change that #)
Timer _timerCompass = Timer(500, getCompData); // This timer will fire every 0.5 seconds to capture Compass data
Timer _timerWear = Timer(1000, wearable); //This timer will start the entire sequence.


void setup() {
    
    // **** CLOUD VARIABLES ***
    Particle.variable("Compass", compassPublic);
    Particle.variable("Latitude", latPublic);
    Particle.variable("Longitude", lonPublic);
    Particle.variable("Cardinal", directionality);
    Particle.variable("Bearing", bearing);
    Particle.variable("isHeatOn", tHeat);
    Particle.variable("heater", heater);
    Particle.variable("Location", location);


    // **** OTHER VARIABLES ****
    pinMode(coldPin, OUTPUT); // set the ColdPin LED to be output
  
    //turn off all heaters
      for (int i=0; i < sizeof(heaters); i++) {
        pinMode(heaters[i],OUTPUT);
        digitalWrite(heaters[i],LOW);
      }
  
    Particle.publish("All Heaters","OFF", 31536000, PRIVATE);

// **** DECLARES FUNCTIONS TO PARTICLE CLOUD 
    Particle.function("toggleHeat", toggleHeat); //declares the Heat Toggle function
    Particle.function("heat", heat); //declares the HEAT cloud function
    Particle.function("GetGPS", getGPS); //declares the GPS cloud function
    Particle.function("SetTarget", setTarget); //declares the Target Setting function
    Particle.function("AntennaStat", antStat); //declares the Antenna cloud function
    Particle.function("GetCompass", getComp); //declares the Compass cloud function
  

    delay(4000); // wait 4 seconds for you to start a Serial monitor if needed
  
    Serial.begin(); //starts USB serial, defaults to 9600 BPS
    Serial.println("Initializing...");

  // **** Initialize the GPS *****
    _gps.begin(9600); //this is for Serial for the GPS, wired to serial1 - TX and RX on the photon
  
  // **** START TIMERS **** 
    bootupTime = millis();
    _timer.start();
    _timerCompass.start();
    _timerWear.start();

  // *** INITIALIZE Compass ****.
  /* Enable auto-gain */
    mag.enableAutoRange(true);

  /* Initialise the sensor */
    if(!mag.begin())
   {
    /* There was a problem detecting the LSM303 ... check your connections */
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    Particle.publish("Compass: ","Ooops, no LSM303 detected ... Check your wiring!",31536000, PRIVATE); //publish the command
    while(1); // will freeze machine
   }
  /* Display some basic information on this compass */
   displaySensorDetails();
   setTarget("21.422474, 39.826192"); //starts with a default destination - Mecca
  
}


// *************** END SETUP ***************


// *************** BEGIN LOOP ***************


void loop() {

    unsigned long currentMillis = millis(); 
        if ((currentMillis - bootupTime >= 8000) && (currentMillis - lastGPStime >= 1000)) { //waits 8 seconds from bootup
            getGPS("1"); //Gets GPS
            lastGPStime = millis();
        }
}

// *************** END LOOP ***************


// *************** WEARABLE SEQUENCE ***************

void wearable() { //function to run every second and update the entire setup
    unsigned long currentMillis = millis();
    if (currentMillis - bootupTime >= 10000) { //waits 10 seconds from bootup
        getDirection(); // gets the cardinal directionality
        getBearing(); //gets the bearing in degrees
        heatBearing(); //turns on the relveant heater    
        }
}
// *************** WEARABLE SEQUENCE ***************


void onSerialData() { //gets the GPS DATA, runs ever 1 ms
  _gps.onSerialData();
}

// *************** COMPASS DATA ***************
// THERE ARE TWO FUNCTIONS FOR THIS, one triggered automatically (getCompData) which updates a cloud variable but doesn't publish an event, and another one (getComp) that does both

void getCompData() {  // *** DOESN'T PUBLISH 
  /* Get a new compass event */
  sensors_event_t event;
  mag.getEvent(&event);
  
  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  // to Change orientation (without accelerometer), change the axis below to the one *not* facing up. 

  float heading = (atan2(event.magnetic.y + magyOffset,event.magnetic.x + magxOffset) * 180) / M_PI;

// Normalize to 0-360 
  if (heading < 0)
  {
    heading = 360 + heading;
  }
  compassPublic = (double)heading;
  }

  // **** PARSE & PUBLISH COMPASS DATA ***
int getComp(String command) { // DOES PUBLISH
    //   Serial.println("Getting Compass"); //debug
    //   Particle.publish("Getting Compass",NULL,31536000, PRIVATE); // debug
  /* Get a new compass event */
  sensors_event_t event;
  mag.getEvent(&event);

  //*** MAGNETIC VECTOR ****
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  //Particle.publish("CompassXYZ: ", "X: " + String(event.magnetic.x) + " Y: " + String(event.magnetic.y) + " Z: " + String(event.magnetic.z), 60, PRIVATE); //publish the compass heading - for Debug

  // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  // to Change orientation (without accelerometer), changed the axis below to the one *not* facing up - X in my case. 
  //   float heading = atan2(event.magnetic.y, event.magnetic.x);
  float heading = (atan2(event.magnetic.y + magyOffset,event.magnetic.x + magxOffset) * 180) / M_PI;
  
  // Normalize to 0-360
  if (heading < 0) {
    heading = 360 + heading;
  }
  
  compassPublic = (double)heading;

  // *** PUBLISH HEADING - DEGREES ***
  Serial.print("Heading (degrees): "); Serial.println(compassPublic);
  Particle.publish("CompassHeading: ", String(compassPublic), 31536000, PRIVATE); //publishes the compass heading
  
  return 1;

}

// *************** HEATING FUNCTIONS ***************

// **** TOGGLE HEATING ****
int toggleHeat(String Command) {
    tHeat = Command.toInt(); //casts the string command to an int
    if (tHeat == 1) {
        Particle.publish("HeatToggle","ON",31536000, PRIVATE); 

    } else {
        heat("0");
        Particle.publish("HeatToggle","OFF",31536000, PRIVATE); 
    }
}

// **** TURNS ON HEAT PADS BY COMMAND ****
int heat(String command) { 
    
    // Particle.publish("command was ",command,31536000, PRIVATE); //publish what heater is supposed to turn on
    heater = command;
    
    if (tHeat != 1) { //don't heat anything if toggle is set to 0
        //turn off all heaters
         for (int i=0; i<sizeof(heaters); i++) {
             digitalWrite(heaters[i],LOW);
         }
        // Particle.publish("Heat is Toggled off",command,31536000, PRIVATE); //publish what heater is supposed to turn on
        return 0;
    } else if (command == "A0") { //skips verification because A0 isn't an INT
    //turn off all heaters
    for (int i=0; i<sizeof(heaters); i++) {
      digitalWrite(heaters[i],LOW);
    }
        digitalWrite(A0, HIGH);
        digitalWrite(coldPin, LOW);
        // Particle.publish("Heater on:",command, 31536000, PRIVATE); //DEBUG
    
    } else if (command == "A1") { //skips verifications because A1 isn't an INT
    //turn off all heaters
    for (int i=0; i<sizeof(heaters); i++) {
      digitalWrite(heaters[i],LOW);
    }
        digitalWrite(A1, HIGH);
        digitalWrite(coldPin, LOW);
        // Particle.publish("Heater on:",command, 31536000, PRIVATE); //DEBUG
    
    } else if (command == "A2") { //skips verifications because A2 isn't an INT
    //turn off all heaters
    for (int i=0; i<sizeof(heaters); i++) {
      digitalWrite(heaters[i],LOW);
    }
        digitalWrite(A2, HIGH);
        digitalWrite(coldPin, LOW);
        // Particle.publish("Heater on:",command, 31536000, PRIVATE); //DEBUG
    
    } else { 
      // *** This function is supposed to verify if the command is in the array. if it's not, turn on the off pin 
      onOff = command.toInt(); // casts the command to an INT
     
      if (onOff == 0) { // 0 works as a default off to all heaters
        //turn off all heaters
        for (int i=0; i<sizeof(heaters); i++) {
          digitalWrite(heaters[i],LOW);
        }
        digitalWrite(coldPin, HIGH);
      }
      
      int wasFound = searchArray(onOff); //looks in the array for the number given in the command
    //   Particle.publish("HeaterFoundInArray?", String(wasFound), 31536000, PRIVATE); // DEBUG
      
      if (wasFound == 1) { //if found in heaters array - this doesn't quite work accurately 
        //turn off all heaters
        for (int i=0; i<sizeof(heaters); i++) {
          digitalWrite(heaters[i],LOW);
        }
        
        if (onOff !=0) { // if onOff is an integer (heater number) and not zero
            digitalWrite(onOff, HIGH); //turns on the Specific heater
            // Particle.publish("Heater on:",command, 31536000, PRIVATE); // DEBUG - which heater is on
            digitalWrite(coldPin, LOW);
        } else { //in array but not int
            digitalWrite(coldPin, HIGH); //turn on the noheat pin
            Particle.publish("Invalid command - Not Int",command, 31536000, PRIVATE);
        }
      } else { // if not in array
        Particle.publish("Invalid command - Not in array",command, 31536000, PRIVATE);
      }
    }
    
      return 1; //ends the function
    }
    
// **** FUNCTION TO SEARCH ARRAY    
int searchArray (int cmd) { 
    int found = 0;
    for (int arrayI = 0; arrayI < sizeof(heaters); arrayI++) {
        if (heaters[arrayI] == cmd) {
          found = 1;
        //   Particle.publish("SearchArrayFunciton", "FOUND", 31536000, PRIVATE); //publishes from the search command. Use for debug
          } else {
        // do nothing
          }
        }
        return found;
    }



// *************** GPS FUNCTIONS ***************

// **** GETS AND PUBLISHES GPS ANTENNA STATUS  ****
int antStat(String command) { 
  // *** Get the Antenna Status ($PGTOP).
  Pgtop pgtop = Pgtop(_gps);
  if (pgtop.parse()) {
    Serial.println("1) Antenna Status ($PGTOP)");
    Serial.println("======================================================");
    Serial.print("Antenna Status: "); Serial.println(pgtop.reference);
    Serial.println("");
    if (String(pgtop.reference) == "1") {
        Particle.publish("Antenna Status: ", "Antenna short/problem", 31536000, PRIVATE);
        return 1;
    } else if (String(pgtop.reference) == "2") {
        Particle.publish("Antenna Status: ", "Internal", 31536000, PRIVATE);
        return 2;
    } else if (String(pgtop.reference) == "3") {
        Particle.publish("Antenna Status: ", "External", 31536000, PRIVATE);
        return 3;
    } else {
        Particle.publish("Antenna Status: ", String(pgtop.reference), 31536000, PRIVATE);
        return 11;
    }
  }
  return 10;
}


// **** GETS AND PUBLISHES THE GPS DATA ****

int getGPS(String command) { //publishes and triggers sub functions for bearing and heat
  // *** Get the Global Positioning System Fixed Data ($GPGGA).
  Gga gga = Gga(_gps);
  if (gga.parse())
   {
    // Serial.print("UTC Time: "); Serial.println(gga.utcTime);
    // Serial.print("Latitude: "); Serial.println(gga.latitude);
    // Serial.print("North/SouthIndicator: "); Serial.println(gga.northSouthIndicator);
    // Serial.print("Longitude: "); Serial.println(gga.longitude);
    // Serial.print("East/WestIndicator: "); Serial.println(gga.eastWestIndicator);
    // Particle.publish("Lat: ", gga.latitude + gga.northSouthIndicator, 31536000, PRIVATE);
    // Particle.publish("Lon: ", gga.longitude + gga.eastWestIndicator, 31536000, PRIVATE);
    
    double latN = convertDegMinToDecDeg(gga.latitude.toFloat()); //in Decimal Degrees
    //.toFloat() converts the string from the GPS to for the convert function to work
    if (String(gga.eastWestIndicator) == "N") {
        latN = latN * (-1);
    }
    double lonN = convertDegMinToDecDeg(gga.longitude.toFloat()); //in Decimal Degrees
        if (String(gga.eastWestIndicator) == "W") {
        lonN = lonN * (-1);
    }
    
    lat = latN; //save to global variables - These are floats
    lon = lonN; 
    
    // these are for using in cloud variables - Doubles
    latPublic = (double)lat;
    lonPublic = (double)lon;

    // Particle.publish("Lat: ", String(latN), 31536000, PRIVATE); //debug
    // Particle.publish("Lon: ", String(lonN), 31536000, PRIVATE); //debug
    location = String(latN) + ", " + String(lonN); // stores it in a cloud variable
    // Particle.publish("LOCATION: ", String(latN) + ", " + String(lonN), 31536000, PRIVATE); //DEBUG - publishes the whole string for easy GPS lookup
    // Serial.print("Latitude: "); Serial.println(latN,6);
    // Serial.print("Longitude: "); Serial.println(lonN,6);
    // Serial.print("Position Fix Indicator: "); Serial.println(gga.positionFixIndicator);
    // Serial.print("Satellites Used: "); Serial.println(gga.satellitesUsed);
    // Serial.print("Horizontal Dilution of Precision: "); Serial.println(gga.hdop);
    // Serial.print("Altitude: "); Serial.print(gga.altitude); Serial.print(" "); Serial.println(gga.altitudeUnit);
    // Serial.print("Geoidal Separation: "); Serial.print(gga.geoidalSeparation); Serial.print(" "); Serial.println(gga.geoidalSeparationUnit);
    // Serial.print("Age of Diff. Corr.: "); Serial.println(gga.ageOfDiffCorr);
    Serial.println("");
    

//   Rmc rmc = Rmc(_gps); // **** GPS DIRECTION *** Not accurate
//   if (rmc.parse())
//   {
//     Serial.print("Course Over Ground: "); Serial.println(rmc.courseOverGround);
//     Particle.publish("COG: ", String(rmc.courseOverGround), 31536000, PRIVATE);
  }
  
  if (gga.latitude!=NULL) { // Return 0 if no GPS data is available yet
    return 1; 
  } else {
      return 0;
  }
}

// *** CONVERTS Degrees+minutes to Decimal degrees (easier on the google maps)

double convertDegMinToDecDeg (float degMin) {
  double min = 0.0;
  double decDeg = 0.0;
 
  //get the minutes, fmod() requires double
  min = fmod((double)degMin, 100.0);
 
  //rebuild coordinates in decimal degrees
  degMin = (int) ( degMin / 100 );
  decDeg = degMin + ( min / 60 );
 
  return decDeg;
}

// *** TARGETING FUNCTIONS

int setTarget(String command) { //sets the target destination GPS, accept Lat+Lan in a string, separated by a comma or a comma+space, as given by google maps
    // example from google maps - https://screencast.com/t/3M5zccZ6w
    // 37.779098, -122.415871 (SF Public library)
    
        int i = command.indexOf(','); // separator is a comma
        //LAT
        String tarLatTemp = command.substring(0,i); //splits the string to two, where the comma is
        tarLatTemp = tarLatTemp.trim(); // removes white space from before or after
        targetLat = tarLatTemp.toFloat(); //cast it to a Float
        //LON
        String tarLonTemp = command.substring(i+1);
        tarLonTemp = tarLonTemp.trim(); //removes white space
        targetLon = tarLonTemp.toFloat();
        
        Particle.publish("targetLat: ", String(targetLat), 31536000, PRIVATE);
        Particle.publish("tarLon: ", String(targetLon), 31536000, PRIVATE);
        oldLoc = command; //saves the location to the old location
        
    
        getDirection(); // gets the cardinal directionality
        getBearing(); //gets the bearing in degrees
        heatBearing(); //turns on the relveant heater

        return 1;
}

void getDirection() { //only gets caridnal directions, which is not very helpful 
    // Norther is larger numbers
    // Easter is larger number (In negative it means less negative, obviously)
    // This probably needs extra math for past 0 points in latitude or longitude
    if (targetLat > lat) {
        directionality = "N";
    } else if (targetLat < lat) {
        directionality = "S";
    } else {
        directionality = "X"; // x marks the spot
    }
    
    if (targetLon > lon) {
        directionality = directionality + "E";
    } else if (targetLon < lon) {
        directionality = directionality + "W";
    } else {
        directionality = directionality + "X";
    }
}


void getBearing() { //calculates angle from location to destination based on GPS (no compass yet)
    //startLat,startLong,endLat,endLong 
    
    float startLatTemp = (float)latPublic; //takes the public variable for GPS current location
    float startLongTemp = (float)lonPublic; 
    float endLatTemp = (float)targetLat; //takes the public variable for target
    float endLongTemp = (float)targetLon;
    
    //convert to Radians
    float startLat = radians(startLatTemp); 
    float startLong = radians(startLongTemp);
    float endLat = radians(endLatTemp);
    float endLong = radians(endLongTemp);
    
    float dLong = endLong - startLong;
    
    float dPhi = log(tan(endLat/2.0+PI/4.0)/tan(startLat/2.0+PI/4.0));
    
      if (abs(dLong) > PI) {
        if (dLong > 0.0) {
           dLong = -(2.0 * PI - dLong);
        } else {
           dLong = (2.0 * PI + dLong);
        }
     }

  bearing = (double)(fmod((degrees(atan2(dLong, dPhi)) + 360.0),360.0)); //saves the angle as bearing
  //fmod is modulo for C++ in floats
  //Particle.publish("bearing", String(bearing), 31536000, PRIVATE); // for debug
}

// *************** USES BEARING TO CONTROL HEATING ***************

void heatBearing() {
    float heatBearing; //turns on the relevant heat pad using the heat() function 

    // A0 is North and then in 45 deg jumps clockwise - 2,3,4,A1(S),5,6,A2
    // account for the direction I'm facing based on the compass reading
    if ((bearing - compassPublic) > 0) { 
        heatBearing = bearing - compassPublic;
    } else {
        heatBearing = bearing - compassPublic +360;
    }
    
    if (heatBearing == 0) { // only for Specific 0 which will appear if I'm missing a GPS reading or target
        Particle.publish("No Bearing", NULL, 31536000, PRIVATE);
        heat("n"); //turn off heaters
    } else if (heatBearing > 337.5 || heatBearing <= 22.5) {
        heat("A0");
    } else if (heatBearing > 22.5 && heatBearing <= 67.5) {
        heat("2");
    } else if (heatBearing > 67.5 && heatBearing <= 112.5) {
        heat("3");
    } else if (heatBearing > 112.6 && heatBearing <= 157.5) {
        heat("4");
    } else if (heatBearing > 157.5 && heatBearing <= 202.5) {
        heat("A1");
    } else if (heatBearing > 202.5 && heatBearing <= 247.5) {
        heat("5");
    } else if (heatBearing > 247.5 && heatBearing <= 292.5) {
        heat("6");
    } else if (heatBearing > 292.5 && heatBearing <= 337.5) {
        heat("A2");
    } 
    
       
}


