#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
void main(){
	int pid;
	pid=fork();
	char *path="/bin/date";
	char *const argv[]={"/bin/date",NULL};
	if(pid==0){ //Ejecuta proceso hijo
		execv(path,argv);
	}else{
		exit(0);
	}
}
