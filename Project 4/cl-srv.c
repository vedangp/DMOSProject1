#include "msgs.h"

#define MAX_MSG_LENGTH		40
#define NUMBER_OF_STRINGS 	10
#define MAX_STRING_LENGTH	37

#define SERVER_PORT	 	20
#define CLIENT1_RECV_PORT 	40
#define CLIENT2_RECV_PORT 	60
#define CLIENT3_RECV_PORT 	80

#define COMMAND_ADD		0
#define COMMAND_DELETE		1
#define COMMAND_MODIFY		2
#define COMMAND_PRINT		3

#define ACK			4
#define ACK_PRINT		5

#define CANNOT_ADD		-1
#define	CANNOT_DELETE		-2
#define	CANNOT_MODIFY		-3
#define	CANNOT_PRINT		-4
#define	NACK			-5

#define MORE_DATA		6
#define	END_OF_DATA		7

int number_of_strings =0;

struct send_data_packet{
	unsigned char client_port_number;
	unsigned char command;
	unsigned char position;
	char* string;
};

struct receive_data_packet{
	unsigned char ack;
	char* strings;
};

char ** strings;

int data_size(struct send_data_packet* data_packet)
{
	if (data_packet->string != NULL)
		return 4 + strlen(data_packet->string);
	else 
		return 3;
}

void send_data(int port_number, struct send_data_packet* data_packet)
{
	int size = data_size(data_packet);
	if (size < MAX_MSG_LENGTH)
	{
		char* data = (char*) data_packet;
		char *msg; 
		int i;
		msg = malloc(MAX_MSG_LENGTH);
		msg[0] = data[0];
		msg[1] = data[1];
		msg[2] = data[2];
		if(data[1] != COMMAND_PRINT || data[1] != COMMAND_DELETE)
		{
			for (i=3;i<MAX_MSG_LENGTH-1;i++)
			{
				msg[i] =  (i < size) ? data_packet->string[i-3] : 0;
			}
		}
		msg[MAX_MSG_LENGTH-1] = END_OF_DATA;
		send(port_number, (struct msg*)msg);
	}
	else
	{
	}
}

int receive_recurse_data(int port_number, char** string)
{
	char* msg;
	int j = 0;

	msg =(char*) malloc(sizeof(struct msg));
	receive(port_number, (struct msg*) msg);
	if (msg[0] == ACK_PRINT)
	{
		if (msg[MAX_MSG_LENGTH-1] == END_OF_DATA)
		{
			int i=0;
			*(string) = (char*)malloc(MAX_MSG_LENGTH-1);
			while (j!=NUMBER_OF_STRINGS)
			{
			//	printf("while iter %d\n",i);
				(*string)[i] = msg[i+1];
				j += (msg[i] == '\0' || msg[i] == 0) ? 1: 0;
				i++;
			}
		}
		else
		{
			
		}
	}else
	{
		string = NULL;
	}
	return msg[0];
}

struct receive_data_packet* receive_data(int port_number)
{
	struct receive_data_packet* data;
	
	data = malloc(sizeof(struct receive_data_packet));

	data->ack = receive_recurse_data(port_number,&data->strings);

	return data;
}

int client_add(int client_port_number, char* string, int position)
{
	struct send_data_packet data;
	struct receive_data_packet* reply_data;
	data.string = malloc(strlen(string));
	data.client_port_number = client_port_number;
	data.command = COMMAND_ADD;
	data.position = position;
	strcpy(data.string,string);
	
	send_data(SERVER_PORT, &data);
	reply_data = receive_data(client_port_number);

	return reply_data->ack;
}

int client_delete(int client_port_number,int position)
{
	struct send_data_packet data;
	struct receive_data_packet* reply_data;
	data.string = NULL;
	data.client_port_number = client_port_number;
	data.command = COMMAND_DELETE;
	data.position = position;
	
	send_data(SERVER_PORT, &data);
	reply_data = receive_data(client_port_number);

	return reply_data->ack;
	
}

int client_modify(int client_port_number, char* string, int position)
{
	struct send_data_packet data;
	struct receive_data_packet* reply_data;
	data.string = malloc(30);
	data.client_port_number = client_port_number;
	data.command = COMMAND_ADD;
	data.position = position;
	strcpy(data.string,string);
	
	send_data(SERVER_PORT, &data);
	reply_data = receive_data(client_port_number);

	return reply_data->ack;	
}

int client_print(int port_number)
{
	struct send_data_packet data;
	struct receive_data_packet* reply_data;
	int i,pos=0;
	data.string = NULL;
	data.client_port_number = port_number;
	data.command = COMMAND_PRINT;
	
	send_data(SERVER_PORT, &data);
	reply_data = receive_data(port_number);

	for (i=0;i<NUMBER_OF_STRINGS;i++)
	{
		if (reply_data->strings[pos] != 0)
		{
			printf("%d: %s\n",i,&(reply_data->strings[pos]));
			pos += strlen(&(reply_data->strings[pos]))+1;
		}else
		{
			printf("%d: NO STRING\n",i);
			pos++;
		}
	}

	return reply_data->ack;	
}

