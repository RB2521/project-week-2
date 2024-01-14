# project-week-2
Arduino Code for UoB, Dept EEE, Project week 2 - Digital Health Monitoring

This is all versions of the code we have written for the second project week spanning 08/01/2024 - 12/10/2024. 

NOTE - This code uses libraries declared at the top of the script. These are:
- MAX30100_PulseOximeter.h
- Adafruit_SSD1306.h
- Adafruit_GFX.h
- Wire.h (Is an extension of MAX30100_PulseOximeter.h . It is noted here to be aware of in case an issues arise)

You can find the libraries and examples used to help create the script at the following links:

https://github.com/gabriel-milan/Arduino-MAX30100

https://github.com/adafruit/Adafruit_SSD1306

This code will not run without an OLED display. This script is configured to screen dimensions 96x16. If using a different-sized display, this must be updated in lines 17&18.

This code will not run without the heart rate click installed into the PCB. If error messages continue to appear on the OLED display, please check the connections of the pins and the ICs on the PCB.

**NOTE** The names of the .ino files are not going to work when loaded into the Arduino IDE. To solve this, follow the prompts displayed in the IDE to create a file and edit the name of the file to be able to run and use the script. The reason for the names is simply to keep easier track of where I was in the progression of the week and ensure I did not upload multiple of the same versions.

If you have any issues please email me at rb2521@bath.ac.uk and I will get back to you as soon as I can.

Thanks!

-Rhys
