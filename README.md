# Warm n Fuzzies

Warm and Fuzzies is an (unnecessary) wearable which **heats up in the direction of your heart's desire** - Home, Friend, Mecca / The Wailing Wall, or the nearest Starbucks.
<p align="center">
<img src="reaji_warm.jpg" width="50%" align="center">
</p>
<br>

It uses Particle's controller, currently set up for the [Particle Photon](https://store.particle.io/collections/photon).

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
* A **7-12V High Capacity, High Drain battery pack** – to run the heating circuits (not a regular 9V)

  You'll need at least 7000mAh to get the heating pads to really heat up. You can use something like [this](http://a.co/99Ji5B1) or A couple of these [18650 Batteries](http://a.co/b3cm1pK)

  :boom::boom::boom:  **Make sure you buy ones with circuit protection! If they don't say they have it, they don't.  
  Unprotected batteries of that sort have the potential to burst into fire or explode!**

  You can swap some of these components with others, but it might require a library or code tweak.
  You can also make your own heating pads or swap the output with something like LEDs or Vibration motors.


## Schematic
![Schematic](warm_schematic_v4.png)<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/warm_schematic_v4.png" target="blank">Open in a new tab</a>
<br>

## Powering this Project

## Operation

## Code Walkthrough
### Libraries 
I have created this project using the Particle Web IDE and these libraries. They should all be added automatically with the 'project.properties' file, but in case this fails, you will need to add these libraries to your code, using the libraries tab. 
* Adafruit_Sensor (1.0.2)
* [Particle-GPS](https://github.com/porrey/Particle-GPS) (1.0.4) by Daniel Porrey
* [Adafruit_LSM303_U](https://github.com/zomerfeld/Adafruit_LSM303_U) (1.0.8) by Adafruit's Kevin Townsend (ported by yours truly)  
<br>

* Important Functions

## Heating Circuit Tutorial
![Schematic](MyCircuit_HeatingPad.png)<a href="https://raw.githubusercontent.com/zomerfeld/warm_n_fuzzies/master/MyCircuit_HeatingPad.png" target="blank">Open in a new tab</a>
<br>

## Caveats and Important notes
* Compass orientation
* Compass Math and Such - if you can fix it let me know 
* GPS Antenna is necessary 



### Dashboard

#### IFTTT Integration

