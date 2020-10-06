/*
cp
*/
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char  *argv[])
{
	int pipefd[2];
    pipe(pipefd);
    char *read="hola soy eray";
    write(pipefd[1],read,1024);
    char*out[1024];
    read(pipefd[0],out,1024);
    write(1,out,1024);
	return 0;
}