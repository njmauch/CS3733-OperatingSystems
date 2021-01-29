#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

int main(int argc, char *argv[]) {
    int PT[32]={2, 4, 1, 7, 3, 5, 6, 0};
    FILE *fileIN, *fileOUT;
    unsigned int p=5, f=3, d=7;
    unsigned int pnum, fnum, dnum;
    unsigned long PA, LA;
    int count = 0;

    if(argc != 3) {
        printf("usage: assing03part1 infile outfile\n");
        exit(1);
        }
    fileIN = fopen(argv[1], "r");
    if(fileIN == NULL){
      fprintf(stderr, "%s", "Error opening sequence file\n");
      exit(1);
    }
    fileOUT = fopen(argv[2], "a+");
    if(fileOUT == NULL) {
      fprintf(stderr, "%s", "Error opening output file\n");
      exit(1);
    }
    while(!feof(fileIN)) {
      if(fread(&LA, sizeof(unsigned long), 1, fileIN) != 1) {
        break;
      }
      count++;
      dnum = LA & 0x7F;
      pnum = LA >> d;
      fnum = PT[pnum];
      PA = (fnum << d) + dnum;
      printf("The LA is %-4lx and translated PA is %-4lx\n", LA, PA);
      if (fwrite(&PA, sizeof(unsigned long), 1, fileOUT) != 1) {
        fprintf(stderr, "%s", "Error writing to output file\n");
        break;
      }
    }
    printf("Total number of pages: %d\n", count);
    fclose(fileIN);
    fclose(fileOUT);

    return 0;
}
