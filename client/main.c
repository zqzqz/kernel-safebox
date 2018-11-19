#include <unistd.h>  
#include <sys/types.h>  
#include <sys/stat.h>  
#include <stdio.h>  
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#define TEXTSIZE 4069

int main () {
    char buf[TEXTSIZE];
    FILE *rd = fopen("../box/tmp", "r");
    if (rd == NULL) {
        printf("error %s", "open file failed");
        return 1;
    }
    int pid = getpid();
    printf("pid %d\n", pid);
    write(STDOUT_FILENO, "1234567812345678", 16);
    int stat;
    stat = fread(&buf, TEXTSIZE, 1, rd);
    printf("text %s\n", buf);

    fclose(rd);
    return 0;
}