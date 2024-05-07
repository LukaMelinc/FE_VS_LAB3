#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// gcc -o copy_camera.c copy_camera

int main() {
    const char *input_path = "/dev/video0";
    const char *output_path = "~/VS/vhod.raw";

    // Each pixel is 3 bytes in RGB24, resolution is 640x480
    const size_t frame_size = 640 * 480 * 3;
    void *buffer = malloc(frame_size);
    if (!buffer) {
        perror("Memory allocation failed");
        return 1;
    }

    // Open the video device with read-only access
    int video_fd = open(input_path, O_RDONLY);
    if (video_fd == -1) {
        perror("Error opening video device");
        free(buffer);
        return 1;
    }

    // Open the output file with read/write, create if it doesn't exist, truncate if it does
    int file_fd = open(output_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd == -1) {
        perror("Error opening output file");
        close(video_fd);
        free(buffer);
        return 1;
    }

    // Read exactly one frame from the video device
    ssize_t n_read = read(video_fd, buffer, frame_size);
    if (n_read == -1) {
        perror("Error reading from video device");
        close(video_fd);
        close(file_fd);
        free(buffer);
        return 1;
    }

    // Write the frame to the output file
    ssize_t n_written = write(file_fd, buffer, n_read);
    if (n_written == -1) {
        perror("Error writing to output file");
        close(video_fd);
        close(file_fd);
        free(buffer);
        return 1;
    }

    // Clean up
    free(buffer);
    close(video_fd);
    close(file_fd);

    return 0;
}
