#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// Function to convert a 24 bpp RGB pixel to a 16 bpp RGB565 pixel
unsigned short rgb24_to_rgb16(unsigned char r, unsigned char g, unsigned char b) {
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

// Function to swap byte order for big-endian system
unsigned short swap_bytes(unsigned short value) {
    return (value >> 8) | (value << 8);
}

int main() {
    int fin, fout;
    unsigned char rgb24[3];
    unsigned short rgb16;
    ssize_t numRead;

    // Open input and output files with POSIX open
    fin = open("vhod.raw", O_RDONLY);
    fout = open("izhod.raw", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fin == -1 || fout == -1) {
        perror("Error opening file");
        return -1;
    }

    // Read 24 bpp image and convert to 16 bpp
    while ((numRead = read(fin, rgb24, 3)) == 3) {
        rgb16 = rgb24_to_rgb16(rgb24[0], rgb24[1], rgb24[2]);
        // Swap bytes for big-endian system
        rgb16 = swap_bytes(rgb16);
        write(fout, &rgb16, 2);
    }

    // Close files using POSIX close
    close(fin);
    close(fout);

    printf("Conversion complete.\n");
    return 0;
}
