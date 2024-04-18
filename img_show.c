#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main() {

 

    int raw_slika = open("izhod.raw", O_RDONLY);
    int show = open("/dev/fb0", O_WRONLY);

   //off_t lseekm_stat =  lseek(int fd, off_t offset, int whence);
    off_t zacentni_odmik = lseek(show, 0, SEEK_CUR);
    char buffer[142000];
    int slika = read(raw_slika, buffer, sizeof(buffer));

    if (slika > 0){
        write(show, buffer, slika);
    }


    off_t koncna_pozicija = lseek(show, 0, SEEK_CUR);

    off_t pozicija_slike = koncna_pozicija - zacentni_odmik;

    lseek(show, pozicija_slike, SEEK_SET);


    close(raw_slika);
    close(show);

    return 0;
}