#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 512


int main(int argc, char ** argv) {
    int serv_fd = 0;
    int con_fd = 0;
    int ret = 0;
    struct sockaddr_in serv_addr;
    struct sockaddr client_addr;
    int client_addr_len = sizeof(struct sockaddr);

    unsigned short port = 0;
    char * file_name = NULL;


  if (argc != 3) {
	printf("Usage: ./server <port> <file-name>\n");
	return -1;
    }

    port = atoi(argv[1]);
    file_name = argv[2];
    

    serv_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_fd == -1) {
	perror("Socket Error\n");
	return -1;
    }

    
    memset(&serv_addr, 0, sizeof(struct sockaddr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    
    ret = bind(serv_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

    if (ret != 0) {
	perror("Bind Error\n");
	return -1;
    }


    ret = listen(serv_fd, 10);

    if (ret != 0) {
	perror("Listen Error\n");
	return -1;
    }

    
    con_fd = accept(serv_fd, &client_addr,(socklen_t *)&client_addr_len);

    if (con_fd < 0) {
	perror("Accept error\n");
	return -1;
    }


    char buff[512];
    FILE * inFile = fopen(file_name,"rb");
    if(inFile == NULL) {
	perror("fopen error\n");
	return -1;
    }

    int outSize = 0;

    while((outSize = fread(&buff,1,BUFFSIZE,inFile)) > 0) {
	send(con_fd,&buff,outSize,0);
    }


    fclose(inFile);

    
    close(con_fd);

    return 0;
}
