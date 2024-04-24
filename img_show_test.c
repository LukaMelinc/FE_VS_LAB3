#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define BYTES_PER_PIXEL 2 // Assuming RGB565 format

int main() {
    int fbfd;
    unsigned short *fbp;
    unsigned short *raw_data;
    long int screensize = SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL;

    // Open the framebuffer device
    fbfd = open("/dev/fb0", O_RDWR);
    if (fbfd == -1) {
        perror("Error opening framebuffer device");
        exit(1);
    }

    // Memory map the framebuffer device
    fbp = (unsigned short *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if ((void *)fbp == MAP_FAILED) {
        perror("Error mapping framebuffer device to memory");
        exit(1);
    }

    // Open the input raw file containing RGB565 pixel data
    FILE *fin = fopen("izhod.raw", "rb");
    if (fin == NULL) {
        perror("Error opening input raw file");
        exit(1);
    }

    // Allocate memory to store raw data
    raw_data = (unsigned short *)malloc(SCREEN_WIDTH * SCREEN_HEIGHT * BYTES_PER_PIXEL);
    if (raw_data == NULL) {
        perror("Error allocating memory for raw data");
        exit(1);
    }

    // Read raw data from file
    if (fread(raw_data, BYTES_PER_PIXEL, SCREEN_WIDTH * SCREEN_HEIGHT, fin) != SCREEN_WIDTH * SCREEN_HEIGHT) {
        perror("Error reading raw data from file");
        exit(1);
    }

    // Close input raw file
    fclose(fin);

    // Display the image on the framebuffer
    for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        // Write pixel value to framebuffer
        fbp[i] = raw_data[i];
    }

    // Unmap framebuffer memory
    munmap(fbp, screensize);
    // Close framebuffer device
    close(fbfd);

    printf("Image displayed on the terminal screen.\n");

    return 0;
}
