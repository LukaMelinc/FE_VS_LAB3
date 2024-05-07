#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

FILE *ptr;
FILE *write_ptr;

int main( ){

	int fi, fo, d = 1280, v = 1024;
	int SIZEI = 480*640*2;
	int SIZE_SCREEN = d * 2 - 1280;
    const size_t bytes_per_row = 640 * 2;
	char *buf;
	buf = (char *) malloc(sizeof(char) * SIZEI);
	
	fi = open("izhod.raw", O_RDONLY);
	fo = open("/dev/fb0", O_WRONLY);

    unsigned char *row_pointer = buf;

	for(int j = 0; j < 640; j++){
		for (int i = 0; i < 480; i++) {
			int bytesRead = read(fi, buf, SIZEI);
            if (bytesRead != SIZEI) {
                fprintf(stderr, "Read error: Expected %d bytes, got %d bytes\n", SIZEI, bytesRead);
                break;
            }
            
            for (size_t i = 0; i < 480; i++){
            ssize_t n_written = write(fi, row_pointer, bytes_per_row);
            //test = test +1;
            //printf("Test: %d\n", test);
            if (n_written != bytes_per_row) {
                    perror("Error writing to output file");
                    close(fo);
                    close(fi);
                    free(buf);
                    return 1;
                    }

                row_pointer += bytes_per_row;
                lseek(fo,2*1280*i,SEEK_SET);
                }
                row_pointer =  buf;
            }

            // if (bytesWritten != SIZEI) {
            //     fprintf(stderr, "Write error: Expected %d bytes, wrote %d bytes\n", SIZEI, bytesWritten);
            // }
			//lseek(fo,2*1280*i,SEEK_SET);
		}
		lseek(fo,0,SEEK_SET);
	}
    
	close(fi);
	close(fo);
	
        free(buf);

        printf("Done!\n");

        return 0;
}
