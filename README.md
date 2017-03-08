# BacklightTooler
Lightweight C utility to controll your backlight on most linux laptops. 

# How to use
Use the install.sh script to compile and install BacklightTooler. BacklightTooler gets installed to /usr/bin/BacklightTooler owned as root with setuid. This makes it easy to use BacklightTooler for keybindings etc.
After instalation BacklightTooler can be run by executing it from command line or any command/program launcher. I'd recommend setting it up with keybindings in your windowmanager.

# Usage
Some examples:
```
BacklightTooler inc  # increments backlight with 10
BacklightTooler inc 25 # increments backlight with 25
BacklightTooler dec # Decrements
BacklightTooler set 100 # Sets backlight to 100.
BacklightTooler auto #Automaticly sets the backlighting using the webcam
```

#Options
All commands also take an 'amount' value that's 10 by default. For inc, dec, set its just the amount to in/decrease or set to. However pulse and auto also take an amount. For pulse it's the speed, and for amount it's the light sensitivity. Both have a default value of 10 as well, so doing auto 20 for example, will make the auto function twice as sensitive.
