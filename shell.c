// Shell starter file
// You may make any changes to any part of this file.

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define COMMAND_LENGTH 1024
#define NUM_TOKENS (COMMAND_LENGTH / 2 + 1)
#define HISTORY_DEPTH 10

char history[HISTORY_DEPTH][COMMAND_LENGTH];
int hisIndex=0;

void addHistory(char *command);
void displayHistory(int index);
/**
 * Command Input and Processing
 */

/*
 * Tokenize the string in 'buff' into 'tokens'.
 * buff: Character array containing string to tokenize.
 *       Will be modified: all whitespace replaced with '\0'
 * tokens: array of pointers of size at least COMMAND_LENGTH/2 + 1.
 *       Will be modified so tokens[i] points to the i'th token
 *       in the string buff. All returned tokens will be non-empty.
 *       NOTE: pointers in tokens[] will all point into buff!
 *       Ends with a null pointer.
 * returns: number of tokens.
 */
int tokenize_command(char *buff, char *tokens[])
{
	int token_count = 0;
	_Bool in_token = false;
	int num_chars = strnlen(buff, COMMAND_LENGTH);
	for (int i = 0; i < num_chars; i++) {
		switch (buff[i]) {
		// Handle token delimiters (ends):
		case ' ':
		case '\t':
		case '\n':
			buff[i] = '\0';
			in_token = false;
			break;

		// Handle other characters (may be start)
		default:
			if (!in_token) {
				tokens[token_count] = &buff[i];
				token_count++;
				in_token = true;
			}
		}
	}
	tokens[token_count] = NULL;
	return token_count;
}


/**
 * Read a command from the keyboard into the buffer 'buff' and tokenize it
 * such that 'tokens[i]' points into 'buff' to the i'th token in the command.
 * buff: Buffer allocated by the calling code. Must be at least
 *       COMMAND_LENGTH bytes long.
 * tokens[]: Array of character pointers which point into 'buff'. Must be at
 *       least NUM_TOKENS long. Will strip out up to one final '&' token.
 *       tokens will be NULL terminated (a NULL pointer indicates end of tokens).
 * in_background: pointer to a boolean variable. Set to true if user entered
 *       an & as their last token; otherwise set to false.
 */
void read_command(char *buff, char *tokens[], _Bool *in_background)
{
	*in_background = false;

	// Read input
	int length = read(STDIN_FILENO, buff, COMMAND_LENGTH-1);

	if (length < 0) {
		perror("Unable to read command from keyboard. Terminating.\n");
		exit(-1);
	}



	// Null terminate and strip \n.
	buff[length] = '\0';
	if (buff[strlen(buff) - 1] == '\n') {
		buff[strlen(buff) - 1] = '\0';
	}

	if (buff[0]!='!'){
		addHistory(buff);
		hisIndex++;
	}


	// Tokenize (saving original command string)
	int token_count = tokenize_command(buff, tokens);
	if (token_count == 0) {
		return;
	}

	// Extract if running in background:
	if (token_count > 0 && strcmp(tokens[token_count - 1], "&") == 0) {
		*in_background = true;
		tokens[token_count - 1] = 0;
	}
}

/*  history helper functions */





	// if(index<10){
	// 	int roll = 0;
	// 	int count = 1;
	// 	for(;roll<index;roll++,count++){
	// 		for(;roll<index;roll++,count++){
	// 			printf("%d", count);
	// 			printf("	%s\n", mat[roll]);
	// 		}
	//
	// 	}
	//  }
	// else{
	// 	int roll;
	// 	int count = index-10;
	// 	for(roll = index%10;roll<10;roll++,count++){
	// 		printf("%d", count);
	// 		printf("	%s\n", mat[roll]);
	// 	}
	// 	for(roll = 0; roll<index%10;roll++,count++){
	// 		printf("%d", count);
	// 		printf("	%s\n", mat[roll]);
	// 	}
	// }












/**
 * Main and Execute Commands
 */
