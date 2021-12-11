#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>
#include <stdio.h>

int textmode(int);
int randline();
int getch();
static char pattern[80];
int textmode(int mode) {
    static struct termios con[2];
    if(mode > 0)
        return(tcsetattr(0, TCSAFLUSH, &con[1]));
    tcgetattr(0, &con[0]);
    tcgetattr(0, &con[1]);
    con[0].c_lflag &= ~(ICANON | ECHO | ISIG);
    con[0].c_iflag &= ~(ISTRIP | IXOFF | IXANY | IXON);
    con[0].c_oflag |= CS8;
    con[0].c_cc[VMIN] = 2;
    con[0].c_cc[VTIME] = 1;
    return(tcsetattr(0, TCSAFLUSH, &con[0]));
}

int getch() {
    unsigned char c[2];
    static int len = 0;
    c[0] = c[1] = 0;
    if((len = read(0, c, 2)) < 2)
        return(c[0]);
    if(c[0] == 27) //ESC
        c[0] = 0;
    ungetc(c[1], stdin);
    return(c[0]);
}
int randline(FILE *fp, unsigned int max_rows) {
    unsigned int i = 0;
    char buff[80];
    int r;
    char c;
    int k;
    int status = 0;
    srand(getpid());
    do {
            k = 0;
            r = rand() % max_rows;
            fseek(fp, 0, SEEK_SET);
            while (i < r) {
                c = fgetc(fp);
                if (c == '\n') {
                    i++;
                }
            }
            fgets(buff, 80, fp);
            for (int j = 0; j < 80; ++j) {
                if (buff[j] == '\0'){
                    pattern[k] = '\0';
                    j = 80;
                }
                else
                if (buff[j] > 32)
                    pattern[k++] = buff[j];
                }
            if (pattern[0] == '\0')
                status = 0;
            else
                status = 1;
        }while (status == 0);
        return(0);
}
int main(int argc, char* argv[]) {
    if (argc != 2) {
        write(1, "Usage: file name\n", 17);
        return -1;
    }
    int len = 0;
    unsigned int rows = 0;
    FILE *f_input;
    int i = 0;
    int err = 0;
    char c;

    f_input = fopen(argv[1], "r");

    if (f_input == NULL) {
        write(1, "couldn\'t open file\n", 19);
    }
    while ((c = fgetc(f_input)) != EOF) {
        if (c == '\n') {
        rows++;
        }
    }

    while(1){
        randline(f_input, rows);
        rows--;
        len = strlen(pattern);
        write(1, pattern, len);
        write(1, "\n", 1);
        textmode(0);
        do {
            if (err) {
            write(1, "There are some mistakes!\n", 24);
            }
            err = 0;
            i = 0;
        while(i < len) {
            switch(c = getch()) {
            case 0: c = '\007';
            // exit(c);
            switch(getch()) {
                case 67: c = pattern[i];
                break;
                case 68: if(i == 0)
                break;
                i--;
                write(1, "\b", 1);
                continue;
                default: break;
            }
            break;
            case 27: write(1, "\n", 1);
                i = len;
                c = '\007';
                err = -1;
                textmode(1);
                exit(c);
                break;
            default: if(c != pattern[i])
                        c = '\007';
             break;
        }
        (c == '\007') ? : i++;
        write(1, &c, 1);
        }
        write(1, "\n", 1);
    } while (err);
    }
    textmode(1);
    return(err);
}