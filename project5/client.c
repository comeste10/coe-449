// compile with: gcc -Wall -m32 -std=c99 -pthread client.c

// Steven Comer
// sfc15
// Project 5 - Parallel Downloads
// client.c
// 22 April 2013

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

// MACROS
#define BUFFSIZE 1024
#define OUTPUT_FILE "output.txt"

// GLOBALS
void * threadExec(void *);
FILE * outfile;
int numThreads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct connection_t {
	int threadNum;
	char ip_addr[20];
	unsigned short port;	
} connection_t;


int main(int argc, char ** argv) {

	///////////////////////////////SETUP///////////////////////////////////	
        if(argc < 3 || (argc%2) == 0) {
                printf("Usage: ./client <ip_addr_n> <port_n> ...\n");
                return -1;
        }

	numThreads = (argc-1)/2;

        outfile = fopen(OUTPUT_FILE,"w+");
        if(outfile == NULL) {
                perror("fopen error\n");
                return -1;
        }

	connection_t * threadData = malloc(sizeof(connection_t)*numThreads);
	for(int i=0; i<numThreads; i++) {
		threadData[i].threadNum = i;
		strcpy(threadData[i].ip_addr,argv[(i*2)+1]);
		threadData[i].port = atoi(argv[(i+1)*2]);
	}
	///////////////////////////////////////////////////////////////////////

	
	///////////////////////////SPAWN THREADS///////////////////////////////
	pthread_t threads[numThreads];
	for(int i=0; i<numThreads; i++) {
		pthread_create(&threads[i],NULL,threadExec,(void *)&threadData[i]);
	}
	///////////////////////////////////////////////////////////////////////

	
	///////////////////////////JOIN THREADS////////////////////////////////
	for(int i=0; i<numThreads; i++) {
		pthread_join(threads[i],NULL);
	}
	///////////////////////////////////////////////////////////////////////


	/////////////////////////////CLEAN UP//////////////////////////////////
	free(threadData);
	fclose(outfile);
	///////////////////////////////////////////////////////////////////////


	return 0;
}

void * threadExec(void * data) {

	///////////////////////////SETUP SOCKET///////////////////////////////
	connection_t info = *(connection_t *)data;
	int threadNum = info.threadNum;
	char stringcounter[20];
	char buff[1024];

        int con_fd = 0;
        int ret = 0;
        struct sockaddr_in serv_addr;

        memset(&serv_addr,0,sizeof(struct sockaddr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(info.port);
        serv_addr.sin_addr.s_addr = inet_addr(info.ip_addr);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////GET CHUNKS//////////////////////////////////
	int sizein = 0;
	int chunkNumber = 0;
	for(int i=0; 1; i++) {
		chunkNumber = threadNum + numThreads*i;
	        con_fd = socket(PF_INET,SOCK_STREAM,0);
	        if(con_fd == -1) {
        	        perror("Socket error\n");
                	exit(-1);
        	}
	        ret = connect(con_fd,(struct sockaddr *)&serv_addr,sizeof(struct sockaddr));
        	if(ret < 0) {
                        perror("Connect error\n");
                       	exit(-1);
                }
	        sprintf(stringcounter,"%d",chunkNumber);
	        send(con_fd,stringcounter,strlen(stringcounter),0);
 	        sizein = recv(con_fd,&buff,BUFFSIZE,0);
		close(con_fd);
        	#ifdef DEBUG
			printf("threadNumber  = %d\n",threadNum);
                        printf("chunkNumber   = %d\n",chunkNumber);
                      	printf("stringcounter = %s\n",stringcounter);
                       	printf("sizein        = %d\n\n",sizein);
                #endif
                if(sizein <= 0) break;
		pthread_mutex_lock(&mutex);
                	fseek(outfile,BUFFSIZE*chunkNumber,0);
               		fwrite(&buff,1,sizein,outfile);
		pthread_mutex_unlock(&mutex);
	}
	///////////////////////////////////////////////////////////////////////


	return NULL;	
}
