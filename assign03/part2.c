#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/stat.h>
#include<sys/types.h>

int find_empty_frame(int *freeFrames);
int select_victim_lru(int *lruCount);

struct Page_table {
    int f_num;
    int vi;
};
typedef struct Page_table Page_table;

int main(int argc, char* argv[]) {
    FILE* fileIN, * fileOUT;
    struct Page_table PT[32];
    unsigned int p = 5, f = 3, d = 7;
    unsigned int pnum, fnum, dnum;
    unsigned long PA, LA;
    int CLK = 0;
    int freeFrames[8] = {0,1,1,1,1,1,1,1};
    int LRUcount[8] = { 0 };
    int revmap[8] = { -1 };
    int i, emptyFrame, lruFrame;
    int count = 0;

    for (i = 0; i < 32; i++) {
        PT[i].vi = 0;
    }

    if (argc != 3) {
        printf("usage: assing03part1 infile outfile\n");
        exit(1);
    }
    fileIN = fopen(argv[1], "r");
    if (fileIN == NULL) {
        fprintf(stderr, "%s", "Error opening sequence file\n");
        exit(1);
    }
    fileOUT = fopen(argv[2], "a+");
    if (fileOUT == NULL) {
        fprintf(stderr, "%s", "Error opening output file\n");
        exit(1);
    }
    while (!feof(fileIN)) {
        if (fread(&LA, sizeof(unsigned long), 1, fileIN) != 1) {
            break;
        }
        CLK++;      
        dnum = LA & 0x7F;
        pnum = LA >> d;
        if (PT[pnum].vi == 1) {
            fnum = PT[pnum].f_num;
            PA = (fnum << d) + dnum;
            if (fwrite(&PA, sizeof(unsigned long), 1, fileOUT) != 1) {
                fprintf(stderr, "%s", "Error writing to output file\n");
                break;
            }
            LRUcount[fnum] = CLK;
        }
        else {
            count++;
            emptyFrame = find_empty_frame(freeFrames);
            if (emptyFrame > 0) {
                PT[pnum].f_num = emptyFrame;
                PT[pnum].vi = 1;
                fnum = PT[pnum].f_num;
                PA = (fnum << d) + dnum;
                if (fwrite(&PA, sizeof(unsigned long), 1, fileOUT) != 1) {
                    fprintf(stderr, "%s", "Error writing to output file\n");
                    break;
                }
                freeFrames[emptyFrame] = 0;
                revmap[emptyFrame] = pnum;
                LRUcount[fnum] = CLK;
            }
            else {
                //count++;
                lruFrame = select_victim_lru(LRUcount);
                PT[revmap[lruFrame]].vi = 0;
                PT[pnum].f_num = lruFrame;
                PT[pnum].vi = 1;
                fnum = PT[pnum].f_num;
                PA = (fnum << d) + dnum;
                if (fwrite(&PA, sizeof(unsigned long), 1, fileOUT) != 1) {
                    fprintf(stderr, "%s", "Error writing to output file\n");
                    break;
                }
                LRUcount[fnum] = CLK;
                revmap[fnum] = pnum;
            }
        }
        //printf("The LA is %-4lx and translated PA is %-4lx\n", LA, PA);
    }
    printf("Part 2 page faults: %d\n", count);
}

int find_empty_frame(int *freeFrames)
{
    int index = -1;
    int i;

    for (i = 1; i < 8; i++) {
        if (freeFrames[i] == 1) {
            index = i;
            return index;
        }
    }
    return index;
}
int select_victim_lru(int *lruCount)
{
    int i;
    int min_index = 1;

    for (i = 2; i < 8; i++) {
        if (lruCount[i] < lruCount[min_index]) {
            min_index = i;
        }
    }
    return min_index;
}