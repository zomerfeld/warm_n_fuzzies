# Warm n Fuzzies

Warm and Fuzzies is a un (unnecessary) wearable which heats up in the direction of your heart's desire - Home, Friend, Mecca / The Wailing Wall, or the nearest Starbucks.
It uses Particle's controller, currently set up for the [Particle Photon](https://store.particle.io/collections/photon).

#### Status

This project is still in Beta and I will update this page as we go along.
If you have any questions about the code or the componenets, email me at noam@cca.edu.

An [Electron](https://store.particle.io/collections/electron) version coming soon. 

## Components
* Particle Photon
* A GPS Breakout - [Adafruit Ultimate GPS V3](https://www.adafruit.com/product/746)
* A Compass Breakout - [LSM303DLHC](https://www.adafruit.com/product/1120)
* Heat Pads - I'm using [Adafruit's](https://www.adafruit.com/product/1481)
* An External Active GPS Antenna (and the correct connectors + adaptors for it) - you can buy Adafruit's or find something on eBay / Amazon
* A 3.7 Battery to power the Photon (or a USB Bank)
* A 7-12 High Capacity, High Drain battery pack to run the heating circuits (not a regular 9V)
You'll need at least 7000mAh to get the heating pads to really heat up. You can use something like [this](http://a.co/99Ji5B1) or A couple of these [18650 Batteries](http://a.co/b3cm1pK)

***Make sure you buy ones with circuit protection! If they don't say they have it they don't and unprotected batteries of that sort have the potential to burst into fire or explode!***

You can swap some of these components with others, but it mightrequire a library or code tweak.
You can also make your own heating pads or swap the output with something like LEDs or Vibration motors.

## Libraries 
I have created this project using the Particle Web IDE and these libraries. They should all be added automatically with the 'project.properties' file, but in case this fails, you will need to add these libraries to your code, using the libraries tab. 
* Adafruit_Sensor (1.0.2)
* [Particle-GPS](https://github.com/porrey/Particle-GPS) (1.0.4) by Daniel Porrey
* [Adafruit_LSM303_U](https://github.com/zomerfeld/Adafruit_LSM303_U) (1.0.8) by Adafruit's Kevin Townsend (ported by yours truly)  

## Schematic
![Schematic](warm_schematic_v4.png)

## Code Walkthrough

#### Dashboard

#### IFTTT Integration

