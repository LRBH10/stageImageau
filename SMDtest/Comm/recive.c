#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>

#define   IPC_SMD_MONITOR_IDENTIFIER  0x00007588


typedef struct {
	long type;
	char texte [256];
} message_t;

int main (int argc, char * argv[]) {
	key_t 	key;
	message_t message;
	int	file;
	long	type;

	if (argc != 3) {
		fprintf(stderr, "Syntaxe : %s fichier_clé type \n", argv[0]);
		exit(EXIT_FAILURE);
	}

	if ((key = (key_t) IPC_SMD_MONITOR_IDENTIFIER) == -1) {
		perror("ftok");
		exit(EXIT_FAILURE);
	}

	printf("LA Key est %d\n", key);

	if (sscanf(argv[2], "%ld", & type) != 1) {
		fprintf(stderr, "Type invalide");
		exit(EXIT_FAILURE);
	}

	if ((file = msgget(key, IPC_CREAT | 0600)) == -1) {
		perror("msgget");	
		exit(EXIT_FAILURE);
	}

	if (msgrcv(file, (void *) & message, 256, type, 0) >= 0)
		fprintf(stdout, "(%ld) %s \n", message.type, message.texte);
	else
		perror("msgrcv");
	
	return EXIT_SUCCESS;
}

