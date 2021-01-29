#include <stdio.h>
#include <stdlib.h>
#include<string.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
#include<assert.h>
#include<errno.h>
#include<signal.h>
#include<time.h>

typedef struct PCB {
    int PID, PR;
    int numCPUBurst, numIOBurst;
    int* CPUBurst, * IOBurst;
    int cpuindex, ioindex;
    struct timespec ts_begin, ts_end;
    struct PCB* prev, * next;
} PCB;

PCB* Ready_Q, * IO_Q;
int file_read_done;
int cpu_sch_done;
int io_sys_done;
int cpu_busy;
int io_busy;
int quantum;
char* input, *algorithm;
int currPID;
sem_t sem_cpu, sem_io;
struct timespec atimespec;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* FileRead_Thread(void* args) {
    FILE* file;
    char buffer[64];
    char* token, * temp;
    int num, sleep;
    int i;
    int j = 0, k = 0;
    

    file = fopen(input, "r");

    if (file == NULL) {
        fprintf(stderr, "%s", "Failed to open file\n");
        exit(1);
    }

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        temp = buffer;
        token = strtok_r(temp, " ", &temp);
        if (strcmp(token, "proc") == 0) {
            PCB temp;
            temp.PID = ++currPID;
            clock_gettime(CLOCK_MONOTONIC, &temp.ts_begin);
            temp.PR = atoi(strtok_r(temp, " ", &temp));
            num = atoi(strtok_r(temp, " ", &temp));
            temp.numCPUBurst = ((num + 1) / 2);
            temp.numIOBurst = ((num) / 2);
            temp.CPUBurst = malloc(sizeof(int) * temp.numCPUBurst);
            temp.IOBurst = malloc(sizeof(int) * temp.numIOBurst);
            for (i = 0; i < num; i++) {
                if (i % 2 == 0) {
                    temp.CPUBurst[j++] = atoi(strtok_r(temp, " ", &temp));
                }
                else {
                    temp.IOBurst[k++] = atoi(strtok_r(temp, " ", &temp));
                }
            }
            temp.cpuindex = 0;
            temp.ioindex = 0;
            insert_ready_q(&temp);
            sem_post(&sem_cpu);
        }
        if (strcmp(token, "sleep") == 0) {
            sleep = atoi(strtok_r(temp, " ", &temp));
            usleep(sleep * 1000);
        }
        if (strcmp(token, "stop") == 0) {
            break;
        }
    }
    file_read_done = 1;
}

void insert_ready_q(PCB* new) {

}

void* CPU_Scheduler_Thread(void* args) {
    int res;

    while (1) {
        if (Ready_Q == NULL && !cpu_busy && IO_Q == NULL && !io_busy && file_read_done == 1) {
            break;
        }
        res = sem_timedwait(&sem_cpu, &atimespec);
        if (res == -1 && errno == ETIMEDOUT) continue;
        cpu_busy = 1;
        pthread_mutex_lock(&lock);
        Total_job += 1;
        scheduler();
        pthread_mutex_unlock(&lock);
        cpu_busy = 0;
        sem_post(&sem_io);
    }
    cpu_sch_done = 1;
}

void* IO_Sys_Thread(void* args) {
    int res;
    while (1) {
        if (Ready_Q == NULL && !cpu_busy && IO_Q == NULL && file_read_done == 1) {
            break;
        }
        res = sem_timedwait(&sem_io, &atimespec);
        if (res == -1 && errno == ETIMEDOUT) continue;
        pthread_mutex_lock(&lock);
        io_busy = 1;
        Total_job += 1;
        ioscheduler();
        io_busy = 0;
        pthread_mutex_unlock(&lock);
        sem_post(&sem_cpu);
        //printf("I/O      This after read thread fell asleep\n");
    }
    io_sys_done = 1;
}

int main(int argc, char* argv[]) {
	double duration;
    int i;
    clock_t start_t, end_t;

    for (i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-input") == 0)
            input = argv[i + 1];
        else if (strcmp(argv[i], "-alg") == 0)
            algorithm = argv[i + 1];
        else if (strcmp(argv[i], "-quantum") == 0) {
            quantum = atoi(argv[i + 1]);
        }
        else
            continue;
    }  

    file_read_done = 0;
    cpu_sch_done = 0;
    io_sys_done = 0;
    cpu_busy = 0;
    io_busy = 0;
    currPID = 0;

    pthread_t file_read, cpu_sch, io_sys;

    pthread_create(&cpu_sch, NULL, CPU_Scheduler_Thread, NULL);
    pthread_create(&file_read, NULL, FileRead_Thread, NULL);
    pthread_create(&io_sys, NULL, IO_Sys_Thread, NULL);
}