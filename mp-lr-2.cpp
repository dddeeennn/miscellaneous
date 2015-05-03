// mp-lr-2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "mpi.h"
#include <iostream>
#include <string>

int *createMatrix (int nrows, int ncols) {
	int *matrix;
	int h, i, j;

	if (( matrix = (int*)malloc(nrows*ncols*sizeof(int))) == NULL) {
		printf("Malloc error");
		exit(1);
	}

	for (h=0; h<nrows*ncols; h++) {
		matrix[h] = 0;
	}

	return matrix;
}

void printArray (int *row, int nElements) {
	int i;
	for (i=0; i<nElements; i++) {
		printf("%d ", row[i]);
	}
	printf("\n");
}

int main(int argc, char* argv[])
{
	printf("Init MPI!\n");
	MPI_Init(&argc,&argv);

	int processId, concurrentProcessCount;
	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
	MPI_Comm_size(MPI_COMM_WORLD, &concurrentProcessCount);
	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);

	int *Amatrix = (int*)malloc(concurrentProcessCount*concurrentProcessCount*sizeof(int));
	int *Bmatrix = (int*)malloc(concurrentProcessCount*concurrentProcessCount*sizeof(int));
	int *Cmatrix = (int*)malloc(concurrentProcessCount*concurrentProcessCount*sizeof(int));
	Cmatrix = createMatrix(concurrentProcessCount, concurrentProcessCount);

	if (processId == 0) {
		Amatrix = createMatrix(concurrentProcessCount, concurrentProcessCount); 
		Amatrix[0] = 1;
		Amatrix[1] = 2;
		Amatrix[2] = 3;
		Amatrix[3] = 4;
		printf("Initial Amatrix:\n");
		printArray(Amatrix, concurrentProcessCount*concurrentProcessCount);

		Bmatrix = createMatrix(concurrentProcessCount, concurrentProcessCount); // Master process creates matrix
		Bmatrix[0] = 5;
		Bmatrix[1] = 6;
		Bmatrix[2] = 7;
		Bmatrix[3] = 8;
		//printf("Bmatrix:\n");
		//printArray(Bmatrix, concurrentProcessCount*concurrentProcessCount);
	}

	int *procARow = (int*) malloc(sizeof(int) * concurrentProcessCount); // received row will contain p integers
	int *procCRow = (int*) malloc(sizeof(int) * concurrentProcessCount); 

	MPI_Scatter(Amatrix, concurrentProcessCount, MPI_INT, // send one row, which contains p integers
		procARow, concurrentProcessCount, MPI_INT, // receive one row, which contains p integers
		0, MPI_COMM_WORLD);

	printf("Process %d received elements: ", processId);
	printArray(procARow, concurrentProcessCount);

	MPI_Bcast(Bmatrix, 4, MPI_INT, 0,MPI_COMM_WORLD);
	printf("Bmatrix:\n");
	printArray(Bmatrix, concurrentProcessCount*concurrentProcessCount);

	for(int i = 0;i<concurrentProcessCount;i++){
		procCRow[i]=0;
		for(int k = 0; k < concurrentProcessCount;k++){
			procCRow[i]+=procARow[k]*Bmatrix[k*concurrentProcessCount+i];
		}
	}
	printf("CRowMatrix:\n");
	printArray(procCRow, 2);

	MPI_Gather(procCRow, 2, MPI_INT, Cmatrix, 2, MPI_INT, 0, MPI_COMM_WORLD);

	if(processId==0){
		printf("CMatrix:\n");
		printArray(Cmatrix, 4);
	}
	printf("MPI finalize... \n");
	printf("\n");

	MPI_Finalize();

	return 0;
}

//lr-1
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//	
//	printf("Get Comm size\n");
//	int size=0;
//	MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//	printf("Get Comm rank\n");
//	int rank=0; MPI_Comm_rank(MPI_COMM_WORLD,&rank);
//
//	printf("I am %d process from %d processes!\n",rank,size);
//	
//	printf("MPI finalize... \n");
//	MPI_Finalize();
//
//	return 0;
//}

//lr-2
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	int message = 45;
//	int receivedMessage=0;
//	
//	int processId;
//	MPI_Comm_rank(MPI_COMM_WORLD,&processId);
//	printf("process id = %d ...\n",processId);
//
//	printf("before send message %d \n", receivedMessage);
//
//	if(processId==0){
//		// process with id=0 send message '45'
//		MPI_Send(&message, 1,  MPI_INT,  1, 0,  MPI_COMM_WORLD);
//		printf("process with id =0 send %d ... \n",message);
//	}
//
//	if(processId==1) {
//		//process with id=1 receive message
//		MPI_Recv(&receivedMessage,1,MPI_INT,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//
//		printf("receive message %d \n",receivedMessage);
//	}
//	
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}

