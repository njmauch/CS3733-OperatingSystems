#define _CRT_SECURE_NO_WARNINGS 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assign02.h"

//gloabl variables 
int clock = 0;
int total_waiting_time = 0;
int total_turnaround_time = 0;
int total_job = 0;
char cpu_scheduling_alg[5] = { '\0' };
int quantum = 0;

int main(int argc, char* argv[]) {
	
	char fileName[11] = { '\0' };
	int i;
	//create new linked list of CPU processes
	PCB_stLL PCB_list = newPCBLinkedList();

	//since order can be different of command arguements loop through argv[] and 
	//determine the filename, algorithm, and quantum based on where it is in the array
	for (i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-input") == 0) {
			strcpy(fileName, argv[i + 1]);
		}
		else if (strcmp(argv[i], "-alg") == 0) {
			strcpy(cpu_scheduling_alg, argv[i + 1]);
		}
		else if (strcmp(argv[i], "-quantum") == 0) {
			quantum = atoi(argv[i + 1]);
		}
		i++;
	}
	//print header
	printf("Student Name: Nathan Mauch\n");
	printf("Input File Name: %s\n", fileName);
	printf("CPU Scheduling Alg: %s\n", cpu_scheduling_alg);

	//open input1.txt file and pass the linked list of CPU processes
	runFile(PCB_list, fileName);

	//ensure that the linked list has been freed from memory
	free(PCB_list->pHead);
	free(PCB_list);
	return 0;
}
//FIFO Algorithm, runs each task as first in first out priority base on the order they are
//in the linked list of CPU processes
void FIFO_Scheduling(PCB_stLL PCB_list) {
	//the current node
	PCB_stNode* pCurrent;
	int CPUReg[8] = { 0 };
	int i;
	//error check if the list is empty
	if (PCB_list->pHead == NULL) {
		fprintf(stderr, "%s", "List is empty\n");
	}
	//set the current node as the head of the linked list of CPU processes
	pCurrent = PCB_list->pHead;
	//loop through all the nodes until NULL value which is the end of the list
	while (pCurrent != NULL) {
		//context switching, adding 1 to each of the registers as if the CPU is doing something
		for (i = 0; i < 8; i++) {
			CPUReg[i] = pCurrent->myReg[i];
			CPUReg[i]++;
			pCurrent->myReg[i] = CPUReg[i];
		}
		//Update the waiting time in the CPU process
		pCurrent->waitingTime = pCurrent->waitingTime + clock - pCurrent->queueEnterClock;
		//Update the total waiting time to execute all the processes
		total_waiting_time = total_waiting_time + pCurrent->waitingTime;
		//Update the over all clock 
		clock = clock + pCurrent->CPUBurst;
		//Update the total turnaround time of the processes
		total_turnaround_time = total_turnaround_time + clock;
		//Update the number of total jobs
		total_job = total_job + 1;
		//Print out the current process that just completed and the clock time it completed at
		printf("Process %d is completed at %d ms\n", pCurrent->ProcId, clock);
		pCurrent = pCurrent->pNext;		
	}
	//Print out the perfomance of the algorithm completing all the CPU processes
	printf("Average Waiting time = %.2f ms\n", (float)total_waiting_time/(float)total_job);
	printf("Average turnaround time = %.2f ms\n", (float)total_turnaround_time/(float)total_job);
	printf("Throughput = %.2f jobs per ms\n", (float)total_job/(float)clock);
}

