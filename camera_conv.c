#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>


// unsigned short rgb3_to_rgb565(unsigned char r, unsigned char g, unsigned char b){
//     return( ((r >> 3) << 11) |  ((g >> 2) << 5) | (b >> 3));
// }

#define WIDTH 640
#define HEIGTH 480


int main(int argc, char *argv[])
{
    int from_file;
    int to_file;

    int buffer_size_read = 640 * 480 * 3;
    int buffer_size_write = 640 * 480 * 4;

    char *buffer_read;
    unsigned short *buffer_write;

    int pack;
    int img_in;

    from_file = open("/home/pi/vhod.raw", O_RDONLY);
    if (from_file == -1)
    {
        printf("napacen vnos from\n");
        exit(1);
    }
    
    to_file = creat("/home/pi/izhod.raw", 0644);
    if (to_file == -1)
    {
        printf("napacen vnos to\n");
        exit(2);
    }

    buffer_read = (char *)malloc(buffer_size_read);
    buffer_write = (unsigned short *)malloc(buffer_size_write);


    while((img_in = read(from_file, buffer_read, buffer_size_read)) > 0)
    {
        sleep(0.5);
        lseek(from_file, 0, SEEK_SET); //seek set: abs. pomik glede na začetek
        
        for(int i = 0; i <= HEIGTH; i++){
            for(int j = 0; j <= WIDTH; j++){


                unsigned short stara_modra =  (buffer_read[(i*640+j)*3 + 0] >> 3) << 11;
                unsigned short stara_zelena = (buffer_read[(i*640+j)*3 + 1] >> 2) << 5;
                unsigned short stara_rdeca =   buffer_read[(i*640+j)*3 + 2] >> 3;

                unsigned short r = (stara_modra | stara_zelena | stara_rdeca);

                buffer_write[i*640*2+j] = r;


            }
        }
        
        lseek(to_file, 0, SEEK_SET);
        write(to_file, buffer_write, buffer_size_write);
    }       



    return 0;
}


