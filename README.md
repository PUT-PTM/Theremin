# Theremin

## Overview
Theremin based on microcontroller and sensors. But what it is? Theremin is an electronic musical instrument controlled without physical contact by the performer. Sounds cool? It is even cooler. If Mozart was still alive, he would play theremin.

## Description

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
  
- STM  <-> HC-SR04 #2 (Effect modulation)
  
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

## Future improvments
- Simple Delay / Reverb / Overdrive / Volume modulation
## Attributions

## License
MIT
## Credits

Authors: Łukasz Dawydzik,  Michał Andrzejewski

The project was conducted during the Microprocessor Lab course held by the Institute of Control and Information Engineering, Poznan University of Technology. Supervisor: Tomasz Mańkowski
