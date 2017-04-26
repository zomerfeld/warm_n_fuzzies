# Warm n Fuzzies

Warm and Fuzzies is a un (unnecessary) wearable which heats up in the direction of your heart's desire - Home, Friend, Mecca / The Wailing Wall, or the nearest Starbucks.
It uses Particle's controller, currently set up for the [Particle Photon](https://store.particle.io/collections/photon).

#### Status

This project is still in Beta and I will update this page as we go along.
If you have any questions about the code or the componenets, email me at noam@cca.edu.

An [Electron](https://store.particle.io/collections/electron) version coming soon. 

## Components
* Particle Photon
* GPS Breakout Adafruit Ultimate GPS V3
* A Compass Breakout - LSM303DLHC
* Heat Pads - I'm using [Adafruit's](https://www.adafruit.com/product/1481)

You can swap these components, some will require a library or code tweak.
You can also make your own heating pads or swap the output with something like LEDs or Vibration motors.

## Libraries 
I have created this project using the Particle Web IDE and these libraries. They should all be added automatically with the 'project.properties' file, but in case this fails, you will need to add these libraries to your code, using the libraries tab. 
* Adafruit_Sensor (1.0.2)
* [Particle-GPS](https://github.com/porrey/Particle-GPS) (1.0.4) by Daniel Porrey
* [Adafruit_LSM303_U](https://github.com/zomerfeld/Adafruit_LSM303_U) (1.0.8) by Adafruit's Kevin Townsend (ported by yours truly)  

## Schematic
![Schematic](warm_schematic_v4.png)

#### Dashboard

#### IFTTT Integration

