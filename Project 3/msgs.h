#include "sem.h"
#define N 			10
#define MSG_QUEUE_LENGTH	10
#define NUMBER_OF_PORTS		10

// Strategy 1: mutex for 1 port


struct msg
{
	int msg[N];
};

struct port 
{
	struct msg msg_queue[MSG_QUEUE_LENGTH];
	struct Semaphore_t *full;
	struct Semaphore_t *empty;
	int current_position;
	struct Semaphore_t *mutex;
};

struct port* ports;

init_port(struct port* port)
{
	port->full = CreateSem(0);
	port->empty = CreateSem(MSG_QUEUE_LENGTH);
	port->current_position = 0;
	port->mutex = CreateSem(1);
}

init_ports()
{
	int i;
	ports = (struct port*) malloc(sizeof(struct port) * NUMBER_OF_PORTS);
	for (i=0;i<NUMBER_OF_PORTS;i++)
	{
		init_port(&ports[i]);
	}
	
}

copy_message(struct msg* source, struct msg* destination)
{
	int i =0;
	for (i=0;i<N;i++)
	{
		destination->msg[i] = source->msg[i];
	}
}

int send(int port_number, struct msg* msg)
{
	struct port* port = &(ports[port_number]);
	if (port_number > 99 || port_number < 0)
		return -1;
	if (msg == NULL)
		return -1;
	P(port->empty);
	P(port->mutex);
	copy_message(msg,&(port->msg_queue[port->current_position]));
	port->current_position++;
	V(port->mutex);
	V(port->full);
}

int receive(int port_number,struct msg* msg)
{
	struct port* port = &(ports[port_number]);
	if (port_number > 99 || port_number < 0)
		return -1;
	if (msg == NULL)
		return -1;
	P(port->full);
	P(port->mutex);
	port->current_position--;
	copy_message(&(port->msg_queue[port->current_position]),msg);
	V(port->mutex);
	V(port->empty);
}