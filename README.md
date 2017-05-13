# Warm & Fuzzies

Warm and Fuzzies is an (unnecessary) wearable which **heats up in the direction of your heart's desire** - Home, Friend, Mecca / The Wailing Wall, or the nearest Starbucks.
<p align="center">
<img src="reaji_warm.jpg" width="50%" align="center">
</p>
<br>

It uses Particle's dev kit, currently set up for the [Particle Photon](https://store.particle.io/collections/photon), the WiFi version. 

#### Status

This project is still in Beta and I will update this page as we go along.
If you have any questions about the code or the components, email me at noam@cca.edu.

An [Electron](https://store.particle.io/collections/electron) version coming soon. 

# Components
* Particle Photon
* **A GPS Breakout** - [Adafruit Ultimate GPS V3](https://www.adafruit.com/product/746)
* **A Compass Breakout** - [LSM303DLHC](https://www.adafruit.com/product/1120)
* **Heating Pads** - I'm using [Adafruit's](https://www.adafruit.com/product/1481)
* An **External Active GPS Antenna** (and the correct connectors + adaptors for it) - you can buy Adafruit's or find something on eBay / Amazon
* A **power source for the Photon** - either a 3.7v Lipo Battery, or a regular 5V USB Bank. 
* A **7-12V High Capacity, High Drain battery pack** – to run the heating circuits (not a regular 9V) - See [Powering this Project] below.

  You can swap some of these components with others, but it might require a library or code tweak.
  You can also make your own heating pads (a nice resource is available [here](http://etextile-summercamp.org/2013/?p=540) or swap the output with something like LEDs or Vibration motors.


# Schematic
<p align="center">
<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/warm_schematic_v4.png" target="blank"><img src="warm_schematic_v4.png" width="98%" align="center"></a>
</p>
<br>

## Powering this Project
  You'll need at least 7000mAh to get the heating pads to heat up fast and in a noticeable way. The heating pads I'm using are about 11cm in width. Check out the data sheet with the V/Ah tables [here](https://cdn-shop.adafruit.com/datasheets/Ultra+Heating+Fabric.pdf) for more info about calculations. 
  
  To get the Voltage and Amperage needed, I recommend using something like [this](http://a.co/99Ji5B1) or A couple of these [18650 Batteries](http://a.co/b3cm1pK)

  :boom::boom::boom:  **Make sure you buy 18650 batteries with circuit protection! If they don't say they have it, they don't.  
  Unprotected batteries of that sort have the potential to burst into fire or explode!**
  
For the Particle - use a separate power supply (any USB 5V phone battery will work), or connect a 3.3V power source to the VIN pin.

You can also create a voltage regulator circuit to split the power supply to a nice clean 3.3v for the Photon, but make sure to test it before, and use a proper heatsink. This project gets pretty hot already! 

# Setup
These instructions assume a basic knowledge of soldering, programming and a familiarity with the Particle IDE environment. If you're just beginning with Particle, I recommend you check out the [Getting Started guide](https://docs.particle.io/guide/getting-started/start/photon/), and build a few basic circuits first.
If you run into any issue or any question - feel free to email me at noam@cca.edu. 

## Create the Circuit
Build it according to the diagram on either on a breadboard, or on a protoboard. I recommend building and testing with one heatpad at a time.  
If you're brave, after testing the code and the components on a breadboard - sew the components onto a piece of fabric using conductive thread, for easy attachment to a piece of clothing after. 
## Power the Particle
Make sure the particle device is powered separately (using the USB connector or the vin pin) from the heat pads. The heat pads need to be powered by the big power supply, but the Particle can only take 5V through the USB, or 3.3v through the VIN pin. 
Their GNDs need to be connected for switching the heat pads MOSFETs on and off. 
## Upload the code
Either use the [Particle Desktop IDE](https://www.particle.io/products/development-tools/particle-desktop-ide) with the entire fileset from this github repository, or copy the contents of [src/warm.ino] to the [particle web IDE](https://build.particle.io/), and import the libraries specified below under [libraries](#libraries).
## Power the heat pads 
Make sure you plug in the power supply for the heat pads last. The project boots up with heat on, so when you plug in the batteries, one of the heat pads should start heating up (there's a default destination programmed). 
**Test it** - Check carefully with your fingers that:
* Only one heatpad is heating up
* That the batteries are not heating up
* That the MOSFETS are not heating up.

If any of the above is happening, unplug the batteries and check for short circuits or inverted components.

There's also a Particle Function you can use to turn the power to the heating circuit off, called `toggleHeat`. Send a `0`, or any other character but `1` to it to turn the heaters off.  

# Operation
The project is meant to be worn. Attach the heating pads to the inside of a shirt, a vest or a jacket, and wire them to the circuit so the pins align to the diagram below:

The diagram shows the allotment of bearing angles to heating pads. The Cardinal Directions (N/E/S/W) are not true magnetic directions, but based on the direction of the individual. "North" is pointing to the front. 
<p align="center">
 <img src="compass_nums2.png" width="80%" align="center">
</p>
<br>

## Particle Functions
Warm & Fuzzies is controlled using the [Particle Cloud Functions](https://docs.particle.io/reference/firmware/photon/#particle-function-) and [variables](https://docs.particle.io/reference/firmware/photon/#particle-variable-) on [Particle Dashboard](https://console.particle.io/devices). Find the right device under your account while your particle device is turned on and connected, to see the variables and functions:
![Dashboard](ParticleDashboard.png)

### ***Cloud Variables***
* **Compass** - Shows you the compass heading in degrees. The compass heading is not 100% accurate (moves between 80-90% accuracy). If you know a solution to get a better compass reading (and I've tried a lot of different compasses breakouts), email me. 
* **Latitude** & **Longitude** - These show the GPS information in Lon and Lat, received from the GPS. If they show 0, it means the GPS hasn't found the satellites yet. 
* **Location**  - Shows your GPS Location in Decimal Degrees (easier to work in, especially on Google Maps). 
* **heater** - Shows the active heater from the heater array. 
* **Bearing** The compass angle between you and the target location, in degrees.
* **Cardinal** - Shows the cardinal direction between you and the target location. Always shows it with 2 letters, so NNE for example will still show as NE. 
* **isHeatOn** - Tells you if the heating circuits are toggled. 

To update the values displayed for a cloud variable, click the little `GET` button next to it. 

### ***Cloud Functions***
All functions will return 0 or 1. To see the values of those who return a meaningful values, see the Variables above. 

To trigger most of these, send any character on them (other than heat & setTarget which expect specific values).

* **getGPS** - Gets the current GPS location. 
* **setTarget** - Sets the destination target for Warm & Fuzzies. Put in your home location, or any other place of importance using decimal degrees. 
* **heat** - wanted to turn on and off a heatpad manually? send the heatpad number (2-6) or name (A0, A1, A2) to turn it on. Disconnect the GPS if you want to use this, or the GPS will turn another one based on the angle between you and destination. 
* **toggleHeat** - Turns off the heating pads temporarily when sent anything but 0. 1 will turn it back on. 
* **AntennaStat** - Shows the state of the GPS's Antenna (right here on the function). Returns 1, 2, or 3. Based on NMEA codes which is used by the [GPS from adafruit](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps.pdf): 1 means antenna short or problem, 2 means internal (not enough to get a signal indoors usually) and 3 means external (which if you bought it, should show). 
* **getCompass** - This is done automatically, but if you want to get another compass reading, you can send any value to this variable. 

***TO BEGIN*** - There's a built in target destination in the code. To set up your own, input coordinates in the function *SetTarget* on the [Particle Dashboard].
(https://console.particle.io/devices). This will take a location in Lat & Lon, Decimal Degrees (DD). To get lat & lon from google maps, see [this guide](http://www.wikihow.com/Get-Latitude-and-Longitude-from-Google-Maps). 

Warm & Fuzzies will start getting warmer towards the direction of your target. When you move around, so would the warm feeling towards that place, or that person. 

# Code Walkthrough
## Libraries
I have created this project using the [Particle Web IDE](https://docs.particle.io/guide/getting-started/build/photon/) and the libraries below. They should all be added automatically with the 'project.properties' file if you use the [Particle Dev IDE desktop software](https://www.particle.io/products/development-tools/particle-desktop-ide), but in case this fails or if you're using the WEB IDE, you will need to add these libraries to your code, using the libraries tab. 

* Adafruit_Sensor (1.0.2)
* [Particle-GPS](https://github.com/porrey/Particle-GPS) (1.0.4) by Daniel Porrey
* [Adafruit_LSM303_U](https://github.com/zomerfeld/Adafruit_LSM303_U) (1.0.8) by Adafruit's Kevin Townsend (ported by yours truly)  
<br>

The code is fairly well documented with comments inline. There are a few that are worth calling out here and explaining.
If you have any questions, feel free to email me. 

## Code Variables
* `heaters` is an array which stores all of the pins associated with the heating pads. If for whatever reason you need to re-assign pins, first update the list here, then also update the turning on of those pins in the functions `heat` and `heatBearing`.
* `tHeat` - is set to 1 by default, which means that at least one heater will heat up when the project is starting. To stop that from happening automatically, change or toggle it to 0. 
* `coldPin` - An optional pin for an LED that will turn on when all heaters are off. 
* `magxOffset` & `magyOffset` - depending on the type of compass you're using, you might need\able to offset the reading by [calibrating the magnetometer](https://learn.adafruit.com/lsm303-accelerometer-slash-compass-breakout?view=all). Although I still am having some issues with my compass, I haven't found it particularly helpful. 
* `Timer` - Timers are great! They're a good and easy way to call a function every X milliseconds in a non interruptive way. They're built into the [Particle Firmware](https://docs.particle.io/reference/firmware/photon/#software-timers), but there's also an [Arduino Library](http://playground.arduino.cc/Code/SimpleTimer) for them if you need it. 
  * The first timer gets the GPS raw data every 1 ms
  * The second timer (timerCompass) launches the [getCompData](#getCompData) function every 0.5 second to get the compass direction.
  * The third timer (timerWear) launches the [wearable](#wearable) function every second. 

There are also a lot of variables, specifically around coordinates that hold the same information in different variables type (floats, longs, ints, and strings) - these are used for different aspects of the system, like sending debug messages to the particle cloud logs, or for sharing with other variables. 

## Important Functions
### setup
Setup (default function, runs at the beginning of the code) calls all the Particle Cloud Variables & Functions, defines the LED and the heat pad pins, waits, starts the timers and initializes the magnetometer and the GPS.
It also sets up Serial debugging (9600 bps) and holds the default destination (it will revert to it after every restart). If you wanted to change the default destination, do it in the last line of setup: `setTarget("21.422474, 39.826192");`
### loop
the loop function (runs on loop on every particle and arduino device), has a millisecond timer inside (the software timers were too early for it), which waits 8 seconds from bootup time, and then triggers [getGPS] every 10 seconds, to get the processed GPS data. 
### getCompData
Gets the compass data, normalizes it to a heading (0-360 degrees) and saves it into the variable `compassPublic`.
### getComp
Does the same as [getCompData] but also publishes the raw data to Serial, and the heading to Serial and the Particle Cloud log. This one is used less, and mostly for debug. 
### toggleHeat
Associated with the cloud function with the same name, toggles heating on or off.
### heat
This functions receives a pin (from the cloud function directly, or from other functions such as [heatBearing]. 
It checks if heating is set to on or off (using the `tHeat` variable). If it's set to 0, it doesn't heat anything and returns 0. 
If heating is toggled on:
It verifies the pin: if the received command is not a valid pin, it will shut off all heaters and turn the cold LED on. Otherwise, if it's a valid heat pad pin, this will heat the associated pad and will turn off all of the others. 

Returns 1 if the function turned a pin on.
### antStat
Checks the status of the Antenna, associated with the cloud function `AntennaStat`.
Returns a different value for each antenna status and publishes it to the Particle Cloud log. 
### getGPS
Parses the raw GPA data (that is already retrieved every ms), converts it from minutes and degrees to decimal degrees, and saves it to global variables: `lat` & `lon`(as floats) and `latPublic` & `lonPublic` (as doubles for publishing to the cloud).
It also saves the location as a string to the cloud variable `location`.

If there is no GPS data, this function will return a 0 and not 1. 
### setTarget
Associated with the cloud function under the same name, this functions accept a string with coordinates in decimal degrees, cleans up the whitespace, splits it to lat & lon, saves them to `targetLat` & `targetLon`, publishes those to the cloud, and triggers [getDirection], [getBearing], and [heatBearing].
### getDirection
This function is not very useful honestly. It calculates the cardinal direction (i.e NE, SE, SW or NW) between the target destination and the current GPS location of Warm & Fuzzies.
### getBearing
getBearing calculates the angle (in degrees) from current location to target destination based on the GPS data.
Formulas and calculation based on this [example](https://gis.stackexchange.com/questions/29239/calculate-bearing-between-two-decimal-gps-coordinates) (after painful painful conversion to C++).
It saves the angle between the two as a the global variable `bearing`.
### heatBearing
This functions binds it all up!

It calculates the difference between the **bearing** (the angle between us and our destination), and the **compass reading**, and derives what part of the body is facing the destination. It then asks the [heat] function to heat up that area using the compass pin diagram above.

If you've changed the heater pins or the placement of the pads, update this function.

# Caveats and Important notes
## Compass orientation
Compass orientation is difficult and tricky with Arduino. It uses magnetic field detection and can be affected by strong magnetic forces around you, or the angle of the compass board. If any of you finds a good solid way to get 100% accurate compass readings with a breakout board, please let me know or fork this repository.
## GPS Antenna 
GPS Breakouts work well outside with the built in antenna, but not so great indoors. I recommend using an active Antenna (link above in [components]) for all of your GPS project. **ESPECIALLY** during the building of the project. It saves so much time when trying to find satellites when you're in the comfort of your own workshop.

# Heating Circuit Tutorial
<p align="center">
<img src="MyCircuit_HeatingPad.png" width="50%" align="center">
<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/MyCircuit_HeatingPad.png" target="blank">Open in a new tab</a>
</p>
<br>

So, one of the biggest a-ha moments I had in this project was learning about MOSFETS. For the longest time, I haven't paid much attention to MOSFETS vs regular Transistors (BJTs). I assumed they're all pretty much the same thing - you push power through one end to switch on power coming from another leg towards gnd. 
That isn't so simple, and while learning all of that specifically on a heating circuit using high drain batteries, I got a few burns, a melted breadboard and a lot of frustration. 
So a quick guide to Transistors can be had here: https://learn.sparkfun.com/tutorials/transistors. Another good guide for MOSFETS is here - https://oscarliang.com/how-to-use-mosfet-beginner-tutorial/:
> * Unlike bipolar transistors, MOSFET is voltage controlled. While BJT is current controlled, the base resistor needs to be carefully calculated according to the amount of current being switched. Not so with a MOSFET. Just apply enough voltage to the gate and the switch operates.
> * Because they are voltage controlled, MOSFET have a very high input impedance, so just about anything can drive them.

* Figure out if you're using an N Channel MOSFET vs a P channel, and wire it correctly.
* Don't forget to use a pulldown resistor between the gate and the ground, so your heat pads don't start heating up unintentionally. 

I was using an [FQP30N06L](https://www.sparkfun.com/products/10213), an LOGIC N-Channel MOSFET.
[Datasheet](https://cdn.sparkfun.com/datasheets/Components/General/FQP30N06L.pdf)

* Source is connected to ground
* Gate is connected to a pin (and a pulldown 10K resistor that's connected to GND)
* Source is connected to the Heatpad, which is connected to the big power source's V+. 
<p align="center">
<img src="MOSFET.png" width="50%" align="center"><br>
<small>This is the diagram for this specific model. Check the datasheet for your MOSFET before wiring</small>
</p>


...and in short - to save you time, when powering on and off big power things using Arduino or Arduino like boards, MOSFETs are your friends. They require less math, less power to turn on and off, and if you wire them correctly - heat up less. 
Read the datasheets, even though they're scary. 


# IFTTT Integration
One of the really nice things about working with particle and using particle cloud exposed functions and variables is that you can easily integrate it with other devices. One easy example is to tie the `setTarget` to someone's phone using an IFTTT Applet with the[{Particle](https://ifttt.com/particle) and the [Button Widget](https://ifttt.com/do_button) service:
<p align="center">
<img src="ifttt.png" width="80%" align="center">
</p>
<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/ifttt.png" target="blank">Open in a new tab</a>

You can use other Particle Devices, or almost any device that can push a string to Particle via IFTTT. 
