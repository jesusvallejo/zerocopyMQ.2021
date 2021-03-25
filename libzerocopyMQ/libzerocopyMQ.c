#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"


#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/uio.h>


  int s; // socket connection 

int createMQ(const char *cola) {

    ssize_t bytes_written;
	int iovcnt;
    struct iovec iov [3];
    
 

 	uint8_t type =0;
	uint8_t qSize=strlen(cola)+1;
	//uint8_t msgSize=strlen(msg)+1;
	
	iov[0].iov_base = &type;
	iov[0].iov_len = sizeof(type);

	iov[1].iov_base = &qSize;
	iov[1].iov_len = sizeof(qSize);

	//iov[2].iov_base = &msgSize;
	//iov[2].iov_len = sizeof(msgSize);

	iov[2].iov_base = cola;
	iov[2].iov_len = qSize;

	//iov[4].iov_base = msg;
	//iov[4].iov_len = msgSize;


	iovcnt = sizeof(iov) / sizeof(struct iovec);
	
	if(brokerSetup()!=0){
		perror("error: createMQ");
		return -1;
	}

	/* Understanding that the queue struct is set send it, 
	we need to change it to writev for the zerocopy  

    if (write(s, cola, strlen(cola)+1)<0) {
        perror("error en write");
        return 1;
    }
    */
    
    bytes_written= writev(s, iov, iovcnt);
    if (bytes_written<0) {
        perror("error en write");
        return -1;
    }
    int leido;
	
    uint8_t reply;
    if ((leido=read(s, &reply, sizeof(reply)))<0) {
            perror("error en read");
            close(s);
            return -1;
    }
    if (reply==0){
    printf("++ createMQ ++ - Returned 0 - OK\n");
    return -1;
	}
    else{
    printf("++ createMQ ++ - Returned != 0 - NO OK\n");
    return 0;
}
}

int destroyMQ(const char *cola){
    ssize_t bytes_written;
	int iovcnt;
    struct iovec iov [3];
    

 	uint8_t type =1;
	uint8_t qSize=strlen(cola)+1;
	//uint8_t msgSize=strlen(msg)+1;
	
	iov[0].iov_base = &type;
	iov[0].iov_len = sizeof(type);

	iov[1].iov_base = &qSize;
	iov[1].iov_len = sizeof(qSize);

	//iov[2].iov_base = &msgSize;
	//iov[2].iov_len = sizeof(msgSize);

	iov[2].iov_base = cola;
	iov[2].iov_len = qSize;

	//iov[4].iov_base = msg;
	//iov[4].iov_len = msgSize;


	iovcnt = sizeof(iov) / sizeof(struct iovec);
	
	if(brokerSetup()!=0){
		perror("error: createMQ");
		return 1;
	}

	/* Understanding that the queue struct is set send it, 
	we need to change it to writev for the zerocopy  

    if (write(s, cola, strlen(cola)+1)<0) {
        perror("error en write");
        return 1;
    }
    */

    bytes_written= writev(s, iov, iovcnt);
    if (bytes_written<0) {
        perror("error en write");
        return 1;
    }
    int leido;
	
	uint8_t reply;
    if ((leido=read(s, &reply, sizeof(reply)))<0) {
            perror("error en read");
            close(s);
            return 1;
    }
    if (reply==0)
    	printf("++ deleteMQ ++ - Returned 0 - OK\n");
    else
    	printf("++ deleteMQ ++ - Returned != 0 - NO OK\n");

    close(s);
    return 0;
}


int put(const char *cola, const void *mensaje, uint32_t tam) {
    ssize_t bytes_written;
	int iovcnt;
    struct iovec iov [5];

    

 	uint8_t type =2;
	uint8_t qSize=strlen(cola)+1;
	/* esto es incorrecto como dice el enunciado ya que son de tipo binario*/
	//uint8_t msgSize=strlen(mensaje)+1;
	
	iov[0].iov_base = &type;
	iov[0].iov_len = sizeof(type);

	iov[1].iov_base = &qSize;
	iov[1].iov_len = sizeof(qSize);

	iov[2].iov_base = &tam;
	iov[2].iov_len = sizeof(tam);

	iov[3].iov_base = cola;
	iov[3].iov_len = qSize;

	iov[4].iov_base = mensaje;
	//iov[4].iov_len = msgSize;
	iov[4].iov_len = tam;


	iovcnt = sizeof(iov) / sizeof(struct iovec);
	
	if(brokerSetup()!=0){
		perror("error: createMQ");
		return 1;
	}

	/* Understanding that the queue struct is set send it, 
	we need to change it to writev for the zerocopy  

    if (write(s, cola, strlen(cola)+1)<0) {
        perror("error en write");
        return 1;
    }
    */

    bytes_written= writev(s, iov, iovcnt);
    if (bytes_written<0) {
        perror("error en write");
        return 1;
    }
    close(s);
    return 0;
}



int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
    return 0;
}

int brokerSetup(){
	struct sockaddr_in dir;
	struct hostent *host_info;
	char * broker_port = getenv("BROKER_PORT") ;
	char * broker_host= getenv("BROKER_HOST");

	if(broker_port == NULL || broker_port == NULL){
		perror("error: ENV not set, set BROKER_PORT && BROKER_HOST");	
		return 1;	
	}
	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error:could not create socket");
		return 1;
	}
	host_info=gethostbyname(broker_host);
	dir.sin_addr=*(struct in_addr *)host_info->h_addr;
	dir.sin_port=htons(atoi(broker_port));
	dir.sin_family=PF_INET;

	if (connect(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error: could not connect");
		close(s);
		return 1;
	}
	printf("Socket connection success\n");
	return 0;
}