//Shortest job first algorith, find the CPU process with the smallest cpu burst
//and run that process first
void SJF_Scheduling(PCB_stLL PCB_list) {
	//Node to store the node with the minimum cpu burst
	PCB_stNode* pMinimum;
	int CPUReg[8] = { 0 };
	int i;
	if (PCB_list->pHead == NULL) {
		fprintf(stderr, "%s", "List is empty\n");
	}
	//Get the node with the smallest cpu burst
	pMinimum = getShortestJob(PCB_list);
	
	//loop through the linked list of CPU processes till null value is found
	while (pMinimum != NULL) {
		//context switching, adding 1 to each of the registers as if the CPU is doing something
		for (i = 0; i < 8; i++) {
			CPUReg[i] = pMinimum->myReg[i];
			CPUReg[i]++;
			pMinimum->myReg[i] = CPUReg[i];
		}
		//Update the waiting time in the CPU process
		pMinimum->waitingTime = pMinimum->waitingTime + clock - pMinimum->queueEnterClock;
		//Update the total waiting time to execute all the processes
		total_waiting_time = total_waiting_time + pMinimum->waitingTime;
		//Update the total turnaround time of the processes
		clock = clock + pMinimum->CPUBurst;
		//Update the total turnaround time of the processes
		total_turnaround_time = total_turnaround_time + clock;
		//update the total number of processes ran
		total_job = total_job + 1;

		printf("Process %d is completed at %d ms\n", pMinimum->ProcId, clock);
		//remove that process from the linked list
		deleteNode(PCB_list, pMinimum->ProcId);
		//get the next process with the smallest cpu burst time
		pMinimum = getShortestJob(PCB_list);
	}
	//Print out the perfomance of the algorithm completing all the CPU processes
	printf("Average Waiting time = %.2f ms\n", (float)total_waiting_time / (float)total_job);
	printf("Average turnaround time = %.2f ms\n", (float)total_turnaround_time / (float)total_job);
	printf("Throughput = %.2f jobs per ms\n", (float)total_job / (float)clock);
}
//Priority algorithm will pick the process with the highest priority and run that process first
void PR_Scheduling(PCB_stLL PCB_list) {
	PCB_stNode* pMaxProcPR;
	int CPUReg[8] = { 0 };
	int i;
	if (PCB_list->pHead == NULL) {
		fprintf(stderr, "%s", "List is empty\n");
	}
	//Get the process with max priority
	pMaxProcPR = getMaxProcID(PCB_list);

	while (pMaxProcPR != NULL) {
		//context switching, adding 1 to each of the registers as if the CPU is doing something
		for (i = 0; i < 8; i++) {
			CPUReg[i] = pMaxProcPR->myReg[i];
			CPUReg[i]++;
			pMaxProcPR->myReg[i] = CPUReg[i];
		}
		//Update the waiting time in the CPU process
		pMaxProcPR->waitingTime = pMaxProcPR->waitingTime + clock - pMaxProcPR->queueEnterClock;
		//Update the total waiting time to execute all the processes
		total_waiting_time = total_waiting_time + pMaxProcPR->waitingTime;
		//Update the total turnaround time of the processes
		clock = clock + pMaxProcPR->CPUBurst;
		//Update the total turnaround time of the processes
		total_turnaround_time = total_turnaround_time + clock;
		//Update the number of total jobs
		total_job = total_job + 1;

		printf("Process %d is completed at %d ms\n", pMaxProcPR->ProcId, clock);
		//delete the current process
		deleteNode(PCB_list, pMaxProcPR->ProcId);
		//get the next max priority process
		pMaxProcPR = getMaxProcID(PCB_list);
	}
	//Print out the perfomance of the algorithm completing all the CPU processes
	printf("Average Waiting time = %.2f ms\n", (float)total_waiting_time / (float)total_job);
	printf("Average turnaround time = %.2f ms\n", (float)total_turnaround_time / (float)total_job);
	printf("Throughput = %.2f jobs per ms\n", (float)total_job / (float)clock);
}
//Round robin algorithm uses a quantum timer and runs a cpu process up until that qauntum time
//then will go to the next process whether or not that current process has finished.  If there is
//still more cpu burst on that process it will be ran again after the other processes have finished
void RR_Scheduling(PCB_stLL PCB_list) {
	PCB_stNode* pCurrent;
	int CPUReg[8] = { 0 };
	int i;
	if (PCB_list->pHead == NULL) {
		fprintf(stderr, "%s", "List is empty\n");
	}
	//Get the first process in the linked list
	pCurrent = PCB_list->pHead;

	while (pCurrent != NULL) {
		//context switching, adding 1 to each of the registers as if the CPU is doing something
		for (i = 0; i < 8; i++) {
			CPUReg[i] = pCurrent->myReg[i];
			CPUReg[i]++;
			pCurrent->myReg[i] = CPUReg[i];
		}
		//If the cpu burst is less than or equal to the quantum time run the full process
		if (pCurrent->CPUBurst <= quantum) {
			//Update the waiting time in the CPU process
			pCurrent->waitingTime = pCurrent->waitingTime + clock - pCurrent->queueEnterClock;
			//Update the total waiting time to execute all the processes
			total_waiting_time = total_waiting_time + pCurrent->waitingTime;
			//Update the total turnaround time of the processes
			clock = clock + pCurrent->CPUBurst;
			//Update the total turnaround time of the processes
			total_turnaround_time = total_turnaround_time + clock;
			//Update the number of total jobs
			total_job = total_job + 1;
			printf("Process %d is completed at %d ms\n", pCurrent->ProcId, clock);
			//since the process cpu burst was less than quantun delete that process 
			//from the linked list
			deleteNode(PCB_list, pCurrent->ProcId);
		}
		//If the cpu burst is greater than the quantum run the process up to quantum time
		//then update the burst time with cpu burst - quantum
		else if (pCurrent->CPUBurst > quantum) {
			//Update the waiting time in the CPU process
			pCurrent->waitingTime = pCurrent->waitingTime + clock - pCurrent->queueEnterClock;
			//Update the total turnaround time of the processes
			clock = clock + quantum;
			//Process still has burst time left over after given CPUt for quantum time ms
			//Update the current CPUBurst
			pCurrent->CPUBurst = pCurrent->CPUBurst - quantum;
			//update the queue enter clock time
			pCurrent->queueEnterClock = clock;
			printf("Process %d is completed at %d ms\n", pCurrent->ProcId, clock);
			//Insert the current process at the end of the linked list of cpu processes with 
			//update cpu burst time 
			if (pCurrent->CPUBurst > 0) {
				insertNewPCB_stNode(PCB_list, *pCurrent);
			}
			//delete the current process as it is no longer needed
			deleteNode(PCB_list, pCurrent->ProcId);
		}
		//get the next process from the linked list
		pCurrent = PCB_list->pHead;
	}
	//Print out the perfomance of the algorithm completing all the CPU processes
	printf("Average Waiting time = %.2f ms\n", (float)total_waiting_time / (float)total_job);
	printf("Average turnaround time = %.2f ms\n", (float)total_turnaround_time / (float)total_job);
	printf("Throughput = %.2f jobs per ms\n", (float)total_job / (float)clock);
}