//lr-3
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	int processId;
//	int concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD,&processId);
//	MPI_Comm_size(MPI_COMM_WORLD,&concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	int message;
//
//	if(processId ==0){
//		printf("process with id=%d send message...\n", processId);
//		message=0;
//		MPI_Send(&message, 1,  MPI_INT,  1, 0,  MPI_COMM_WORLD);
//		printf("waiting for process #0 receive message...\n");
//		MPI_Recv(&message, 1, MPI_INT,concurrentProcessCount-1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//		printf("receive message %d \n", message);
//	}else{
//		printf("process #%d waiting for message from previous process #%d...\n",processId,processId-1);
//		MPI_Recv(&message, 1, MPI_INT,processId-1, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//		printf("process #%d receive message: %d  from process #%d\n",processId,message,processId-1);
//		message+=1;
//
//		if(processId==concurrentProcessCount-1)
//		{
//			printf("process #%d send message process #0 \n",processId);
//			MPI_Send(&message, 1,  MPI_INT, 0, 0,  MPI_COMM_WORLD); 
//		}
//		else
//		{
//			printf("process #%d send message process #%d \n",processId,processId+1);
//			MPI_Send(&message, 1,  MPI_INT,  processId+1, 0,  MPI_COMM_WORLD);
//		}
//	}
//
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}

//lr-4
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	int processId;
//	int concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD,&processId);
//	MPI_Comm_size(MPI_COMM_WORLD,&concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	int message;
//
//	if(processId ==0){
//		printf("master process prepare receive message... \n");
//		for(int i = 1; i < concurrentProcessCount; i++)
//		{
//			MPI_Recv(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//			printf("master process receive message %d \n",message);
//		}
//	}
//	else
//	{
//		printf("slave process #%d send message %d \n", processId, processId);
//		message=processId;
//		MPI_Send(&message, 1,  MPI_INT, 0, 0,  MPI_COMM_WORLD); 
//	}
//
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}

//lr-5
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	MPI_Request request;
//	MPI_Status status;
//
//	int processId;
//	int concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD,&processId);
//	MPI_Comm_size(MPI_COMM_WORLD,&concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	int message;
//
//	if(processId ==0){
//		message=999;
//		printf("process #%d set message %d \n",processId,message);
//		MPI_Isend(&message,1,MPI_INT,1,0,MPI_COMM_WORLD, &request);
//		printf("process #%d isend message %d to process #1... \n",processId,message);
//	}
//
//	if(processId==1)
//	{
//		printf("process #%d wait for irecv message from process #0 ...\n", processId);
//		MPI_Irecv(&message,1,MPI_INT,0,0,MPI_COMM_WORLD, &request);
//
//		MPI_Wait(&request, &status);
//		printf("process #1 receive message %d \n",message);
//
//	}
//
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}

//lr-6
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	MPI_Request requests[2];
//	MPI_Status statuses[2];
//
//	int processId;
//	int concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD,&processId);
//	MPI_Comm_size(MPI_COMM_WORLD,&concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	int message = processId;
//	
//	if(processId==0)
//	{
//		printf("first process #0 send to process #1 \n");
//		MPI_Isend(&message,1,MPI_INT,1,0,MPI_COMM_WORLD, &requests[0]);
//		MPI_Irecv(&message, 1, MPI_INT , concurrentProcessCount-1, 0, MPI_COMM_WORLD, &requests[1]);
//
//	}
//	else if(processId!=concurrentProcessCount-1)
//	{
//		printf("process #%d isend message %d  to process #%d \n",processId, message, processId+1);
//		MPI_Isend(&message,1,MPI_INT,processId+1,0,MPI_COMM_WORLD, &requests[0]);
//		MPI_Irecv(&message,1,MPI_INT,processId-1,0,MPI_COMM_WORLD, &requests[1]);
//	}
//	else
//	{
//		printf("last process #%d send to process #0 \n", processId);
//		MPI_Isend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &requests[0]);
//		MPI_Irecv(&message, 1, MPI_INT,processId-1, 0, MPI_COMM_WORLD, &requests[1]);
//	}
//	
//	MPI_Waitall(2, requests, statuses);
//	printf("process #%d receive message %d \n",processId, message);
//	
//
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}

