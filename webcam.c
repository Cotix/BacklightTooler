#include "webcam.h"
int getLightLevel() {
    int fd = open(VIDEO, O_RDWR);
    struct v4l2_capability cap;
    struct v4l2_format format;
    ioctl(fd, VIDIOC_QUERYCAP, &cap);
    format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
    format.fmt.pix.width = 1280;
    format.fmt.pix.height = 720;

    ioctl(fd, VIDIOC_S_FMT, &format);
    struct v4l2_requestbuffers bufrequest;
    bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufrequest.memory = V4L2_MEMORY_MMAP;
    bufrequest.count = 1;

    ioctl(fd, VIDIOC_REQBUFS, &bufrequest);
    struct v4l2_buffer bufferinfo;
    memset(&bufferinfo, 0, sizeof(bufferinfo));

    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;

    ioctl(fd, VIDIOC_QUERYBUF, &bufferinfo);

    void* buffer_start = mmap(
        NULL,
        bufferinfo.length,
        PROT_READ | PROT_WRITE,
        MAP_SHARED,
        fd,
        bufferinfo.m.offset
    );

    memset(buffer_start, 0, bufferinfo.length);
    memset(&bufferinfo, 0, sizeof(bufferinfo));

    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;

    int type = bufferinfo.type;
    ioctl(fd, VIDIOC_STREAMON, &type);
    ioctl(fd, VIDIOC_QBUF, &bufferinfo);
    ioctl(fd, VIDIOC_DQBUF, &bufferinfo);
    ioctl(fd, VIDIOC_STREAMOFF, &type);

    long total = 0;
    int i = 0;
    for ( i = 0; i != bufferinfo.length; ++i) total += ((unsigned char*)buffer_start)[i];
    munmap(buffer_start, bufferinfo.length);
    return total/i;
}
