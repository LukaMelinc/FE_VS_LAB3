#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {

    while(1) {

        int read = open("vhod.raw", O_RDONLY);      // prebere vhod.raw in zapiše/najprej postavi v izhdod.raw
        int wrtite = open("izhod.raw", O_WRONLY | O_CREAT, 0666);

      

        close(read);
        close(write);
        
  // Delay to sync, adjust as necessary
    }

    return 0;
}