//lr-7
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	MPI_Request requests[2];
//	MPI_Status statuses[2];
//
//	MPI_Request sendRequests[2];
//	MPI_Status sendStatuses[2];
//		
//	MPI_Request recvRequests[2];
//	MPI_Status recvStatuses[2];
//
//	int requestIdx=0;
//	int processId;
//	int concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
//	MPI_Comm_size(MPI_COMM_WORLD, &concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	int message = processId;
//
//	for(int i=0; i<concurrentProcessCount; i++)
//	{
//		if(i!=processId)
//		{
//			printf("process #%d isend message %d  to process #%d \n", processId, message, i);
//			MPI_Isend(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &sendRequests[requestIdx]);
//			requestIdx++;
//		}
//	}
//
//	MPI_Waitall(2, sendRequests, sendStatuses);
//
//	requestIdx=0;
//
//	for(int i=0; i<concurrentProcessCount; i++)
//	{
//		if(i!=processId)
//		{
//			// printf("process #%d wait for recv message from process #%d \n", processId, i);
//			MPI_Irecv(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &recvRequests[requestIdx]);
//			MPI_Wait(&recvRequests[requestIdx], &recvStatuses[requestIdx]);
//			printf("process #%d receive message %d from process #%d \n", processId, message, i);
//			requestIdx++;
//		}
//	}
//
//	/*MPI_Waitall(2, recvRequests, recvStatuses);
//	printf("process #%d receive message %d \n", processId, message);*/
//
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}
//

//lr-8
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//
//	MPI_Init(&argc,&argv);
//
//	int processId;
//	int concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
//	MPI_Comm_size(MPI_COMM_WORLD, &concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	std::string inputStr;
//	char message[100];
//	int inputStrLength=0;
//
//	if(processId==0)
//	{
//		inputStr="aaaaaabbbbcccccccccccc";
//		inputStrLength = inputStr.length();
//		strncpy_s(message, inputStr.c_str(), sizeof(message));	
//	}
//	
//	MPI_Bcast (message, 100, MPI_CHAR, 0,MPI_COMM_WORLD);
//	MPI_Bcast (&inputStrLength, 1, MPI_INT, 0,MPI_COMM_WORLD);
//	printf("broadcast have been sended message %s... \n",inputStr.c_str(), inputStrLength);
//	printf("input string: %s , length  %d \n", inputStr.c_str(), inputStrLength);
//
//	int count = 0;
//	char a = 'a';
//	for (int i=processId; i<26; i=i+concurrentProcessCount) {
//		int count=0;
//		for(int j=0;j<inputStrLength;j++)
//		{
//			if(((char)((int)a + i))==message[j]) count++;
//		}
//
//		if(count>0)
//		{
//			std::string s;
//			printf("%s = %d \n", std::string(1,(char)(a+i)).c_str(), count);
//		}
//	}
//
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}

//lr-9
//int main(int argc, char* argv[])
//{
//		printf("Init MPI!\n");

//MPI_Init(&argc,&argv);

//int processId;
//int concurrentProcessCount;
//MPI_Comm_rank(MPI_COMM_WORLD, &processId);
//MPI_Comm_size(MPI_COMM_WORLD, &concurrentProcessCount);
//printf("I am %d of %d processes .\n", processId, concurrentProcessCount);

//long int iteration = 1e10;
//long int iterationByProcess=iteration/concurrentProcessCount;

//double x,
//	sum=0,
//	sumByProcess=0;

//for (int i=0; i<iterationByProcess; i++) {
//	x = (i+0.5)/iterationByProcess;
//	sumByProcess += 4.0/(1.0 + x*x);
//}

//MPI_Reduce(&sumByProcess,&sum,1,MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);

//if (processId == 0) printf("PI =3.%d \n", sum/iteration);
//

//printf("MPI finalize... \n");
//printf("\n");

//MPI_Finalize();

//return 0;
//}


//lr-10
//printf("Init MPI!\n");
//MPI_Init(&argc,&argv);

//int message=-1;

//MPI_Group oldGroup, group;
//MPI_Comm newComm;
//int processIds[3] = {0, 2, 4};

//int processId;
//int concurrentProcessCount;
//MPI_Comm_rank(MPI_COMM_WORLD, &processId);
//MPI_Comm_size(MPI_COMM_WORLD, &concurrentProcessCount);
////printf("I am %d of %d processes .\n", processId, concurrentProcessCount);

//MPI_Comm_group(MPI_COMM_WORLD,&oldGroup);
//MPI_Group_incl (oldGroup, 3, processIds, &group);
//MPI_Comm_create (MPI_COMM_WORLD, group, &newComm);

