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
	char * msg = "2wfk";
	void * rmsg;
	uint32_t  tam;
	bool blocking = false;

    if (createMQ(cola) == 0)
    	printf("Test1-OK\n");
   	else
   		printf("Test1-BAD\n");


   	if (put(cola,msg,strlen(msg)+1 )== 0)
    	printf("Test2-OK\n");
   	else
   		printf("Test2-BAD\n");

   
   	if (get(cola,&rmsg,&tam,blocking)==0){
   		if(strcmp(msg,(char *)rmsg)==0)
   				printf("Test3-OK\n");
    	 
	}
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
    
    	if (put(cola,msg,strlen(msg)+1) < 0)
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

   	if(put(cola,msg,strlen(msg)+1)==0){
   		if (get(&cola[1],&rmsg,&tam,blocking)==0){
   			if(get(cola,&rmsg,&tam,blocking)==0 && strcmp(msg,(char *)rmsg )==0)
   				printf("Test10-OK\n");
   			else
   				printf("Test10.3-BAD\n");
   		}
   		else
   			printf("Test10.2-BAD\n");
   	}
   	else
   		printf("Test10.1-BAD\n");




   	cola = "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";
	msg = "gfdhfghhdfghdfgdhfhdfghdfgh";
	blocking = false;

    if (createMQ(cola) == 0)
    	printf("Test11-OK\n");
   	else
   		printf("Test11-BAD\n");


   	if (put(cola,msg,strlen(msg)+1) == 0)
    	printf("Test12-OK\n");
   	else
   		printf("Test12-BAD\n");


   	if (get(cola,&rmsg,&tam,blocking) == 0 && strcmp(msg,(char *)rmsg)==0)
    	printf("Test13-OK\n");
   	else
   		printf("Test13-BAD\n");


   	if (destroyMQ(cola) == 0)
    	printf("Test14-OK\n");
   	else
   		printf("Test14-BAD\n");


    if (destroyMQ(cola) <0)
    	printf("Test15-OK\n");
   	else
   		printf("Test15-BAD\n");


   	if (createMQ(cola) == 0 ){
   		if (createMQ(cola)<0)
    		printf("Test16-OK\n");
   	}
   	else
   		printf("Test16-BAD\n");


   	if (destroyMQ(cola) == 0){
   		if(get(cola,&rmsg,&tam,blocking)<0)
    		printf("Test17-OK\n");
    	if (put(cola,msg,strlen(msg)+1) < 0)
    		printf("Test18-OK\n");
    	else
    		printf("Test18-BAD\n");
    }
   	else
   		printf("Test17-BAD\n");

    if (createMQ(cola) == 0 && createMQ(&cola[1])==0){
    	if(destroyMQ(&cola[2])<0)
    		printf("Test19-OK\n");
    	else
    		printf("Test19.1-BAD\n");
    }
   	else
   		printf("Test19-BAD\n");

   	if(put(cola,msg,strlen(msg)+1)==0){
   		if (get(&cola[1],&rmsg,&tam,blocking)==0){
   			if(get(cola,&rmsg,&tam,blocking)==0 && strcmp(msg,(char *)rmsg )==0)
   				printf("Test20-OK\n");
   			else
   				printf("Test20.3-BAD\n");
   		}
   		else
   			printf("Test20.2-BAD\n");
   	}
   	else
   		printf("Test20.1-BAD\n");


   
   
    return 0;
}
