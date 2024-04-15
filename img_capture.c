#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(){

    int fd_r = open("/dev/video0", O_RDONLY);       //odpremo in beremo (readOnly) iz tega naslova
    int fd_w = open("vhod.raw", O_WRONLY | O_CREAT, 0666);  // write/create file

    //if (fd_r < 0 || fd_w < 0) {
    //    perror("Failed to open file");
    //    return 1;
    //}

    char buffer[4096];
    int bytes_read = read(fd_r, buffer, sizeof(buffer));

    if (bytes_read > 0) {
        write(fd_w, buffer, bytes_read);
    }

    close(fd_r);
    close(fd_w);

    return 0;
}
