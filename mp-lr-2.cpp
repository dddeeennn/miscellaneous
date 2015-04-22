// mp-lr-2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "stdio.h"
#include "mpi.h"


int main(int argc, char* argv[])
{
	printf("Init MPI!\n");

	MPI_Init(&argc,&argv);

	MPI_Request requests[2];
	MPI_Status statuses[2];

	int processId;
	int concurrentProcessCount;
	MPI_Comm_rank(MPI_COMM_WORLD,&processId);
	MPI_Comm_size(MPI_COMM_WORLD,&concurrentProcessCount);
	printf("I am %d of %d processes .\n", processId, concurrentProcessCount);

	int message = processId;
	
	if(processId==0)
	{
		printf("first process #0 send to process #1 \n");
		MPI_Isend(&message,1,MPI_INT,1,0,MPI_COMM_WORLD, &requests[0]);
		MPI_Irecv(&message, 1, MPI_INT , concurrentProcessCount-1, 0, MPI_COMM_WORLD, &requests[1]);

	}
	else if(processId!=concurrentProcessCount-1)
	{
		printf("process #%d isend message %d  to process #%d \n",processId, message, processId+1);
		MPI_Isend(&message,1,MPI_INT,processId+1,0,MPI_COMM_WORLD, &requests[0]);
		MPI_Irecv(&message,1,MPI_INT,processId-1,0,MPI_COMM_WORLD, &requests[1]);
	}
	else
	{
		printf("last process #%d send to process #0 \n", processId);
		MPI_Isend(&message, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &requests[0]);
		MPI_Irecv(&message, 1, MPI_INT,processId-1, 0, MPI_COMM_WORLD, &requests[1]);
	}
	
	MPI_Waitall(2, requests, statuses);
	printf("process #%d receive message %d \n",processId, message);
	

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