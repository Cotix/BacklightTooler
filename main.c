#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_BRIGHTNESS 416
#define MIN_BRIGHTNESS 0
#define PATH "/sys/class/backlight/intel_backlight/brightness"
int getBrightness();
void setBrightness(int target);

void inc() {
	int brightness = getBrightness() + 10;
	brightness = brightness > MAX_BRIGHTNESS ? MAX_BRIGHTNESS : brightness;
    setBrightness(brightness);
}

void dec() {
    int brightness = getBrightness() - 10;
    brightness = brightness < MIN_BRIGHTNESS ? MIN_BRIGHTNESS : brightness;
    setBrightness(brightness);
}

void setBrightness(int target) {
	char buffer[256];
	sprintf(buffer, "%i", target);
	FILE* file = fopen(PATH, "w");
    fwrite(buffer, 4, 1, file);
	fclose(file);
}

void pulse() {
    int low = 1;
    int high = MAX_BRIGHTNESS;
    int t;
    int counter = 30;
    while (counter --> 0){
        for (t = low; t < high; ++t) {
            setBrightness(t);
            usleep(2000);
        }
        for (t = high; t > low; --t) {
            setBrightness(t);
            usleep(2000);
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

int main(int argc, char **argv) {
	if (argc != 2) return 1;
	if (!strcmp(argv[1], "inc")) inc();
	if (!strcmp(argv[1], "dec")) dec();
	if (!strcmp(argv[1], "pulse")) pulse();
}
