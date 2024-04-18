#include <stdio.h>
#include <stdlib.h>

// Function to convert a 24 bpp RGB pixel to a 16 bpp RGB565 pixel
unsigned short rgb24_to_rgb16(unsigned char r, unsigned char g, unsigned char b) {
    return ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
}

int main() {
    FILE *fin, *fout;
    unsigned char rgb24[3];
    unsigned short rgb16;

    // Open input and output files
    fin = fopen("vhod.raw", "rb");
    fout = fopen("izhod.raw", "wb");
    if (fin == NULL || fout == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read 24 bpp image and convert to 16 bpp
    while (fread(rgb24, 3, 1, fin) == 1) {
        rgb16 = rgb24_to_rgb16(rgb24[0], rgb24[1], rgb24[2]);
        fwrite(&rgb16, 2, 1, fout);
    }

    // Close files
    fclose(fin);
    fclose(fout);

    printf("Conversion complete.\n");
    return 0;
}
