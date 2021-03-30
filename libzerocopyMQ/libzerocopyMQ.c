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
    int returned;
 

 	uint8_t type =0;
	uint16_t qSize=strlen(cola)+1;
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
    //printf("++ createMQ ++ - Returned 0 - OK\n");
    returned =  0;
	}
    else{
    //printf("++ createMQ ++ - Returned != 0 - NO OK\n");
    returned = -1;
}
close(s);
return returned;
}

int destroyMQ(const char *cola){
    ssize_t bytes_written;
	int iovcnt;
    struct iovec iov [3];
    int returned;

 	uint8_t type =1;
	uint16_t qSize=strlen(cola)+1;
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
    if (reply==0){
    	//printf("++ deleteMQ ++ - Returned 0 - OK\n");
    	returned = 0;
	}
    else{
    	//printf("++ deleteMQ ++ - Returned != 0 - NO OK\n");
    	returned = -1;
    }

    close(s);
	return returned;
    
}

int put(const char *cola, const void *mensaje, uint32_t tam) {
    ssize_t bytes_written;
	int iovcnt;
    struct iovec iov [5];
    int returned;

 	uint8_t type =2;
	uint16_t qSize=strlen(cola)+1;
	
	iov[0].iov_base = &type;
	iov[0].iov_len = sizeof(type);

	iov[1].iov_base = &qSize;
	iov[1].iov_len = sizeof(qSize);

	iov[2].iov_base = &tam;
	iov[2].iov_len = sizeof(tam);

	iov[3].iov_base = cola;
	iov[3].iov_len = sizeof(char)*qSize;

	iov[4].iov_base = mensaje;
	iov[4].iov_len = sizeof(void)*tam;

	iovcnt = sizeof(iov) / sizeof(struct iovec);
	
	if(brokerSetup()!=0){
		perror("error: put");
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
        perror("error en write put");
        return -1;
    }
  	int leido;
    uint8_t reply;
    if ((leido=recv(s, &reply, sizeof(reply),0))<0) {
            perror("error en read put");
            close(s);
            return -1;
    }
    
    close(s);
    if (reply==0)
    	//printf("++ Put ++ - Returned 0 - OK\n");
    	return 0;
    else
    	//printf("++ Put ++ - Returned != 0 - NO OK\n");
    	return -1;
}
	




int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
	ssize_t bytes_written;
	int iovcnt;
    struct iovec iov [3];
    //int returned;

 	uint8_t type = 3;
	uint16_t qSize=strlen(cola)+1;
	
	iov[0].iov_base = &type;
	iov[0].iov_len = sizeof(type);

	iov[1].iov_base = &qSize;
	iov[1].iov_len = sizeof(qSize);

	iov[2].iov_base = cola;
	iov[2].iov_len = sizeof(char)*qSize;


	iovcnt = sizeof(iov) / sizeof(struct iovec);
	
	if(brokerSetup()!=0){
		perror("error: put");
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
        perror("error en write put");
        return -1;
    }
    
    int leido;
    uint32_t msgSize;
    leido=recv(s,&msgSize,sizeof(msgSize),0);

    if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the size of the queue");
					close(s);
					return -1;
	}
	if ((int)msgSize<0){
		perror("error:queue not found");
		close(s);
		return -1;
	}
	if(msgSize == 0){
		if (!blocking)
			return 0;
		else
			return -1;
	}
	* tam = msgSize; //  creo que mal porque estoy copiando los datos
	// now that we know the size of the message we can allocate memory for it
	void * msg;
	msg = (void *) malloc(msgSize);
	leido=recv(s,msg,msgSize*sizeof(void),MSG_WAITALL);

	if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the size of the queue");
					close(s);
					return -1;
	}
	* mensaje = msg; //  creo que mal porque estoy copiando los datos

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
	//printf("Socket connection success\n");
	return 0;
}