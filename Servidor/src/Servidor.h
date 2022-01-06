#ifndef EJERCICIO11_H_
#define EJERCICIO11_H_

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>
#include "Collections/queue.h"
#include <string.h>

void iniciar_conexion();

void queue_sync_push(t_queue *, void *element);
void *queue_sync_pop(t_queue *);

#endif /* EJERCICIO11_H_ */
