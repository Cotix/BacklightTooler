#include "webcam.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#define MAX_BRIGHTNESS 416
#define MIN_BRIGHTNESS 0
#define DEFAULT_AMOUNT 10
#define DEFAULT_SPEED 15

int getBrightness();
void setBrightness(int target);

// This function enables to get the brightness file for systems,
// where backlight brightness is located in non intel folders
// like /sys/class/backlight/acpi_video0/brightness
void getBacklightBrightnessFilePath (char *path)
{
    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir ("/sys/class/backlight/")) != NULL) 
    {
        int found = 0;
        while ((ent = readdir (dir)) != NULL) {
            if ( strcmp(ent->d_name, ".")!=0 && strcmp(ent->d_name, "..")!=0 ) {
                // take first folder and get out
                sprintf (path, "/sys/class/backlight/%s/brightness", ent->d_name);
                found = 1;
                break;
            }
        }

        if (found==0) {
            fprintf (stdout, "Unable to find any folder under /sys/class/backlight/");
            exit (EXIT_FAILURE);
        }

        closedir (dir);
    } else {
        fprintf (stderr, "Unable to get the current systems backlight folder.");
        exit (EXIT_FAILURE);
    }
}

void inc(int amount) {
    int brightness = getBrightness() + amount;
    brightness = brightness > MAX_BRIGHTNESS ? MAX_BRIGHTNESS : brightness;
    setBrightness(brightness);
}

void dec(int amount) {
    int brightness = getBrightness() - amount;
    brightness = brightness < MIN_BRIGHTNESS ? MIN_BRIGHTNESS : brightness;
    setBrightness(brightness);
}

void writeBrightness(int target) {
    if (target < MIN_BRIGHTNESS) target = MIN_BRIGHTNESS;
    if (target > MAX_BRIGHTNESS) target = MAX_BRIGHTNESS;
    char buffer[256];
    sprintf(buffer, "%i", target);
    char brightnessFilePath[512];
    getBacklightBrightnessFilePath (brightnessFilePath);
    FILE* file = fopen(brightnessFilePath, "w");
    fwrite(buffer, 4, 1, file);
    fclose(file);
}

void setBrightness(int target) {
    if (target < MIN_BRIGHTNESS) target = MIN_BRIGHTNESS;
    if (target > MAX_BRIGHTNESS) target = MAX_BRIGHTNESS;
    int current = getBrightness();
    int inc = current > target ? -1 : 1;
    int steps = (target-current)*inc;
    while (steps --> 0) {
        current += inc;
        writeBrightness(current);
        usleep(20000/DEFAULT_SPEED);
    }
}

void pulse(int amount) {
    int low = 1;
    int high = MAX_BRIGHTNESS;
    int t;
    while (1){
        for (t = low; t < high; ++t) {
            writeBrightness(t);
            usleep(20000/amount);
        }
        for (t = high; t > low; --t) {
            writeBrightness(t);
            usleep(20000/amount);
        }
    }
}

int getBrightness() {
    char brightnessFilePath[512];
    getBacklightBrightnessFilePath (brightnessFilePath);
    FILE* file = fopen(brightnessFilePath, "r");
    char buf[16];
    fread(buf, 8, 1, file);
    fclose(file);
    return atoi(buf);
}

void autoBrightness(int amount) {
    int target = getLightLevel()*amount;
    target -= LOW_AVERAGE;
    target *= MAX_BRIGHTNESS;
    target /= HIGH_AVERAGE;
    setBrightness(target);
}

void help() {
    puts("Usage: BacklightTool OPTION [AMOUNT]\n"\
         "Options: set, inc, dec, pulse, auto");
}

int main(int argc, char **argv) {
    if (argc < 2) {
        help();
        return 0;
    }
    int amount = DEFAULT_AMOUNT;
    if (argc > 2) amount = atoi(argv[2]);
    if (amount < 0 || amount > MAX_BRIGHTNESS) {
        puts("Invalid amount.\n");
        return 0;
    }
    if (!strcmp(argv[1], "inc")) inc(amount);
    else if (!strcmp(argv[1], "dec")) dec(amount);
    else if (!strcmp(argv[1], "pulse")) pulse(amount);
    else if (!strcmp(argv[1], "auto")) autoBrightness(amount);
    else if (!strcmp(argv[1], "set") && argc > 2) setBrightness(amount);
    else help();
}
