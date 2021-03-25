#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "cola.h"
#include "diccionario.h"


int main(int argc, char *argv[]) {
	int s, s_conec, leido;
	unsigned int tam_dir;
	struct sockaddr_in dir, dir_cliente;
	int opcion=1;

	if (argc!=2) {
		fprintf(stderr, "Uso: %s puerto\n", argv[0]);
		return -1;
	}
	if ((s=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("error creando socket");
		return -1;
	}
	/* Para reutilizar puerto inmediatamente */
        if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &opcion, sizeof(opcion))<0){
                perror("error en setsockopt");
                return -1;
        }
	dir.sin_addr.s_addr=INADDR_ANY;
	dir.sin_port=htons(atoi(argv[1]));
	dir.sin_family=PF_INET;
	if (bind(s, (struct sockaddr *)&dir, sizeof(dir)) < 0) {
		perror("error en bind");
		close(s);
		return -1;
	}
	if (listen(s, 5) < 0) {
		perror("error en listen");
		close(s);
		return -1;
	}

	struct diccionario *dic;
    dic = dic_create(); // creamos el diccionario para las colas

	while (1) {
		tam_dir=sizeof(dir_cliente);
		if ((s_conec=accept(s, (struct sockaddr *)&dir_cliente, &tam_dir))<0){
			perror("error en accept");
			close(s);
			return -1;
		}

		/* memory allocation for the needed buffers*/
		uint8_t type; // type of conection so we know what to consult and reserve memory for.
		leido=recv(s_conec,&type,sizeof(uint8_t),0);
		printf("Type of operation: %d\n",type);

		if (leido<0) {
			perror("error: Could not read type of operation");
			close(s);
			close(s_conec);
			return -1;
		}


		/*instead of printing deconstruct the msg and act accordingly
		struct paquete {
						int op; // type 0: createMQ, type 1: destroyMQ , type 2: put , type 3: get 
						char *nombre_cola; // id for the queue
						void *mensaje;//
						....
				};
		int i;
		for(i=0; i<30 ; i++){
			printf("%s\n",&buf[i]);//traza mensaje
		}
		*/

		
		switch(type){

			case 0: // createMQ
				{
				uint8_t qSize;
				uint8_t i=0;
				leido=recv(s_conec,&qSize,sizeof(qSize),0);
				//printf("Size of queue: %d\n",qSize);

				if (leido<0) { // to be subtituted with a function to reduce size of code
					perror("error: Could not read the size of the queue");
					close(s);
					close(s_conec);
					return -1;
				}

				// now that we know the size of the queue we can allocate memory for it
				char * nombre_cola;
				nombre_cola = (char *) malloc(qSize);
				leido=recv(s_conec,nombre_cola,sizeof(nombre_cola),0);
				//printf("Queue Name: %s\n",nombre_cola);

				if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the queue name");
					close(s);
					close(s_conec);
					return -1;
				}
				struct cola *cola;
				cola = cola_create();
				if(dic_put(dic,nombre_cola,cola) < 0){
        			perror("error: La cola ya exite\n");
        			// Send -1 to client: Failed!
        			//return -1;
        			i=-1;
        			send(s, &i, sizeof(i), 0);
        			break;
        		}
				printf("++ createMQ ++ - ");
				printf("Nombre_cola:%s , ",nombre_cola);
				printf("Tamaño:%d\n",qSize);
				// Send 0 to client: Success!
				if (write(s_conec, &i, sizeof(i))<0){// to be subtituted with a function to reduce size of code
        			printf("Caca de la buena,el broker no ha podido devolver el ok\n");
        		}
        		break;
        	}
			case 1: // destroyMQ
				{
				uint8_t qSize;
				int error;
				uint8_t i=0;
				leido=recv(s_conec,&qSize,sizeof(qSize),0);
				//printf("Size of queue: %d\n",qSize);

				if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the size of the queue");
					close(s);
					close(s_conec);
					return -1;
				}

				// now that we know the size of the queue we can allocate memory for it
				char * nombre_cola;
				nombre_cola = (char *) malloc(qSize);
				leido=recv(s_conec,nombre_cola,sizeof(nombre_cola),0);
				//printf("Queue Name: %s\n",nombre_cola);

				if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the queue name");
					close(s);
					close(s_conec);
					return -1;
				}

				// proceed to look for the queue on the dict with name
				struct cola *cola;
				cola = dic_get(dic, nombre_cola, &error);

				if (error < 0){// to be subtituted with a function to reduce size of code
        			perror("error: No se encontro la cola en el diccionario");
        			// Send -1 to client: Failed!
        			//return -1;
        			i=-1;
        			send(s_conec, &i, sizeof(i), 0);
        			break;
				}
				// destroy the queue found on the dict
				if(cola_destroy(cola, NULL)<0){// to be subtituted with a function to reduce size of code
					perror("error: La cola no exite en la queue\n");
        			// Send -1 to client: Failed!
        			//return -1;
        			i=-1;
        			send(s_conec, &i, sizeof(i), 0);
        			break;
				}
				// remove from the dict the queue name
				if (dic_remove_entry(dic, nombre_cola, NULL) < 0){// to be subtituted with a function to reduce size of code
        			perror("error: La cola no exite en el diccionario\n");
        			// Send -1 to client: Failed!
        			//return -1;
        			i=-1;
        			send(s_conec, &i, sizeof(i), 0);
        			break;
				}

				printf("++ destroyMQ ++ - ");
				printf("Nombre_cola:%s , ",nombre_cola);
				printf("Tamaño:%d\n",qSize);
        		// Send 0 to client: Success!
        		//return 0;
        		if (write(s_conec, &i, sizeof(i))<0){
        			printf("Caca de la buena,el broker no ha podido devolver el ok\n");
        		}
        		break;
        	}
        	
			case 2: // put
			{
				//int error;
				//uint8_t i=0;

				uint8_t qSize;
				leido=recv(s_conec,&qSize,sizeof(qSize),0);
				if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the size of the queue");
					close(s);
					close(s_conec);
					return -1;
				}

				uint32_t msgSize;
				leido=recv(s_conec,&msgSize,sizeof(msgSize),0);
				if (leido<0) {// to be subtituted with a function to reduce size of code
					perror("error: Could not read the size of the message");
					close(s);
					close(s_conec);
					return -1;
				}

				// now that we know the size of the queue we can allocate memory for it
				char * nombre_cola;
				nombre_cola = (char *) malloc(qSize);
				leido=recv(s_conec,nombre_cola,sizeof(nombre_cola),0);

				// now that we know the size of the message we can allocate memory for it
				void * msg;
				msg = (void *) malloc(msgSize);
				leido=recv(s_conec,msg,sizeof(msg),0);

				printf("size cola:%d\n",qSize);
				printf("size mensaje:%d\n",msgSize);
				printf("nombre_cola:%s\n",nombre_cola);
				printf("mensaje:%d\n",*(uint8_t *)msg);
				break;
			}
			/*
			case 3: // get
				//qSize = buf[1];
				//uint8_t msgSize = buf[2];
				//char * nombre_cola = &buf[3];
				//char * nombre_cola = &buf[4];
				printf("get ");
				//printf("nombre_cola:%s ",nombre_cola);
				//printf("mensaje:%s\n",mensaje);
				break;
				*/
			default:
				perror("error: Unknow type");
		}
		
		/*
		leido=read(s_conec, buf, TAM);
			printf("prueba:%s\n",&buf[1]);
		*/
		
		printf("cerrando conexion\n" );
		close(s_conec);
	}
	close(s);
	return 0;
}