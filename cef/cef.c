#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define NPOD 640*480*3
#define SCREEN_SIZE 1280

void shrani_sliko(int read_fd);
void obdelaj_sliko(int read_fd, int write_fd);
void prikazi_sliko(int write_fd);

int main(int argc, char *argv[]) {
    int fd1[2], fd2[2];
    pipe(fd1);
    pipe(fd2);

    pid_t pid1 = fork();
    if (pid1 == 0) {
        // Child process 1: read from /dev/video0 and write to fifo24 pipe
        close(fd1[0]);
        close(fd2[0]);
        close(fd2[1]);
        dup2(fd1[1], STDOUT_FILENO);
        shrani_sliko(fd1[1]);
        exit(0);
    }

    pid_t pid2 = fork();
    if (pid2 == 0) {
        // Child process 2: read from fifo24 pipe and write to fifo16 pipe
        close(fd1[1]);
        close(fd2[0]);
        dup2(fd1[0], STDIN_FILENO);
        dup2(fd2[1], STDOUT_FILENO);
        obdelaj_sliko(fd1[0], fd2[1]);
        exit(0);
    }

    // Parent process: read from fifo16 pipe and write to /dev/fb0
    close(fd1[0]);
    close(fd1[1]);
    close(fd2[1]);
    dup2(fd2[0], STDIN_FILENO);
    prikazi_sliko(fd2[0]);

    return 0;
}

void shrani_sliko(int write_fd) {
    char *pom;
    int fi;
    int n_pod;
    n_pod = NPOD/480;
    pom=(char *) malloc(sizeof(char) * n_pod);
    fi = open("/dev/video0", O_RDONLY);

    while (1) {
        read(fi, pom, n_pod);
        write(write_fd, pom, n_pod);
    }

    close(fi);
    close(write_fd);
}

void obdelaj_sliko(int read_fd, int write_fd) {
    int fi, a;
    int SIZEI = 640*3;
    int SIZEO = 640*2;
    char *bufI, *bufO, R, G, B;

    bufI = (char *) malloc(sizeof(char) * SIZEI);
    bufO = (char *) malloc(sizeof(char) * SIZEO);

    while(1) {
        read(read_fd, bufI, SIZEI);

        for(a = 0; a <= 640; a++) {
            R = bufI[a*3];
            G = bufI[a*3+1];
            B = bufI[a*3+2];
            
            bufO[a*2+1] = (R & 0xF8) | ((G & 0xE0)>>5);
            bufO[a*2] = ((G & 0x1C)<<3) | ((B & 0xF8)>>3);
        }

        write(write_fd, bufO, SIZEO);
    }

    close(read_fd);
    close(write_fd);
}

void prikazi_sliko(int read_fd) {
    int fo;
    int SIZEI = 640*2;
	char *buf, *bufO, *bufB;
	buf = (char *) malloc(sizeof(char) * SIZEI);
	
	fo = open("/dev/fb0", O_WRONLY);
	
	while(1){
		read(read_fd, buf, SIZEI);
		write(fo, buf, SIZEI);
		lseek(fo,SCREEN_SIZE,SEEK_CUR);
		
		if((lseek(fo,0,SEEK_CUR)) >= (SCREEN_SIZE * 2 * 480)){
			lseek(fo,0,SEEK_SET);
		}
	}
}