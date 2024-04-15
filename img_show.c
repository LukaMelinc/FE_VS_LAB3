#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int read = open("izhod.raw", O_RDONLY);
    int show = open("/dev/fb0", O_WRONLY);

   

   
    close(read);
    close(show);

    return 0;
}