//Initialize new linked list of processes
PCB_stLL newPCBLinkedList() {
	PCB_stLL list = (PCB_stLL)malloc(sizeof(PCB_stImp));
	list->pHead = NULL;
	return list;
}

//runFile actually opens the file with the processes containing process ID, 
//process priority, and cpu burst time, based on command parameters chosing which
//scheduling algorithm to use
void runFile(PCB_stLL PCB_list, char fileName[]) {
	//temp node to store values read from input file
	PCB_stNode pNew;
	FILE* fileRead;
	//open the file
	fileRead = fopen(fileName, "r");
	if (fileRead == NULL) {
		fprintf(stderr, "%s", "Error opening file\n");
		exit(1);
	}
	//read the first line of the file
	fscanf(fileRead, "%d %d %d", &pNew.ProcId, &pNew.ProcPR, &pNew.CPUBurst);
	//insert temp node into the end of the linked list
	insertNewPCB_stNode(PCB_list, pNew);
	//continue reading the file until end of file
	while (!feof(fileRead)) {
		//read the next line of the file
		fscanf(fileRead, "%d %d %d", &pNew.ProcId, &pNew.ProcPR, &pNew.CPUBurst);
		//insert temp node into the end of the linked list
		insertNewPCB_stNode(PCB_list, pNew);
	}
	//Run first in first out algorith
	if (strcmp(cpu_scheduling_alg, "FIFO") == 0) {
		FIFO_Scheduling(PCB_list);
	}
	//Run shortest job first algorithm
	else if (strcmp(cpu_scheduling_alg, "SJF") == 0) {
		SJF_Scheduling(PCB_list);
	}
	//Runn the highest priority first algorithm
	else if (strcmp(cpu_scheduling_alg, "PR") == 0) {
		PR_Scheduling(PCB_list);
	}
	//Run the round robin algorithm
	else if (strcmp(cpu_scheduling_alg, "RR") == 0) {
		RR_Scheduling(PCB_list);
	}
	//close the file
	fclose(fileRead);
}
//Inserts the new process into the linked list
PCB_stNode* insertNewPCB_stNode(PCB_stLL PCB_list, PCB_stNode value) {
	PCB_stNode* pTemp, *pNew;
	//allocate memeory for the new process
	pNew = allocPCB_stNode(PCB_list, value);
	//if the list of processes is empty insert the new process
	//at the beginning of the list
	if (PCB_list->pHead == NULL) {
		PCB_list->pHead = pNew;
		return pNew;
	}
	//if list isn't empty enter the process at the end of the list
	else {
		pTemp = PCB_list->pHead;
		while (pTemp->pNext != NULL) {
			pTemp = pTemp->pNext;
		}
		pTemp->pNext = pNew;
	}
	return pNew;
}

