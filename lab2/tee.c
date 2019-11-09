#include<string.h>
#include<errno.h>
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
int parse(char *line,char **argv)
{
	int i=0;
	int j=0;
	while (*line != '\0') 
	{   
		if(*line=='\n')
		{	
			*line++='\0';
			return i;
		}
		while (*line == ' ' || *line == '\t')
		{
            *line++ = '\0';
			if(*line=='\n')
			{	
				*line++='\0';
				return i;
			}
		}
		*argv++= line;
		i++;
        while (*line != '\0' && *line != ' ' && *line != '\t' )
		{
			line++;
			if(*line=='\n')
			{	
				*line++='\0';
				return i;
			}
		}
    }
    *argv = '\0';  
	return i;
}

void tee(char **fname,int num,char **args)
{
	int a[2];
	if(pipe(a)==-1)
		printf("%s",strerror(errno));
	int pid=fork();
	if(!pid)
	{
		close(a[0]);
		close(1);
		dup2(a[1],1);
		if(execvp(*args,args)<0)
		{
			fprintf(stderr,"command not found");
			int errnum = errno;
			fprintf(stderr, "Error %d : %s\n",errnum, strerror( errnum ));
		}
		close(a[1]);
		
		exit(0);
	}
	else
	{
		
		close(a[1]);
		FILE *logfile[10];
		for(int i=0;i<num;i++)
		{
			logfile[i]=fopen(fname[i],"w");
			if(!logfile[i])
				printf("fileopen error\n");
		}
		char ch;
		printf("parent listening: \n");
		while(read(a[0],&ch,1)>0)
		{
			putchar(ch);
			int i=0;
			for(int i=0;i<num;i++)
				fputc(ch,logfile[i]);
		}
		putchar('\n');
		for(int i=0;i<num;i++)
		{
			fputc('\n',logfile[i]);
			fclose(logfile[i]);
		}
		close(a[0]);
		int status;
		wait(&status);
		exit(0);
	}
}
int main(int argc,char **argv)
{
	if(argc==1)
	{
		printf("error in arguments propoer format: COMMAND ARGS tee FILENAME1 FILENME2");
		exit(0);
	}
	int i=0;
	char argsl[1000]={'\0'};
	char fnamel[1000]={'\0'};
	char *args[100]={'\0'};
	char *fname[100]={'\0'};
	for(i=1;i<argc&&(strcmp(argv[i],"tee")!=0)&&(strcmp(argv[i],"TEE")!=0);i++)
	{
		strcat(argsl,argv[i]);
		strcat(argsl,"\t");
	}
	strcat(argsl,"\n");
	int x=i;
	for(i=x+1;i<argc;i++)
	{
		strcat(fnamel,argv[i]);
		strcat(fnamel,"\t");
	}
	strcat(fnamel,"\n");
	int ano=parse(argsl,args);
	int fno=parse(fnamel,fname);
	tee(fname,fno,args);
}