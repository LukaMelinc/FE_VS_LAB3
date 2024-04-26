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
	int SIZEI = 640*2;
	int SIZE_SCREEN = d * 2 - 1280;
	char *buf, *bufO, *bufB;
	buf = (char *) malloc(sizeof(char) * SIZEI);
	
	fi = open("fifo_izhod", O_RDONLY);
	fo = open("/dev/fb0", O_WRONLY);
	while(1){
		for (int i = 0; i < 480; i++) {
			//buf = (char *)malloc(sizeof(char) * SIZEI);
			read(fi, buf, SIZEI);
			
			//memset(buf, 255, SIZEI);

			write(fo, buf, SIZEI);
			lseek(fo,SIZE_SCREEN,SEEK_CUR);
			//free(buf);
			//memset(buf,255, SIZEI);
			//write(fo,buf,SIZEI);
		
			if((lseek(fo,0,SEEK_CUR)) >= (d * 2 * 480)){
				lseek(fo,0,SEEK_SET);
			}
			
			//memset(buf, 255, SIEZI);
			//write(fo,buf,SIZEI);
		
		}
	}
    
	close(fi);
	close(fo);
	
        free(buf);

        printf("Done!\n");

        return 0;
}
