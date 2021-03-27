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
	void **rmsg;
	uint32_t *rtam;
	bool blocking = false;
	
    createMQ(cola);
    
    put(cola,&msg,sizeof(msg));

    get(cola,rmsg,rtam,blocking);
    return 0;
}
