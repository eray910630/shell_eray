#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#define maximo 256
#define maxComm 40


char entrada[maximo];
char direccion[maximo];
char comand[maximo];
char params[maximo];
//char paramsotro[maximo];
char *string[maximo];
int tam;
int algoenlinea;


int get_comand(int ptr)
{
	while(entrada[ptr] == ' ')
		ptr++;
	int c = 0;
	while(entrada[ptr] != ' ' && ptr < tam)
		comand[c++] = entrada[ptr++];
	 comand[c] = '\0';
	
	while(entrada[ptr] == ' ' && ptr < tam)
		ptr++;
	return ptr;
} 

int get_params(int ptr)
{
	int c = 0;
	while(entrada[ptr] != ' ' && ptr < tam)
	{
		params[c++] = entrada[ptr++];
	}
	params[c] = '\0';
	return ptr;
}

int count_Pipe()
{
	int count=0;
	int pos=0;
   while(pos < tam)
   {
     if(entrada[pos++]=='|') count++;
   }
   return count;
}
int HayMayor()
{
  int pos=0;
  while(pos < tam)
  {  	  
      if(entrada[pos]=='>')
      {
      	if(entrada[pos + 1]=='>')
      	{    
      		break;     		
      	}
      	return 1;
      }
     pos++;
  }
  return 0;

}

int HayMenor()
{
	int pos=0;
	while(pos < tam)
	{
		if(entrada[pos]=='<')return 1;
		pos++;
	}
	return 0;
}

int HayMayorMayor()
{
	int pos=0;
	while(pos < tam)
	{
		if(entrada[pos]=='>' && entrada[pos + 1]=='>')return 1;
		pos++;
	}
	return 0;
}

int HayAlgoEnLaEntrada()
{

   algoenlinea=0;
   int pos=0;
   while(pos < tam)
   {
   	  if(entrada[pos]!=' ')algoenlinea++;
   	  pos++;
   }
   return algoenlinea;
}


 void SplitString()
 {
 	 int ptr=get_comand(0);
 	 string[0]=comand; 	  	  	
 	 int k=1; 
 	 
 	 while(ptr < tam)
 	 {
       while(entrada[ptr] == ' ')
		ptr++;        
       
       string[k++]=&entrada[ptr];         
	   while(entrada[ptr] != ' ' && ptr < tam)
	    {
		  ptr++;		  
	    }	    
	    entrada[ptr] = '\0';
	    ptr++;   
	}
	string[k]=NULL;		
 }
