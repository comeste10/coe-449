#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>

#define BUFFSIZE 512

int main(int argc, char ** argv) {
    int con_fd = 0;
    int ret = 0;
    struct sockaddr_in serv_addr;

    unsigned short port = 0;
    char * file_name = NULL;


    if (argc != 3) {
	printf("Usage: ./client <port> <file-name>\n");
	return -1;
    }

    port = atoi(argv[1]);
    file_name = argv[2];
    

    con_fd = socket(PF_INET, SOCK_STREAM, 0);

    if (con_fd == -1) {
	perror("Socket Error\n");
	return -1;
    }

    
    memset(&serv_addr, 0, sizeof(struct sockaddr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");;
    
  
    ret = connect(con_fd, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr));

    if (ret < 0) {
	perror("Connect error\n");
	return -1;
    }

    char buff[512];
    int sizeIn = 0;
    FILE * outFile = fopen(file_name,"w");
    if(outFile == NULL) {
	perror("fopen error\n");
	return -1;
    }

    while((sizeIn = recv(con_fd, &buff, BUFFSIZE, 0)) > 0) {
        fwrite(&buff,1,sizeIn,outFile);
    }

    fclose(outFile);    
    
    close(con_fd);


    return 0;
}
