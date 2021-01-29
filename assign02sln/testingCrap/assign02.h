typedef struct PCB_stNode {
	int ProcId;
	int ProcPR;
	int CPUBurst;
	int myReg[8];
	int queueEnterClock;
	int waitingTime;
	struct PCB_stNode* pNext;
} PCB_stNode;

typedef struct {
	PCB_stNode* pHead;
}PCB_stImp;

typedef PCB_stImp* PCB_stLL;

void FIFO_Scheduling(PCB_stLL PCB_list);
void SJF_Scheduling(PCB_stLL PCB_list);
void PR_Scheduling(PCB_stLL PCB_list);
void RR_Scheduling(PCB_stLL PCB_list);
PCB_stLL newPCBLinkedList();
void runFile(PCB_stLL PCB_list, char fileName[]);
PCB_stNode* insertNewPCB_stNode(PCB_stLL PCB_list, PCB_stNode value);
PCB_stNode* allocPCB_stNode(PCB_stLL PCB_list, PCB_stNode value);
PCB_stNode* getShortestJob(PCB_stLL PCB_list);
void deleteNode(PCB_stLL PCB_list, int procId);
PCB_stNode* getMaxProcID(PCB_stLL PCB_list);