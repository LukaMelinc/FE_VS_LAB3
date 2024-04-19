#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



#define WIDTH 640
#define HEIGTH 480
#define FB_DEVICE "/dev/fb0"


// privzeta velikost bloka za prepis z read/write
#define NPOD 2228800
// #define NPOD 4096

int main()
{
    char *pom;

    int video; 
    int slika;

    ssize_t n_pod, p_pod;

    n_pod = NPOD;

    /* odpremo datoteko za branje, ime datoteke doloca argv[1] */
    video = open("izhod.raw", O_RDONLY);
    if (video == -1) { /* O_RDONLY je seveda 0 */
        printf("Napaka open /dev/video0\n");
        exit(2);
    }

    slika = open("/dev/fb0", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (slika == -1){
        printf("Napaka open /dev/fb0\n");
        exit(3);
    }

    /* Zagotovimo si pomnilnik za podatke, najvec Npodatkov */
    if ((pom = malloc( n_pod )) == NULL){
        printf("Napaka malloc\n");
        exit(4);
    }

    /* prepisujemo podatke do konca datoteke */
    printf("Velikost bloka %ld bajtov\n", n_pod);
    // while ((p_pod = read(video, pom, n_pod)) > 0){
    while(1){
        p_pod = read(video, pom, n_pod);
        if(p_pod == -1){
            printf("Napaka read\n");
            exit(5);
        }

        lseek(video, 0, SEEK_SET);
        lseek(slika, 0, SEEK_SET);

        if( write(slika, pom, p_pod) != p_pod){
            printf("Napaka write\n");
            exit(6);
        }
    }

    if ((close(slika) == -1) || (close(video) == -1)){
        printf("Napaka close\n");
        exit(7);
    }

    exit(0);
} 