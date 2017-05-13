# Warm n Fuzzies

Warm and Fuzzies is an (unnecessary) wearable which **heats up in the direction of your heart's desire** - Home, Friend, Mecca / The Wailing Wall, or the nearest Starbucks.
<p align="center">
<img src="reaji_warm.jpg" width="50%" align="center">
</p>
<br>

It uses Particle's dev kit, currently set up for the [Particle Photon](https://store.particle.io/collections/photon), the WiFi version. 

#### Status

This project is still in Beta and I will update this page as we go along.
If you have any questions about the code or the componenets, email me at noam@cca.edu.

An [Electron](https://store.particle.io/collections/electron) version coming soon. 

## Components
* Particle Photon
* **A GPS Breakout** - [Adafruit Ultimate GPS V3](https://www.adafruit.com/product/746)
* **A Compass Breakout** - [LSM303DLHC](https://www.adafruit.com/product/1120)
* **Heating Pads** - I'm using [Adafruit's](https://www.adafruit.com/product/1481)
* An **External Active GPS Antenna** (and the correct connectors + adaptors for it) - you can buy Adafruit's or find something on eBay / Amazon
* A **power source for the Photon** - either a 3.7v Lipo Battery, or a regular 5V USB Bank. 
* A **7-12V High Capacity, High Drain battery pack** – to run the heating circuits (not a regular 9V) - See [Powering this Project] below.

  You can swap some of these components with others, but it might require a library or code tweak.
  You can also make your own heating pads or swap the output with something like LEDs or Vibration motors.


## Schematic
<p align="center">
<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/warm_schematic_v4.png" target="blank"><img src="warm_schematic_v4.png" width="98%" align="center"></a>
</p>
<br>

## Powering this Project
  You'll need at least 7000mAh to get the heating pads to really heat up. The heating pad I'm using are about 11cm in width. Check out the data sheet with the V/Ah tables [here](https://cdn-shop.adafruit.com/datasheets/Ultra+Heating+Fabric.pdf).
  
  To get the Voltage and Amperage needed, I recommend using something like [this](http://a.co/99Ji5B1) or A couple of these [18650 Batteries](http://a.co/b3cm1pK)

  :boom::boom::boom:  **Make sure you buy 18650 batteries with circuit protection! If they don't say they have it, they don't.  
  Unprotected batteries of that sort have the potential to burst into fire or explode!**
  
For the Particle - use a separate power supply (like any USB 5V phone battery), or connect a 3.3V power source to the VIN pin. 
You can also create a voltage regulator circuit to split the power supply to a nice clean 3.3v for the Photon, but make sure to test it before, and use a proper heatsink. This project gets pretty hot already! 

## Setup
These instructions assume you have a basic knowledge of soldering, programming and a familiarity with the Particle IDE environment. If you're just beginning with Particle, I recommend you check out the [Getting Started guide](https://docs.particle.io/guide/getting-started/start/photon/), and build a few basic circuit first.
If you run into any issue or any question - feel free to email me at noam@cca.edu. 

### Create the circuit
Either on a breadboard, or on a protoboard. If you're brave - sew the components onto a piece of fabric using conductive thread.
### Power the Particle
Make sure the particle device is powered separately (using the USB connector or the vin pin) than the heat pads. The Heatpads need to be powered by the big power supply, but the Particle can only take 5V through the USB, or 3.3v through the VIN pin. 
Their GNDs need to be connected for switching the heatapd MOSFETs on and off. 
### Upload the code
Either use the Particle Dev IDE with the entire file set from this github repository, or copy the contents of [src/warm.ino] to the [particle online IDE](https://build.particle.io/), and import the libraries specified below under [libraries](#libraries).
### Power the heatpads 
Make sure you plug in the power supply for the heatpads last. The project boots up with heat on, so when you plug in the batteries, one of the heatpads should start heating up (there's a default destination programmed). 
**Test it** - Check carefully with your fingers that:
* Only one heatpad is heating up
* That the batteries are not heating up
* That the MOSFETS are not heating up.

If any of the above is happening, unplug the batteries and check for shortcircuits or inverted components.

There's also a Particle Function you can use to turn the power to the heating circuit off, called *toggleHeat*. Send a zero, or any other character to it to turn the heaters off temporarily.  

## Operation
The project is meant to be worn. Attach the heating pads to the inside of a shirt, a vest or a jacket, and wire them to the circuit so the pins align to the diagram below:

The diagram shows the allotment of bearing angles to heating pads. The Cardinal Directions (N/E/S/W) are not true magnetic directions, but based on the direction of the individual. "North" is pointing to the front. 
![Compass](compass_nums2.png)<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/compass_nums2.png" target="blank">Open in a new tab</a>
<br>

### Particle Functions
Warm & Fuzzies is controlled using the functions and variables on [Particle Dashboard](https://console.particle.io/devices). Find the right device under your account while the particle is turned on and connected to see the variables and functions:
![Dashboard](ParticleDashboard.png)

#### ***Variables***
* **Compass** - Shows you the compass heading in degrees. The compass heading is not 100% accurate (moves between 80-90% accuracy). If you know a solution to get a better compass reading (and I've tried a lot of different compasses breakouts), email me. 
* **Latitude** & **Longitude** - These show the GPS information in Lon and Lat, recieved from the GPS. If they show 0, it means the GPS hasn't found the satelites yet. 
* ** Location **  - Shows your GPS Location in Decimal Degrees (easier to work in, especially on Google Maps). 
* ** heater ** - Shows the active heater from the heater array. 
* ** Bearing ** The compass angle between you and the target location, in degrees.
* ** Cardinal** - Shows the cardinal direction between you and the target location. Always shows it with 2 letters, so NNE for example will still show as NE. 
* ** IsHeatOn ** - Tells you if the heating circuits are toggled. 

To update the variables value, click the little `GET` button. 

#### ***Functions***
All functions will return 0 or 1. To see the values of those who return a meaningful values, see the Variables above. 
To trigger most of these, send any character on them (other than heat & setTarget which expect specific values).

* **getGPS** - Gets the current GPS location. 
* **setTarget** - Sets the destination target for navigation. Put in your home location, or any other place of importance using decimal degrees. 
* **heat** - wanted to turn on and off a heatpad manually? send the heatpad number (2-6) or name (A0, A1, A2) to turn it on. Disconnect the GPS if you want to use this, or the GPS will turn another one based on the angle between you and destination. 
* **toggleHeat** - Turns off the heating pads temporarily when sent anything but 0. 1 will turn it back on. 
* **AntennaStat** - Shows the state of the GPS's Antenna (right here on the function). Returns 1, 2, or 3. Based on NMEA codes which is used by the [GPS from adafruit](https://cdn-learn.adafruit.com/downloads/pdf/adafruit-ultimate-gps.pdf): 1 means antenna short or problem, 2 means internal (not enough to get a signal indoors usally) and 3 means external (which if you bought it, should show). 
* **getCompass** - This is done automatically, but if you want to get another compass reading, you can send any value to this variable. 

Set up a destination using the Particle Function *SetTarget* on the [Particle Dashboard](https://console.particle.io/devices). This takes a location in Lat & Lon, in Decimal Degrees (DD). To get lat & lon from google maps, see [this guide](http://www.wikihow.com/Get-Latitude-and-Longitude-from-Google-Maps). 


## Code Walkthrough
### Libraries
I have created this project using the Particle Web IDE and these libraries. They should all be added automatically with the 'project.properties' file, but in case this fails, you will need to add these libraries to your code, using the libraries tab. 
* Adafruit_Sensor (1.0.2)
* [Particle-GPS](https://github.com/porrey/Particle-GPS) (1.0.4) by Daniel Porrey
* [Adafruit_LSM303_U](https://github.com/zomerfeld/Adafruit_LSM303_U) (1.0.8) by Adafruit's Kevin Townsend (ported by yours truly)  
<br>

* Important Functions

## Heating Circuit Tutorial
<p align="center">
![Schematic](MyCircuit_HeatingPad.png)<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/MyCircuit_HeatingPad.png" target="blank">Open in a new tab</a>
</p>
<br>

## Caveats and Important notes
* Compass orientation
* Compass Math and Such - if you can fix it let me know 
* GPS Antenna is necessary 



### Dashboard

#### IFTTT Integration
* Extras - IFTTT Location with phone

