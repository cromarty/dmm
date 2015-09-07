
# dmm #

Code to interface to the serial port on Unit-T Digital Multimeters

## History ##

I have a Unit-T UT60A digital multimeter, which has an opto-isolated RS232 serial port.

I am also totally blind.  Although I have another digital multimeter with a talking function, I 
bought this meter for it's potential as a meter with I could make measurements and have them spoken 
for me by the screen-reader on my computer.

But the software supplied with the meter is Windows only and is almost totally inaccessible.

This is because the folks who write consumer product software such as that for a digital multimeter 
think it is clever for the on-screen dialog to look like a meter.  WHich usually results in a 
totally inaccessible display which a screen-reader cannot read.

So I set about writing some Linux, and possibly also Windows code to replace the provided software.

I am aiming to make the software capable of:

* Taking multiple measurements.
* Data-logging.
* Speaking the measurements either with the screen-reader running on the desktop or by interfacing 
to text-to-speech directly.

The software must be:

* Robust and cope with errors such as the meter not being switched on when the program is started.
* Able to read data from the meter asynchronously so as not to block a GUI interface or make it 
sluggish.
* Easy to understand and extend.

