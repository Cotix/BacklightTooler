#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_BRIGHTNESS 416
#define MIN_BRIGHTNESS 0
#define PATH "/sys/class/backlight/intel_backlight/brightness"
int getBrightness();

void inc() {
	char buffer[256];
	int brightness = getBrightness() + 10;
	brightness = brightness > MAX_BRIGHTNESS ? MAX_BRIGHTNESS : brightness;
	sprintf(buffer, "%i", brightness);
	FILE* file = fopen(PATH, "w");
	fwrite(buffer, strlen(buffer), 1, file);
	fclose(file);
}

void dec() {
    char buffer[256];
    int brightness = getBrightness() - 10;
    brightness = brightness < MIN_BRIGHTNESS ? MIN_BRIGHTNESS : brightness;
    sprintf(buffer, "%i", brightness);
	FILE* file = fopen(PATH, "w");
    fwrite(buffer, 4, 1, file);
	fclose(file);
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
}
