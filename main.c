#include "webcam.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_BRIGHTNESS 416
#define MIN_BRIGHTNESS 0
#define DEFAULT_AMOUNT 10
#define DEFAULT_SPEED 15
#define PATH "/sys/class/backlight/intel_backlight/brightness"

int getBrightness();
void setBrightness(int target);

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
    FILE* file = fopen(PATH, "w");
    fwrite(buffer, 4, 1, file);
    fclose(file);
}

void setBrightness(int target) {
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
    FILE* file = fopen(PATH, "r");
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

int main(int argc, char **argv) {
    if (argc < 2) {
        puts("Usage: BacklightTool OPTION [AMOUNT]\n"\
             "Options: set, inc, dec, pulse, auto");
        return 0;
    }
    int amount = DEFAULT_AMOUNT;
    if (argc > 2) amount = atoi(argv[2]);
    if (amount <= 0 || amount > MAX_BRIGHTNESS) {
        puts("Invalid amount.\n");
        return 0;
    }
    if (!strcmp(argv[1], "inc")) inc(amount);
    if (!strcmp(argv[1], "dec")) dec(amount);
    if (!strcmp(argv[1], "pulse")) pulse(amount);
    if (!strcmp(argv[1], "auto")) autoBrightness(amount);
    if (!strcmp(argv[1], "set") && argc > 2) setBrightness(amount);
}
