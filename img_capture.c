#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){

    int kamera = open("/dev/video0", O_RDONLY);       //odpremo in beremo (readOnly) iz tega naslova
    int file = open("vhod.raw", O_WRONLY | O_CREAT, 0666);  // write/create file

    //if (fd_r < 0 || fd_w < 0) {
    //    perror("Failed to open file");
    //    return 1;
    //}

    char buffer[4096];      // dodelimo plac za sliko
    int slika = read(kamera, buffer, sizeof(buffer));       //preberemo kamero

    if (slika > 0) {
        write(file, buffer, slika);     // zapišemo sliko
    }

    close(kamera);
    close(file);

    return 0;
}