//int processIdNewGroup=-1;
//int concurrentProcessCountNewGroup=-1;

//if(newComm != MPI_COMM_NULL)
//{
//	MPI_Comm_rank(newComm, &processIdNewGroup);
//	MPI_Comm_size(newComm, &concurrentProcessCountNewGroup);

//	if(processIdNewGroup==0)message=777;
//	MPI_Bcast (&message, 1, MPI_INT, 0, newComm);
//}

//printf("MPI_COMM_WORLD: %d from %d. New comm: %d from %d. Message = %d \n",
//	processId,concurrentProcessCount,processIdNewGroup,concurrentProcessCountNewGroup,message);

//printf("MPI finalize... \n");
//printf("\n");

//MPI_Finalize();

//return 0;

//int *createMatrix (int nrows, int ncols) {
//	int *matrix;
//	int h, i, j;
//
//	if (( matrix = (int*)malloc(nrows*ncols*sizeof(int))) == NULL) {
//		printf("Malloc error");
//		exit(1);
//	}
//
//	for (h=0; h<nrows*ncols; h++) {
//		matrix[h] = 0;
//	}
//
//	return matrix;
//}
//
//void printArray (int *row, int nElements) {
//	int i;
//	for (i=0; i<nElements; i++) {
//		printf("%d ", row[i]);
//	}
//	printf("\n");
//}
//
//int main(int argc, char* argv[])
//{
//	printf("Init MPI!\n");
//	MPI_Init(&argc,&argv);
//
//	int processId, concurrentProcessCount;
//	MPI_Comm_rank(MPI_COMM_WORLD, &processId);
//	MPI_Comm_size(MPI_COMM_WORLD, &concurrentProcessCount);
//	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);
//
//	int *Amatrix = (int*)malloc(concurrentProcessCount*concurrentProcessCount*sizeof(int));
//	int *Bmatrix = (int*)malloc(concurrentProcessCount*concurrentProcessCount*sizeof(int));
//	int *Cmatrix = (int*)malloc(concurrentProcessCount*concurrentProcessCount*sizeof(int));
//	Cmatrix = createMatrix(concurrentProcessCount, concurrentProcessCount);
//
//	if (processId == 0) {
//		Amatrix = createMatrix(concurrentProcessCount, concurrentProcessCount); 
//		Amatrix[0] = 1;
//		Amatrix[1] = 2;
//		Amatrix[2] = 3;
//		Amatrix[3] = 4;
//		printf("Initial Amatrix:\n");
//		printArray(Amatrix, concurrentProcessCount*concurrentProcessCount);
//
//		Bmatrix = createMatrix(concurrentProcessCount, concurrentProcessCount); // Master process creates matrix
//		Bmatrix[0] = 5;
//		Bmatrix[1] = 6;
//		Bmatrix[2] = 7;
//		Bmatrix[3] = 8;
//		//printf("Bmatrix:\n");
//		//printArray(Bmatrix, concurrentProcessCount*concurrentProcessCount);
//	}
//
//	int *procARow = (int*) malloc(sizeof(int) * concurrentProcessCount); // received row will contain p integers
//	int *procCRow = (int*) malloc(sizeof(int) * concurrentProcessCount); 
//
//	MPI_Scatter(Amatrix, concurrentProcessCount, MPI_INT, // send one row, which contains p integers
//		procARow, concurrentProcessCount, MPI_INT, // receive one row, which contains p integers
//		0, MPI_COMM_WORLD);
//
//	printf("Process %d received elements: ", processId);
//	printArray(procARow, concurrentProcessCount);
//
//	MPI_Bcast(Bmatrix, 4, MPI_INT, 0,MPI_COMM_WORLD);
//	printf("Bmatrix:\n");
//	printArray(Bmatrix, concurrentProcessCount*concurrentProcessCount);
//
//	for(int i = 0;i<concurrentProcessCount;i++){
//		procCRow[i]=0;
//		for(int k = 0; k < concurrentProcessCount;k++){
//			procCRow[i]+=procARow[k]*Bmatrix[k*concurrentProcessCount+i];
//		}
//	}
//	printf("CRowMatrix:\n");
//	printArray(procCRow, 2);
//
//	MPI_Gather(procCRow, 2, MPI_INT, Cmatrix, 2, MPI_INT, 0, MPI_COMM_WORLD);
//
//	if(processId==0){
//		printf("CMatrix:\n");
//		printArray(Cmatrix, 4);
//	}
//	printf("MPI finalize... \n");
//	printf("\n");
//
//	MPI_Finalize();
//
//	return 0;
//}
