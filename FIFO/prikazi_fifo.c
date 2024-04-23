#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

FILE *ptr;
FILE *write_ptr;

int main( int argc, char *argv[] ){

	int fi, fo, d = 1280, v = 1024;
	int SIZEI = 640*2;
	int SIZE_SCREEN = d * 2 - 1280;
	char *buf, *bufO, *bufB;
	buf = (char *) malloc(sizeof(char) * SIZEI);
	
	fi = open("fifo16", O_RDONLY);
	fo = open("/dev/fb0", O_WRONLY);
	
	while(1){
		read(fi, buf, SIZEI);
		write(fo, buf, SIZEI);
		lseek(fo,SIZE_SCREEN,SEEK_CUR);
		
		if((lseek(fo,0,SEEK_CUR)) >= (d * 2 * 480)){
			lseek(fo,0,SEEK_SET);
		}
	}
    
	close(fi);
	close(fo);
	return 0;
}