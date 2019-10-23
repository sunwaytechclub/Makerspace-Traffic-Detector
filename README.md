# Makerspace traffic detector

The system uses Adruino Uno to detect human passed by the system and send data to Sigfox backend using UnaShield V2S.

Ways of sending data using UnaShield V2S can be found in the adruino library [example](https://github.com/UnaBiz/unabiz-arduino/blob/master/examples/send-altitude/send-altitude.ino). More information on UnaShield can be found on the official [website](https://unabiz.github.io/unashield/hardware.html).

The threshold of IR detection will affect the distance and accuracy of detection. It is also determined by the strength of the IR transmitter which can be altered using resistor. Be sure alter the distance between the IR transmitter and IR receiver as well as the ressitance of the IR transmitter to find a sweet spot between distance and accuracy of the system.

## Tools

1. Arduino Uno
2. UnaShield V2S
3. Infrared transmitter IR333-A
4. Infrared module TSIP 1738
5. BJT NPN transistor TIP 120
6. Adapter with DC output. Rating: 5V 1A
7. 330 ohm resistor
8. Potentiometer
9. 4 x AA Battery Holder
10. Jumper Wire
11. Breadboard/Donut board

## Schematics

![Schematic](schematics/schematic.png)