void Ejecuta()
{
	int mayor=HayMayor();
	int menor=HayMenor();
	int maxmax=HayMayorMayor();
	int pipes=count_Pipe();
    
	if(mayor==0 && menor==0 && maxmax==0 && pipes==0)
	{
		SplitString();        
		pid_t pid;
		int status=0;
		pid=fork();
		if(pid==0)
		{
			if (execvp(string[0],string)==-1)
			{
				printf("Error en los argumentos\n");
			}		
		}
		wait(&status);
	}
	else if (mayor==1) 
	{
		SplitString();
		int i=0;
		int k=0;
		while(string[i]!=NULL) //esto era para saber la pos del >
		{			
            if(strcmp(string[i],">")==0)
            {
            	k=i;            	           	
            }
			i++;
		}
		
		int fd=open(string[i - 1],O_RDWR | O_CREAT,0644);                
		int status=0;
		pid_t pid;
		pid=fork();         

		if(pid==0)
		{		
			 i=0;
             while(strcmp(string[i],">")!=0)
             {               
                  i++;
             }              
            string[i]=NULL;
			dup2(fd,1);			
			close(fd);
			if(execvp(string[0],string) == -1)
			{
			  printf("invalid command\n");			  
			}
		}
		close(fd);
		wait(&status);
		
	}
	else if(menor==1)
	{
		SplitString();
		int i=0;
		int k=0;
		while(string[i]!=NULL) //esto era para saber la pos del <
		{			
            if(strcmp(string[i],"<")==0)
            {
            	k=i;            	           	
            }
			i++;
		}
		
		int fd=open(string[i - 1],O_RDONLY);                
		int status=0;
		pid_t pid;
		pid=fork();         

		if(pid==0)
		{		
			 i=0;
             while(strcmp(string[i],"<")!=0)
             {               
                  i++;
             }              
            string[i]=NULL;
			dup2(fd,0);			
			close(fd);
			if(execvp(string[0],string) == -1)
			{
			  printf("invalid command\n");			  
			}
		}
		close(fd);
		wait(&status);		
	}
	else if (maxmax == 1)
	{
		SplitString();
		int i=0;
		int k=0;
		
		while(string[i]!=NULL) //esto era para saber la pos del >>
		{			
            if(strcmp(string[i],">>")==0 )
            {
            	k=i;            	            	           	
            }
			i++;
		}
		
		int fd=open(string[i - 1],O_RDWR | O_APPEND | O_CREAT,0644);                
		int status=0;
		pid_t pid;
		
		pid=fork();
		if(pid==0)
		{		
			 i=0;
             while(strcmp(string[i],">>")!=0)
             {       
              
                 i++; 
             }              
            string[i]=NULL; 
			dup2(fd,1);			
			close(fd);
			if(execvp(string[0],string) == -1)
			{
			  printf("invalid command\n");			  
			}
		}
		close(fd);
		wait(&status);	
	}
	else
	{
		SplitString();
		int i=0;		
		char*primerParametro[maximo];
		char*segundoParametro[maximo];

		while(string[i][0]!='|') 
		{ 
            primerParametro[i]=string[i];
			i++;
		}
		primerParametro[i]=NULL;
		i++;
		int  p=0;
		while(string[i]!=NULL) 
		{ 
            segundoParametro[p]=string[i];
			i++;
			p++;
		}
		segundoParametro[p]=NULL;
		      
		int status=0;
		pid_t pid;

		int fds[2];
		pipe(fds);
		
		pid=fork();
		if(pid==0)
		{  
			dup2(fds[1],1);			
			close(fds[0]);
			if(execvp(primerParametro[0],primerParametro) == -1)
			{
			  printf("invalid command\n");			  
			}
		}
		pid=fork();
		if(pid==0)
		{ 
			dup2(fds[0],0);			
			close(fds[1]);
			if(execvp(segundoParametro[0],segundoParametro) == -1)
			{
			  printf("invalid command\n");			  
			}
		}
		close(fds[0]);
		close(fds[1]);
		wait(&status);
		wait(&status);	
	}
  
}


int main(int argc, char *argv[])
{
	printf("%s\n","******************************************" );
	printf("%s\n", "*****            Integrantes  ************");
	printf("%s\n","*****    Juan Eray Alvarez Hernandez  *******" );
	printf("%s\n","****     Daher                    *************" );
	printf("%s\n","******************************************" );
	
	
	while(1)
	{ 
	  int i;
	  int espacio;	  
	  getcwd(direccion,maximo);//coge la direccion donde esta el  tipo actual y es la que imprimo
	 

	  printf("%s ~$",direccion);
	  fflush(NULL);
      fgets(entrada,maximo,stdin);
      if(strlen(entrada)==0) continue;      
      tam=strlen(entrada);
      tam--;
      entrada[tam]='\0'; //aki le pongo al final 0 para ajustar la linea
      HayAlgoEnLaEntrada();//para el comando distinto de cd y exit

      int ptr = get_comand(0);//esto es para saber el comando que voy a leer donde tengo una variable llamada comand donde guardo el comando y devuelve un int para saber por donde voy
      if(strcmp("exit",comand)==0)
      {
      	break;
      }
      ptr = get_params(ptr); //voy a tener en params los valores ya parseados puntero es un int que se ira modificando

	  if (strcmp("cd",comand) == 0)
	  {
	  	   if(chdir(params) == -1)
	  	   {
	  	   		printf("Error en los argumentos\n");
	  	   		continue;
	  	   }
	  }
	  else if(algoenlinea > 0)
	  {
	  	  Ejecuta();	  	
	  }    

	}
	return 0;
}