void print_strings_table()
{
	int i;
	
	printf("strings table\n");
	
	for(i=0;i<NUMBER_OF_STRINGS;i++)
	{
		printf("%d: %s\n",i,strings[i]);
	}
}

void client()
{
	int inp;
	int i;
	char string[30];

	
	while(1)
	{
		printf("What do you want to do?\n");
		scanf("%d",&inp);

		switch(inp)
		{
			case 0:
				printf("enter the position where you want to add:\n");
				scanf("%d",&i);
				printf("enter the string you want to add:");
				scanf("%s",string);
				client_add(CLIENT1_RECV_PORT,string,i);
				break;
			case 1:
				printf("delete position: \n");
				scanf("%d",&i);
				client_delete(CLIENT1_RECV_PORT,i);
				break;
			case 2:
				printf("modify position:\n");
				scanf("%d",&i);
				printf("modify string:\n");
				scanf("%s",string);
				client_modify(CLIENT1_RECV_PORT,string,i);
				break;
			case 3: 
				print_strings_table();
				client_print(CLIENT1_RECV_PORT);
				break;
			default: 
				printf("invalid\n");
		}
		yield();
	}
}

void server_init()
{
	int i;
	strings = calloc (NUMBER_OF_STRINGS, sizeof(char*));
	
	for (i=0;i<NUMBER_OF_STRINGS;i++)
	{
		strings[i] = NULL;
	}
}

int server_add(char* string, int position)
{
	if (position < 0 || position > NUMBER_OF_STRINGS)
		return CANNOT_ADD;
	if (string == NULL)
		return CANNOT_ADD;

	strings[position] = malloc(strlen(string)+1);
	strcpy(strings[position],string);

	return ACK;
}

int server_delete(int position)
{
	if (position < 0 || position > NUMBER_OF_STRINGS)
		return CANNOT_DELETE;
	
	free(strings[position]);
	strings[position] = NULL;

	return ACK;
}

int server_modify(char* string, int position)
{
	if (position < 0 || position > NUMBER_OF_STRINGS)
		return CANNOT_MODIFY;
	if (string == NULL)
		return CANNOT_MODIFY;

	strings[position] = realloc(strings[position],strlen(string)+1);
	strcpy(strings[position],string);

	return ACK;
}

int all_string_length(char** strings)
{
	int i,length=0;
	for(i=0;i<NUMBER_OF_STRINGS;i++)
	{
		if (strings[i] != NULL)
			length += strlen(strings[i]) + 1;
		else 
			length += 1;
	}

	return length;
}

char* server_print()
{
	int length=0,i,j,super_string_length=0;
	char* super_string;

	length = all_string_length(strings);

	super_string = malloc(sizeof(char)*length);

	for(i=0;i<NUMBER_OF_STRINGS;i++)
	{
		if (strings[i] != NULL)
		{
			for (j=0;j<strlen(strings[i]);j++)
			{
				super_string[super_string_length++] = strings[i][j];
			}
			super_string[super_string_length++] = '\0';
		}
		else 
		{	
			super_string[super_string_length++] = 0;
		}
	}

	return super_string;
}

void send_server_data(int port_number, struct receive_data_packet* data)
{
	int size = all_string_length(strings) + 1,i;
	char* msg; 

	msg = malloc(sizeof(char)* MAX_MSG_LENGTH);
	if (size < MAX_MSG_LENGTH)
	{
		msg[0] = ACK_PRINT;
		for (i=1;i<MAX_MSG_LENGTH;i++)
		{
			msg[i] = (i < size) ? data->strings[i-1] : 0;
		}
		msg[MAX_MSG_LENGTH - 1] = END_OF_DATA;
		send(port_number, (struct msg*)msg);
	}else
	{
	}
}

void server()
{
	char* msg;
	char* reply_msg;
	msg =(char*) malloc(sizeof(struct msg));
	reply_msg = (char*) calloc(1,sizeof(struct msg));
	while(1)
	{
		int ret;
		receive(SERVER_PORT,(struct msg*)msg);
		struct receive_data_packet* data;
		data = malloc (sizeof(struct receive_data_packet));
		char* strings;
		switch(msg[1])
		{
			case COMMAND_ADD:
				ret = server_add(&msg[3],msg[2]);
				reply_msg[0] = ret;
				send(msg[0],(struct msg*)reply_msg);
				break;
			case COMMAND_DELETE:
				ret = server_delete(msg[2]);
				reply_msg[0] = ret;
				send(msg[0],(struct msg*)reply_msg);
				break;
			case COMMAND_MODIFY:
				ret = server_modify(&msg[3],msg[2]);
				reply_msg[0] = ret;
				send(msg[0],(struct msg*)reply_msg);
				break;
			case COMMAND_PRINT:
				strings = server_print();
				data->ack = ACK_PRINT;
				data->strings = strings;
				send_server_data(msg[0],data);
				free(data);
				break;
			default:
				printf("Invalid Command\n");
		}
		yield();
//		printf("dasd %d %d %d %s\n",msg[0],msg[1],msg[2], &msg[3]);
	}
}

int main()
{
	init_RunQ();
	init_ports();
	server_init();
	printf("\101");
	
	start_thread(client);
	start_thread(server);

	run();
}