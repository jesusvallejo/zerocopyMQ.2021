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
    createMQ("ppp");
    int  msg = 7;
    put("ppp",&msg,sizeof(msg));
    return 0;
}
