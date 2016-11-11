#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX_BRIGHTNESS 416
#define MIN_BRIGHTNESS 0
#define DEFAULT_AMOUNT 10
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

void setBrightness(int target) {
	char buffer[256];
	sprintf(buffer, "%i", target);
	FILE* file = fopen(PATH, "w");
    fwrite(buffer, 4, 1, file);
	fclose(file);
}

void pulse(int amount) {
    int low = 1;
    int high = MAX_BRIGHTNESS;
    int t;
    int counter = 30;
    while (counter --> 0){
        for (t = low; t < high; ++t) {
            setBrightness(t);
            usleep(20000/amount);
        }
        for (t = high; t > low; --t) {
            setBrightness(t);
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

int main(int argc, char **argv) {
	if (argc < 2) {
        puts("Usage: BacklightTool OPTION [AMOUNT]\n"\
             "Options: inc, dec, pulse");
        return 0;
    }
    int amount = DEFAULT_AMOUNT;
    if (argc > 2) amount = atoi(argv[2]);
    if (amount < 0 || amount > MAX_BRIGHTNESS) {
        puts("Invalid amount.\n");
        return 0;
    }
	if (!strcmp(argv[1], "inc")) inc(amount);
	if (!strcmp(argv[1], "dec")) dec(amount);
	if (!strcmp(argv[1], "pulse")) pulse(amount);
}
