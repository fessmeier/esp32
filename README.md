#ESP32 Projects
This is a place for tracking my ESP32 projects.  
My development environment is a Mac book using Visual Studio Code as the IDE.  


## Projects
**OLED display** using the Adafruit Featherwing OLED 128x32 pixel display and the ESP32-DevKit C.  
The ESP_IDF framwork used is "esp-idf-v2.0-rc1".  

## Visual Studio Code
I started with Eclipse and CDT as my IDE.  However, when I tried to debug C/C++ code the debugger would not work.  GDB works fine from the command line just not from Eclipse.  Also, when I tried debuggging the ESP32 through JTAG it did not work so well.  This was mainly due to the ESP32 and OpenOCD not Eclipse.  Since the combination of Eclipse and OpenOCD could not provide easy debugging I decided to just go with a simple editor that could be configured to compile code.  VS Code seemed to fit the bill and so far it's doing what I expected.  Hopefully in the near future I can try getting it set up for debugging.  

In my first project I have the VS Code settings folder included.  The settings allow for the project to be compiled with "make all", "make app", "make clean", and "make flash".  The "make menuconfig" is not included since it needs to run in a terminal and that does not seem to work in VS Code.  However, VS Code does have a built in terminal so "make menuconfig" can be run inside that.

If you decide to try VS Code then you will need to edit the two config files.  The "tasks.json" file will need to be modified with the correct IDF_PATH for your system.  The "c_cpp_properties.json" will need to be modified to point to the correct include paths for your system.

The default setting for a build is for "make all".  To start a build press shift/command/B.  If you want to select another build type then press command/P and type in "task " with a space after the word task and a list will pop up with the available options.
