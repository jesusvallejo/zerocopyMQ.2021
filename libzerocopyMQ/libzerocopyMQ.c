#include <stdint.h>
#include "zerocopyMQ.h"
#include "comun.h"

//#define port BROKER_PORT
//define host BROKER_HOST

int createMQ(const char *cola) {
    return 0;
}
int destroyMQ(const char *cola){
    return 0;
}
int put(const char *cola, const void *mensaje, uint32_t tam) {

    return 0;
}
int get(const char *cola, void **mensaje, uint32_t *tam, bool blocking) {
    return 0;
}

void brokerSetup(){
	char * broker_port;
	char * broker_host:
	if!(brokergetenv())
}