#include "msgs.h"
int server_port = 2;
int client_port = 6;

void createMessage(struct msg* msg,int seed)
{
	int i;
	for(i=0;i<10;i++)
	{
		msg->msg[i] = seed+i;
	}
}

void client()
{
	struct msg* msg;
	struct msg* recv_msg;
	printf("Client started\n");
	msg = (struct msg*) malloc(sizeof(struct msg));
	recv_msg = (struct msg*) malloc(sizeof(struct msg));
		
	createMessage(msg,10);
	
	while(1)
	{
		printf("Client Loop\n");
		int i;
		send(server_port,msg);
		receive(client_port,recv_msg);
		printf("Client: ");
		for(i=0;i<10;i++) printf("%d ",recv_msg->msg[i]);
		printf("\n");
	}
}


void server()
{
	struct msg* msg;
	struct msg* send_msg;
	printf("Server started1\n");
	msg = (struct msg*) malloc(sizeof(struct msg));
	send_msg = (struct msg*) malloc(sizeof(struct msg));
		
	createMessage(send_msg,15);
	
	while(1)
	{
		printf("Server Loop\n");
		int i;
		receive(server_port,msg);
		printf("Server: ");
		for(i=0;i<10;i++) printf("%d ",msg->msg[i]);
		printf("\n");
		send(client_port,send_msg);
	}
	
}

int main()
{
	init_RunQ();
	init_ports();
	
	start_thread(client);
	start_thread(server);
	run();
}