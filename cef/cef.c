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
    
    int fd1[2], fd2[2];     // two arrays, each hold two flie descriptors that form the ends of popes
    
    pipe(fd1);  //initializes the pipes fd1 and fd2
    pipe(fd2);  //each pipe call sets up two file descriptors, first for reading, the second for writing

    pid_t pid1 = fork();    // duplicates the current process
    if (pid1 == 0) {
        // Child process 1: read from /dev/video0 and write to fifo24 pipe (sends to fd1)
        close(fd1[0]);  //we close unused pipe ends, we don't read from fd1 and don't use fd2 at all
        close(fd2[0]);
        close(fd2[1]);
        dup2(fd1[1], STDOUT_FILENO);    //duplicate the file descriptor fd1 (the write end) to STDOUT_FILENO, redirecting the output to this pipe
        shrani_sliko(fd1[1]);   //reads data from dev and writes to fd1[1]
        exit(0);    //exit the child
    }

    pid_t pid2 = fork();        //drugi fork, ustvarimo drugega otroka, podeduje file descriptor
    if (pid2 == 0) {
        // Child process 2: read from fifo24 (fd1) pipe and write to fifo16 pipe
        close(fd1[1]);  //close unnecessary pipes (write from fd1 and read from fd2)
        close(fd2[0]);
        dup2(fd1[0], STDIN_FILENO);     //standard inpit to fd1 read
        dup2(fd2[1], STDOUT_FILENO);    //standard output to fd2
        obdelaj_sliko(fd1[0], fd2[1]);  //call obdelaj fucntion with pipes connected
        exit(0);
    }

    // Parent process: read from fifo16 (fd2) pipe and write to /dev/fb0 to display
    close(fd1[0]);  //close fd1 and fd2 read
    close(fd1[1]);
    close(fd2[1]);
    dup2(fd2[0], STDIN_FILENO); //fd2 write as standard input
    prikazi_sliko(fd2[0]);

    return 0;
}

void shrani_sliko(int write_fd) {   //write_fd - file descriptor of the write-end pipe
    // open the video device for reading
    //reads chunks of data and writes them to the file descriptor (write_fd)
    //chunks according to pixel amount - NPOD and the vertical dimension
    char *pom;      //pointer to char, boints to a memory buffer for reading data
    int fi; //stores fiče descriptor for video device
    int n_pod;      //determine the size of chunks of data to be sent
    n_pod = NPOD/480;       //entire frame/height
    pom=(char *) malloc(sizeof(char) * n_pod);      //buffer for one row of data
    fi = open("/dev/video0", O_RDONLY);

    while (1) {     //each iteration reads a chink of data from video device into buffer pom and writes to write_fd
        read(fi, pom, n_pod);       //file descriptor for the video deivce, buffer fo storing data, number of bytes to read
        write(write_fd, pom, n_pod);    //sends the data to the file descriptor
    }

    close(fi);  //closes the file descriptor of the video device and write end of the pipe
    close(write_fd);
}

void obdelaj_sliko(int read_fd, int write_fd) { //takes two file descriptors from which the function reads input and writes outpu
    //reads data from input  file descriptor (read_fd)
    //converts 24 bit ot 16 bit with masking and bit shifting
    // write to the output file descriptor (write_fd) 
    int a;
    int SIZEI = 640*3;
    int SIZEO = 640*2;
    char *bufI, *bufO, R, G, B;     //pointers to character array for storing inputs/outputs

    bufI = (char *) malloc(sizeof(char) * SIZEI);
    bufO = (char *) malloc(sizeof(char) * SIZEO);

    while(1) {
        read(read_fd, bufI, SIZEI);

        for(a = 0; a <= 640; a++) {
            R = bufI[a*3];
            G = bufI[a*3+1];
            B = bufI[a*3+2];
            //correct byte positions are accessed for each component
            
            bufO[a*2+1] = (R & 0xF8) | ((G & 0xE0)>>5); //5 and 3 bits
            bufO[a*2] = ((G & 0x1C)<<3) | ((B & 0xF8)>>3);  //3 and 5 bits
            // red uses the 5 most significant bits
            // green divides bits between two bytes (first byte three mostsignificant bits and then the remaining 5)
            // blue uses 5 most significant bits
        }

        write(write_fd, bufO, SIZEO);
    }

    close(read_fd);
    close(write_fd);
}

void prikazi_sliko(int read_fd) {
    // opens framebuffer device
    //reads from read_fb file descriptor and writes to framebuffer
    // moving the position indicator forward with SCREEN_SIZE to skip padding
    // resets the position to the start of the framebuffer after the end
    int fo;
    int SIZEI = 640*2;  //bytes of the displayed image
    char *buf;
    buf = (char *) malloc(sizeof(char) * SIZEI);
    
    fo = open("/dev/fb0", O_WRONLY);
    
    while(1){
        read(read_fd, buf, SIZEI);
        write(fo, buf, SIZEI);
        lseek(fo,SCREEN_SIZE,SEEK_CUR);
        //moves the file position indicator of the framebuffer by SCREEN_SIZE (one row)
        
        
        if((lseek(fo,0,SEEK_CUR)) >= (SCREEN_SIZE * 2 * 480)){
            lseek(fo,0,SEEK_SET);
            //checks the current position in the framebuffer file
            //If the position exceeds the position fo the frame, resets the position to the start of the file
            //loops over the frame and updates it
        }
    }
}