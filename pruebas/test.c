#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "zerocopyMQ.h"
#include <string.h>

 
int main(int argc, char * argv[]){
	char * cola = "ppp";
	int  msg = 7;
	void * rmsg;
	uint32_t  tam;
	bool blocking = false;

    if (createMQ(cola) == 0)
    	printf("Test1-OK\n");
   	else
   		printf("Test1-BAD\n");


   	if (put(cola,&msg,sizeof(msg)) == 0)
    	printf("Test2-OK\n");
   	else
   		printf("Test2-BAD\n");


   	if (get(cola,&rmsg,&tam,blocking) == 0 && msg == *(int *)rmsg)
    	printf("Test3-OK\n");
   	else
   		printf("Test3-BAD\n");


   	if (destroyMQ(cola) == 0)
    	printf("Test4-OK\n");
   	else
   		printf("Test4-BAD\n");


    if (destroyMQ(cola) <0)
    	printf("Test5-OK\n");
   	else
   		printf("Test5-BAD\n");


   	if (createMQ(cola) == 0 ){
   		if (createMQ(cola)<0)
    		printf("Test6-OK\n");
   	}
   	else
   		printf("Test6-BAD\n");


   	if (destroyMQ(cola) == 0){
   		if(get(cola,&rmsg,&tam,blocking)<0)
    		printf("Test7-OK\n");

    
    	if (put(cola,&msg,sizeof(msg)) < 0)
    		printf("Test8-OK\n");
    	else
    		printf("Test8-BAD\n");
    }
   	else
   		printf("Test7-BAD\n");

    if (createMQ(cola) == 0 && createMQ(&cola[1])==0){
    	if(destroyMQ(&cola[2])<0)
    		printf("Test9-OK\n");
    	else
    		printf("Test9-BAD\n");
    }
   	else
   		printf("Test9-BAD\n");

   	if(put(cola,&msg,sizeof(msg))==0){
   		if (get(&cola[1],&rmsg,&tam,blocking)==0){
   			if(get(cola,&rmsg,&tam,blocking)==0 && msg == *(int *)rmsg )
   				printf("Test10-OK\n");
   			else
   				printf("Test10.3-BAD\n");
   		}
   		else
   			printf("Test10.2-BAD\n");
   	}
   	else
   		printf("Test10.1-BAD\n");

   
   
    return 0;
}
