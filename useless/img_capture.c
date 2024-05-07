#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>



// privzeta velikost bloka za prepis z read/write
// 640 x 480 x 3 byte
#define NPOD 921600  

int main()
{
    char *pom;

    int video; 
    int slika;

    ssize_t n_pod, p_pod;

    n_pod = NPOD;

    /* odpremo datoteko za branje, ime datoteke doloca argv[1] */
    video = open("/dev/video0", O_RDONLY);
    if (video == -1) { /* O_RDONLY je seveda 0 */
        printf("Napaka open /dev/video0\n");
        exit(2);
    }

    /* odpremo(ustvarimo) datoteko za pisanje, ime doloca argv[2] */
    /* biti dovoljenj: rwxrwxrwx lastnik(rwx) skupina(rwx) vsi(rwx) */
    /* 1 = dovoli, 0 = prepove, r = read, w = write, x = execute */
    /* 0644(osmisko) = 110 100 100 (dvojisko) = rw-r--r--(dovoljenja)*/
    /* rw za lastnika, r za skupino, r za vse uporabnike */

    slika = open("vhod.raw", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (slika == -1){
        printf("Napaka open 'vhod.raw'\n");
        exit(3);
    }

    /* Zagotovimo si pomnilnik za podatke, najvec Npodatkov */
    if ((pom = malloc( n_pod )) == NULL){
        printf("Napaka malloc\n");
        exit(4);
    }

    /* prepisujemo podatke do konca datoteke */
    printf("Velikost bloka %ld bajtov\n", n_pod);
    while ((p_pod = read(video, pom, n_pod)) != 0){
        if (p_pod == -1){
            printf("Napaka read\n");
            exit(5);
        }

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
