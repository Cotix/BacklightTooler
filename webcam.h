#ifndef WEBCAM_HEADER
#define WEBCAM_HEADER
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <sys/mman.h>

#define VIDEO "/dev/video0"
#define LOW_AVERAGE 700
#define HIGH_AVERAGE 2000

int getLightLevel();
#endif
