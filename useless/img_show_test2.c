#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

unsigned short swap_bytes(unsigned short value) {
    return (value >> 8) | (value << 8);
}

int main() {
    const char *fb_path = "/dev/fb0";
    const char *img_path = "izhod.raw";
    int fbfd, imgfd;
    struct fb_var_screeninfo vinfo;
    struct fb_fix_screeninfo finfo;
    char *fbp = 0;

    // Open the framebuffer device
    fbfd = open(fb_path, O_RDWR);
    if (fbfd == -1) {
        perror("Error opening framebuffer device");
        return EXIT_FAILURE;
    }

    // Get fixed and variable screen information
    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1 || ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading screen information");
        close(fbfd);
        return EXIT_FAILURE;
    }

    // Map framebuffer to user memory
    long int screensize = vinfo.yres_virtual * finfo.line_length;
    fbp = (char*)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        perror("Failed to map framebuffer device to memory");
        close(fbfd);
        return EXIT_FAILURE;
    }

    // Open the image file
    imgfd = open(img_path, O_RDONLY);
    if (imgfd == -1) {
        perror("Error opening image file");
        munmap(fbp, screensize);
        close(fbfd);
        return EXIT_FAILURE;
    }

    // Calculate the size of the image data
    size_t img_size = 640 * 480 * 2; // 640x480, 2 bytes per pixel (RGB565)
    char *img = (char *)malloc(img_size);
    if (!img) {
        perror("Failed to allocate memory for the image");
        close(imgfd);
        munmap(fbp, screensize);
        close(fbfd);
        return EXIT_FAILURE;
    }

    // Read image data from file
    if (read(imgfd, img, img_size) != img_size) {
        perror("Failed to read image data");
        free(img);
        close(imgfd);
        munmap(fbp, screensize);
        close(fbfd);
        return EXIT_FAILURE;
    }

    // Write image data to framebuffer with byte swapping if needed
    for (int y = 0; y < 480; y++) {
        for (int x = 0; x < 640; x++) {
            unsigned short *pixel = (unsigned short *)(img + y * 640 * 2 + x * 2);
            unsigned short swapped_pixel = swap_bytes(*pixel);
            memcpy(fbp + y * finfo.line_length + x * 2, &swapped_pixel, 2);
        }
    }

    // Cleanup
    free(img);
    close(imgfd);
    munmap(fbp, screensize);
    close(fbfd);

    return EXIT_SUCCESS;
}
