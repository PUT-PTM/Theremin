# Theremin

## Overview
Theremin based on microcontroller and sensors. But what it is? Theremin is an electronic musical instrument controlled without physical contact by the performer. Sounds cool? It is even cooler. If Mozart was still alive, he would play theremin.

## Description
Theremin is an university project made by students at Electric Department of Poznan University of Technology. It was being developed since March 2018 till June 2018. It uses two HC-SR04 sensors - one is used to modulate played sound when second one is used for volume modulation

## Tools
- STM32F4 Discovery
- Ultrasonic Ranging Module HC-SR04 x2
- Analog Test Board containing LM386M, speaker, analog input/output

## How to run
You have to connect pins as follows:
- STM  <-> HC-SR04 #1 (Frequency modulation)
  
  5V   <-> VCC  
  Trig <-> PD12  
  Echo <-> PD13  
  GND  <-> GND  
  
- STM  <-> HC-SR04 #2 (Volume modulation)
  
  5V   <-> VCC  
  Trig <-> PB0  
  Echo <-> PB1  
  GND  <-> GND  
  
- STM  <->  Analog Test Board  

  GND  <-> GND  
  5V   <-> 5V  
  Audio<-> PA4  
  Leave the rest unpinned
## How to compile
No additional action is needed to compile (tested only on System Workbench for STM32 based on Eclipse) 

## Future improvments
- Simple audio effects instead of volume modulation (Delay / Reverb / Overdrive) 
## Attributions
Ultrasonic Ranging Module HC-SR04 documentation:
- http://www.playembedded.org/blog/wp-content/uploads/2016/11/HCSR04.pdf

## License
MIT
## Credits

Authors: Łukasz Dawydzik,  Michał Andrzejewski

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology. Supervisor: Tomasz Mańkowski
