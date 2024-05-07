#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fi, fo;
    int screenWidth = 1280;  // Width of the actual screen in pixels
    int imageWidth = 640;    // Width of the image to display
    int height = 480;        // Height in rows
    int bytesPerPixel = 2;   // Assuming each pixel uses 2 bytes

    int imageRowSize = imageWidth * bytesPerPixel;  // Bytes per row of the image
    int screenRowSize = screenWidth * bytesPerPixel; // Bytes per row of the screen

    // Allocate buffer to hold one row of image data
    char *buf = (char *)malloc(imageRowSize);
    if (!buf) {
        perror("Failed to allocate buffer");
        return 1;
    }

    // Open input and output files
    fi = open("izhod.raw", O_RDONLY);
    if (fi == -1) {
        perror("Failed to open input file");
        free(buf);
        return 1;
    }

    fo = open("/dev/fb0", O_WRONLY);
    if (fo == -1) {
        perror("Failed to open output file");
        close(fi);
        free(buf);
        return 1;
    }

    // Read and write each row of the frame
    for (int i = 0; i < height; i++) {
        int bytesRead = read(fi, buf, imageRowSize);
        if (bytesRead != imageRowSize) {
            fprintf(stderr, "Read error: Expected %d bytes, got %d bytes\n", imageRowSize, bytesRead);
            break;
        }

        // Set the position at the start of the current row
        lseek(fo, i * screenRowSize, SEEK_SET);
        int bytesWritten = write(fo, buf, imageRowSize);
        if (bytesWritten != imageRowSize) {
            fprintf(stderr, "Write error: Expected %d bytes, wrote %d bytes\n", imageRowSize, bytesWritten);
            break;
        }
    }

    // Clean up resources
    close(fi);
    close(fo);
    free(buf);

    return 0;
}
