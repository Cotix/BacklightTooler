#!/bin/bash
gcc main.c webcam.c -o BacklightTooler
sudo chown root BacklightTooler
sudo chmod +s BacklightTooler
sudo mv BacklightTooler /usr/bin/BacklightTooler
