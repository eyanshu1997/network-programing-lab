#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<time.h>
int *pid;
int n;
struct s { long type; char text[100]; int pid;};
void sigquit();
void child(struct s mymsg,int mq_id)
{
	while(1)
	{
		memset(mymsg.text, 0, 100);
		int received;
		received = msgrcv(mq_id, &mymsg, sizeof(mymsg), getpid(), 0);
		memset(mymsg.text, 0, 100);
		int x=(rand() *getpid() );
		if(x<0)
			x=x*-1;
		int c= x%2;
		if(c==0)
			c=1;
		else
			c=0;
		char coin[2]={'\0'};
		printf("child %d generated random no %d %d\n",getpid(),x,c);
		sprintf(coin,"%d",c);
		strcpy(mymsg.text,coin );
		mymsg.type = 1;
		mymsg.pid=getpid();
		msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);
	}
	exit(0);
}
int check(int * pid,int no,int s)
{
	int i=0;
	for(int i=0;i<no;i++)
		if(pid[i]==s)
			return i;
	return -1;
}
int main (int argc, char*argv[]) 
{
	if(argc!=2)
	{
		printf("error in args");
		exit(0);
	}
	n=atoi(argv[1]);
	printf("no of child is %d\n",n);
	key_t ipckey;
	int mq_id;
	struct s mymsg;
	ipckey = ftok("/tmp/foow", 42);
	mq_id = msgget(ipckey, IPC_CREAT | 0666);
	pid=(int *)malloc(n*sizeof(int));
	int i=0;
	for(i=0;i<n;i++)
	{
		pid[i]=fork();
		if(pid[i]==0)
		{
			child(mymsg,mq_id);
		}
		else
		{
			signal(SIGINT,sigquit);
		}
	}
	while(1)
	{
		int *recv=(int *)malloc(n*sizeof(int));
		int *pi=(int *)malloc(n*sizeof(int));
		for(i=0;i<n;i++)
		{
			memset(mymsg.text, 0, 100); 
			strcpy(mymsg.text, "Hello, world!");
			mymsg.type = pid[i];
			mymsg.pid=getpid();
			msgsnd(mq_id, &mymsg, sizeof(mymsg), 0);
		}
		for(i=0;i<n;i++)
		{
			int received;
			memset(mymsg.text, 0, 100);
			received = msgrcv(mq_id, &mymsg, sizeof(mymsg), 1, 0);
			printf(" server recieved from child %d :messgae recieved :%s \n",mymsg.pid, mymsg.text);
			recv[i]=atoi(mymsg.text);
			pi[i]=mymsg.pid;
		}
		printf("server recieved responses\n");
		int count1=0;
		int count0=0;
		for(i=0;i<n;i++)
		{
				printf("%d : %d \n",pi[i],recv[i]);
				if(recv[i]==1)
					count1++;
				else
					count0++;
		}
		if(count1>count0)
			printf("1 Selected by server\n");
		else
			printf("0 Selected by server\n");
		sleep(1);
	}
}
void sigquit(int sig) 
{ 
	printf("Read complete killing all child\n");
	int i=0;
	for(i=0;i<n;i++)
	{
		kill(pid[i], SIGQUIT);
	}
	exit(0);
}