//allocates memory and sets the values of the process
PCB_stNode* allocPCB_stNode(PCB_stLL PCB_list, PCB_stNode value) {
	PCB_stNode* pNew;
	int i;
	//allocating memory for the process
	pNew = (PCB_stNode*)malloc(sizeof(PCB_stNode));
	if (pNew == NULL) {
		fprintf(stderr, "%s", "Error allocating memory for new node\n");
	}
	//setting all the values of the new process
	pNew->CPUBurst = value.CPUBurst;
	pNew->ProcId = value.ProcId;
	pNew->ProcPR = value.ProcPR;
	for (i = 0; i < 8; i++) {
		pNew->myReg[i] = value.ProcId;
	}
	pNew->queueEnterClock = 0;
	pNew->waitingTime = 0;
	pNew->pNext = 0;
	return pNew;
}
//Function to find the process with the smallest cpu burst
//in the list of the processes and returns that process
PCB_stNode* getShortestJob(PCB_stLL PCB_list) {
	PCB_stNode* pMinimum, *pCurrent;
	
	pCurrent = PCB_list->pHead;
	pMinimum = PCB_list->pHead;
	if (PCB_list->pHead == NULL) {
		pMinimum = NULL;
		return pMinimum;
	}
	else {	
		while (pCurrent != NULL) {
			if (pCurrent->CPUBurst < pMinimum->CPUBurst) {
				pMinimum = pCurrent;
			}
			pCurrent = pCurrent->pNext;
		}
	}
	return pMinimum;
}
//Removes process from the linked list
void deleteNode(PCB_stLL PCB_list, int procId) {
	PCB_stNode* pPrev, *pTemp, *pCurrent;

	if (PCB_list->pHead == NULL) {
		fprintf(stderr, "%s", "List is empty\n");
	}

	if (PCB_list->pHead->ProcId == procId) {
		pTemp = PCB_list->pHead;
		PCB_list->pHead = PCB_list->pHead->pNext;
		free(pTemp);
		return;
	}
	pPrev = PCB_list->pHead;
	pCurrent = PCB_list->pHead->pNext;
	while (pCurrent != NULL && pPrev != NULL) {
		if (pCurrent->ProcId == procId) {
			pTemp = pCurrent;
			pPrev->pNext = pCurrent->pNext;
			free(pTemp);
			return;
		}
		pPrev = pCurrent;
		pCurrent = pCurrent->pNext;
	}
	return;
}
//Gets the process with the highest priority from the list
//of processes
PCB_stNode* getMaxProcID(PCB_stLL PCB_list) {
	PCB_stNode* pMaximum, * pCurrent;

	pCurrent = PCB_list->pHead;
	pMaximum = PCB_list->pHead;
	if (PCB_list->pHead == NULL) {
		pMaximum = NULL;
		return pMaximum;
	}
	else {
		while (pCurrent != NULL) {
			if (pCurrent->ProcPR > pMaximum->ProcPR) {
				pMaximum = pCurrent;
			}
			pCurrent = pCurrent->pNext;
		}
	}
	return pMaximum;
}