# epd4in2bc-co2monitor

Converting images to the e-ink dispay hex array:
https://github.com/Wh1teRabbitHU/ImageToEpaperConverter/tree/master

Repos/ImageToEpaperConverter/ImageToEpaperConverter
$ node standalone.js 
Successfully converted the given image file! Source file: pusheen2.png , target folder: output
ImageToEpaperConverter]$ 


Copy the generated .cpp and .h to the arduino project folder. Remove the AVR include and the PROGMEM keyword.
The Bitmap has to be rotated 180 degrees for some reason.


# Pinout
## epaper:
* Busy:	G25
* RST:	G26
* DC:	G27
* CS:	G15
* CLK:	G13
* DIN:	G14
* GND:	GND
* VCC:	3v3

## SCD14:
* SDA:	G21
* SCL:	G22
* GND:	GND
* VCC:	3V3