int main(int argc, char* argv[])
{
	char input_buffer[COMMAND_LENGTH];
	char *tokens[NUM_TOKENS];

	for(int a=0 ;a<10;a++){												// reset history
			for(int b=0 ;b<COMMAND_LENGTH;b++)
				history[a][b]='\0';
	}



	while (true) {
		// Get command
		// Use write because we need to use read() to work with
		// signals, and read() is incompatible with printf().



		//display folder and '>'
		char dirBuf[100];
		char* disDir=getcwd(dirBuf,sizeof(dirBuf));
		strcat(disDir,"> ");
		bool isMine=false;
		write(STDOUT_FILENO,disDir,strlen(disDir));



		_Bool in_background = false;
		read_command(input_buffer, tokens, &in_background);


		// DEBUG: Dump out arguments:
		for (int i = 0; tokens[i] != NULL; i++) {
			write(STDOUT_FILENO, "   Token: ", strlen("   Token: "));
			write(STDOUT_FILENO, tokens[i], strlen(tokens[i]));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
		if (in_background) {
			//write(STDOUT_FILENO, "Run in background.", strlen("Run in background."));
		}



///     handle !!!!!
		if (strchr(tokens[0],'!')){
			// if !!
			if(tokens[0][1]=='!'){

				char cpyRecord[COMMAND_LENGTH];
				strcpy(cpyRecord,history[(hisIndex-1)%10]);
				strcpy(history[(hisIndex)%10],history[(hisIndex-1)%10]);
				//printf("%s %s\n","now displaying original ", history[(hisIndex-1)%10]);
				tokenize_command(cpyRecord ,tokens);


				hisIndex++;
			}
			else{

				int cmd;
				cmd=atoi(&tokens[0][1]);

				//if invald
				if(hisIndex<=10 && (cmd>hisIndex||cmd<=0) ){
						write(STDOUT_FILENO,"Invalid command", strlen("Invalid command"));
						write(STDOUT_FILENO, "\n", strlen("\n"));
						continue;
				}
				else if	(cmd<(hisIndex-9)||cmd>hisIndex){
					write(STDOUT_FILENO,"Invalid command", strlen("Invalid command"));
					write(STDOUT_FILENO, "\n", strlen("\n"));
					continue;
				}


				//if !n
				else{

						int dep=(cmd-1)%10;
						//printf("%s %d\n","displaying current depth ", dep);

						write(STDOUT_FILENO,history[dep], strlen(history[dep]));
						write(STDOUT_FILENO, "\n", strlen("\n"));

						if(hisIndex%10!=dep){
							char cpyRecord[COMMAND_LENGTH];
							strcpy(history[(hisIndex)%10],history[dep]);
							strcpy(cpyRecord,history[(dep)%10]);
							//printf("%s\n"," cpoed successfully " );
							tokenize_command(cpyRecord,tokens);
						}
						else{
							tokenize_command(history[dep],tokens);

						}

						hisIndex++;


				}
			}
		}

		if(strcmp(tokens[0],"exit") == 0){							// test for exit()
			isMine=true;
			exit(0);
		}

		if(strcmp(tokens[0],"pwd") == 0){
			isMine=true;
			char buf[500];
			char* workDir = getcwd(buf, sizeof(buf));
			write(STDOUT_FILENO, workDir, strlen(workDir));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}

		if(strcmp(tokens[0],"cd") == 0){
			isMine=true;
			int ret=chdir(tokens[1]);
			if(ret!= 0){
				write(STDOUT_FILENO,"No such directory.", strlen("No such directory."));
				write(STDOUT_FILENO, "\n", strlen("\n"));
			}
		}

		if (strcmp(tokens[0],"history")==0){
			isMine=true;
			// display History
			displayHistory(hisIndex);
		}




		/**
		 * Steps For Basic Shell:
		 * 1. Fork a child process
		 * 2. Child process invokes execvp() using results in token array.
		 * 3. If in_background is false, parent waits for
		 *    child to finish. Otherwise, parent loops back to
		 *    read_command() again immediately.
		 */

		 if(!isMine){  	// is a child process is needed
				pid_t pid;
	     	pid=fork();
				if(pid<0){
					 fprintf(stderr, "Fork Failed");
					 exit(-1);
			 	}
			 	else if(pid==0){ //child process
					execvp(tokens[0], tokens);
					exit(0);
			 	}
				else { /* parent process */
				 wait(NULL);
				}
		}



	}  //end while
	return 0;
}  //end main


void addHistory(char *command){

		//  if(command[strlen(command)]=='&'){
		// 	 int b;
		// 	 for (b=0;b<strnlen(command, COMMAND_LENGTH);b++){
		// 			history[(hisIndex)%10][b]=command[b];
		// 	 }
		// 	 history[(hisIndex)%10][b+1]='&';
		//  }

		// else{
			 //
			//  for(int a=0;command[a]!='\n';a++){
			// 	 history[(hisIndex)%10][a]=command[a];
			//  }
		 //}
		 //printf("%s%s\n", command ,"have added to history");
		//  char cpyRecord[COMMAND_LENGTH];
		//  for(int a=0;command[a]!='\0';a++){
		// 	 cpyRecord[a]=command[a];
		//  }
		 strcpy(history[(hisIndex)%10],command);

}

void displayHistory(int index){
	char msg[100];
	if(index<10){
		int roll=0;
		int count=1;
		for(;count<=index;roll++,count++){
			sprintf(msg,"%d	%s",count,history[roll]);
			write(STDOUT_FILENO,msg, strlen(msg));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
	}

	else {
		int count=index-9;
		for (int b=10;count<=index;b--,count++){
			sprintf(msg,"%d	%s",count,history[(index-b)%10]);
			write(STDOUT_FILENO,msg, strlen(msg));
			write(STDOUT_FILENO, "\n", strlen("\n"));
		}
	}
}
