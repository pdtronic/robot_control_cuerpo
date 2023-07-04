                   .:                     :,                                          
,:::::::: ::`      :::                   :::                                          
,:::::::: ::`      :::                   :::                                          
.,,:::,,, ::`.:,   ... .. .:,     .:. ..`... ..`   ..   .:,    .. ::  .::,     .:,`   
   ,::    :::::::  ::, :::::::  `:::::::.,:: :::  ::: .::::::  ::::: ::::::  .::::::  
   ,::    :::::::: ::, :::::::: ::::::::.,:: :::  ::: :::,:::, ::::: ::::::, :::::::: 
   ,::    :::  ::: ::, :::  :::`::.  :::.,::  ::,`::`:::   ::: :::  `::,`   :::   ::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  :::::: ::::::::: ::`   :::::: ::::::::: 
   ,::    ::.  ::: ::, ::`  :::.::    ::.,::  .::::: ::::::::: ::`    ::::::::::::::: 
   ,::    ::.  ::: ::, ::`  ::: ::: `:::.,::   ::::  :::`  ,,, ::`  .::  :::.::.  ,,, 
   ,::    ::.  ::: ::, ::`  ::: ::::::::.,::   ::::   :::::::` ::`   ::::::: :::::::. 
   ,::    ::.  ::: ::, ::`  :::  :::::::`,::    ::.    :::::`  ::`   ::::::   :::::.  
                                ::,  ,::                               ``             
                                ::::::::                                              
                                 ::::::                                               
                                  `,,`


http://www.thingiverse.com/thing:1358552
44mm Mecanum Wheel (Small, Solid and Low Cost) by Jonah_Innoart is licensed under the Creative Commons - Attribution - Share Alike license.
http://creativecommons.org/licenses/by-sa/3.0/

# Summary

These are Mecanum wheels designed for small factor robotic systems.

It's a low-cost design, the only non-printed parts required is some dressmaker pins.

Testing video:
https://www.youtube.com/watch?v=WKruDl5lpHE

Specifications:
* Wheel Diameter: 44mm
* Wheel Width: 21.4mm
* Axle Diameter: 3mm
* Accessory Parts: 27mm dressmaker pins

*UPDATE - 2016/04/12*
Some ask for more about the rover so I uploaded the basic parts I used to build the rover in the video. Please read the last section *How to Build a Rover*.

# Print Settings

Printer Brand: Up!
Printer: Up Mini
Rafts: Doesn't Matter
Supports: No
Resolution: 0.2mm
Infill: 0%

Notes: 
This model is easy to print for most printers, as long as you follow these instructions:

Print with *NO SUPPORTS*. This is important because this model is designed as a self-supporting shell structure. Adding supports will not give better results but ruin the surface.

Also, print with *NO FILLS*. The model has a closed shell which is strong enough for any platform at this size.

# Post-Printing

## How to Assemble

Put a roller into a slot on the wheel body, and push an dressmaker pin through to mount them together.

*BE VERY CAREFUL* when working with those pins. You will need some practice before you can install a roller without bending the pin or hurting your fingers.

Based on your printer's resolution, the mounting holes might be too small. If that happens, rework the holes with a drill. Make sure that the rollers can roll smoothly.



![Alt text](https://thingiverse-production-new.s3.amazonaws.com/assets/22/0b/16/41/c3/th.jpg)
27mm Dressmaker Pins

## How to Improve Performance

The performance of a Mecanum wheel depends heavily on the grip between rollers and ground. Print the rollers with finest resolution will improve it.

Additionaly, I found it helpful to cover the rollers with a piece of heat shrink tube. I tested it with 12mm heat shrink tube and it improves a lot, as shown in the video.

![Alt text](https://thingiverse-production-new.s3.amazonaws.com/assets/a3/6a/2a/db/57/th_1.jpg)
Heat Shrink Tube can improve grip

## How to Build a Rover

This design is about wheels, not about rover. The rover in the demo video was far from satisfactory. It only served as a testbench for the wheels. But if you would like to build a similar one, here is some advice: 

###BOM:

Printable:
* Top plate                                 x1
* Bottom plate                            x1
* Mecanum wheel                      x4
* Motor mount                            x4

Non-printable:
* N20 Gear Box Motor               x4
* L9110S Dual Motor Driver       x2
* Logeek Max10 Dev Board       x1
* ESP8266 Wifi Module              x1 
* 2S Li-ion battery                      x1

and several Hex Rod Coupling Nuts & Screws  

###About Motors:

Some will prefer servos instead of DC motor, like [this one](http://www.thingiverse.com/thing:5681) by @Madox. If you want a serious work, you should read his instructions instead of mine.
I choose 100RPM N20 Motor because it's smaller and faster. However it might be tricky to handle speed differences between motors.

###About Electronics:

As a FPGA engineer, I use Logeek Max10 which is a FPGA dev board, and my program is witten in verilog instead of C code. That is why I suggest you look into other designs for algorithms. If you use Arduino UNO, there is a problem that L9110S requires two PWMs per motor, that gives eight PWMs in total. Since UNO does not have that many PWMs, I guess you have to hand code GPIO to emulate PWMs. For Arduino Mega or newer models, it is not a problem since they have enough PWMs.

###About RC:

I use a ESP8266 WiFi module and a client App on Android for remote controller. In fact it is my colleague's work and I borrowed it. There should be a lot of reference designs for ESP8266.

There is a simple principle for mecanum controller. To get full control on mecanum wheels, at least three control channels required. One for overall throttle, one for left/right differential, and another for front/back differential. The speed of each wheel is a sum of the three signed components. 

I truely hope this will help, but what you will get largely depends on what you already have on your hand. This why we call it DIY. 

Have fun!

![Alt text](https://cdn.thingiverse.com/assets/3b/de/37/7d/69/images.jpg)
N20 Gear Box Motor

![Alt text](https://cdn.thingiverse.com/assets/48/b9/4f/77/77/logeek_thumbnail.jpg)
Logeek MAX 10 Dev-Board