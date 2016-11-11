# BacklightTooler
Simple C utility to tool your backlight

# How to use
Use the install.sh script to compile and install BacklightTooler. BacklightTooler gets installed to /usr/bin/BacklightTooler owned as root with setuid. This makes it easy to use BacklightTooler for keybindings etc.
After instalation BacklightTooler can be run by executing it from command line or any command/program launcher. I'd recommend setting it up with keybindings in your windowmanager.

# Usage
Some examples:
```
BacklightTooler inc  # increments backlight with 10


BacklightTooler inc 25 # increments backlight with 20


BacklightTooler dec # Decrements


BacklightTooler set 100 # Sets backlight to 100.
```
