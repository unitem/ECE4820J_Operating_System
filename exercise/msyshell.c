#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<sys/stat.h>
#include<fcntl.h>
//这里的全局变量暂时还消不掉，因为如果用的是局部变量，那么在调用完返回的时候就被销毁了，但是其实是不能销毁的
char op[100];
char t[100],p[100],r[100],q[100];

struct command {
    int argc;
    char *argv[10];
    char *input;
    char *output;
    int mode;
};

int split(char *line, char *separator, char *word_array[])
{
    int word_count=0;
    char *word=NULL;

    word=strtok(line,separator);
    while(word) 
    {
        word_array[word_count]=word;
        word_count++;
        word=strtok(NULL,separator);
    }
    word_array[word_count]=NULL;
    return word_count;
}

void parse_command(char *command,struct command *order)
{	
	//define&init
	int st[10];
	memset(st,0,sizeof(st));
	order->mode=0;
	order->input=NULL;
	order->output=NULL;
	
	//split
	char *argv[10];
	int argc=split(command," ",argv);

	//test
	/*for(int i=0;i<argc;i++)
	{
		printf("%s ",argv[i]);
		fflush(stdout);
	}
	printf("\n");*/

	//search < > >>
	for(int i=1;i<argc;i++)
	{
		strcpy(op,argv[i]);
		//printf("[%s]\n",op);
		//fflush(stdout);
		if(op[0]=='<')
		{
			int k=0;
			for(int j=1;op[j]!='\0';j++)
				r[k++]=op[j];
			r[k]='\0';
			if(r[0]!='/')
			{
				getcwd(q,sizeof(q));
				strcat(q,"/");
				strcat(q,r);
				order->input=q;
			}
			else
				order->input=r;
			st[i]=1;
			//printf("1:%s\n",input);
		}
		else if(op[0]=='>')
		{
			int k=0;
			for(int j=1;op[j]!='\0';j++)
			{
				if(op[j]=='>') continue;
				t[k++]=op[j];
			}
			t[k]='\0';
			//printf("[%s]\n",t);
			//fflush(stdout);
			if(t[0]!='/')
			{
				getcwd(p,sizeof(p));
				//printf("[%s]\n",p);
				//fflush(stdout);
				strcat(p,"/");
				strcat(p,t);
				//printf("[%s]\n",p);
				//fflush(stdout);
				order->output=p;
				//printf("-1\n");
				//fflush(stdout);
			}
			else
				order->output=t;
			if(op[1]=='>')
				order->mode=1;
			//printf("[%s]\n",output);
			//fflush(stdout);
			st[i]=1;
		}		
	}
	int k=0;
	for(int i=0;i<argc;i++)
	{
		if(!st[i])
			order->argv[k++]=argv[i];
	}
	order->argv[k]=NULL;
	order->argc=k;
	//printf("k:%d\n",k);
	//printf("2:%s\n",input);*/
}
void dump_command(struct command *order)
{
	printf("%d\n",order->argc);
	for(int i=0;i<order->argc;i++)
		printf("%s ",order->argv[i]);
	printf("\n");
	//printf("-1\n");
	//fflush(stdout);
	if(order->input!=NULL)
		printf("input:%s\n",order->input);
	else
		printf("input:NULL\n");
	//printf("0\n");
	//fflush(stdout);
	if(order->output!=NULL)
		printf("output:%s mode:%d\n",order->output,order->mode);
	else
		printf("output:NULL\n");
}

void redirect(struct command *order)
{
	int fd1,fd2;
	if(order->input!=NULL)
	{
		fd1=open(order->input,O_RDONLY);
		if(fd1==-1)
		{
			printf("input fail open!\n");
			exit(0);
		}
		dup2(fd1,0);
	}
	if(order->output!=NULL)
	{
		//printf("%s\n",output);
		if(order->mode==0)
			fd2=open(order->output,O_CREAT|O_TRUNC|O_WRONLY,0666);
		else
			fd2=open(order->output,O_CREAT|O_APPEND|O_WRONLY,0666);
		if(fd2==-1)
		{
			printf("output fail open\n");
			exit(0);
		}
		dup2(fd2,1);
	}	
}
void mysys(char *command)
{
	struct command order;
    parse_command(command, &order);
	
	//dump_command(&order);

	if(strcmp(order.argv[0],"cd")==0)
		chdir(order.argv[1]);
	else if(strcmp(order.argv[0],"exit")==0)
		exit(0);
	else if(strcmp(order.argv[0],"pwd")==0)
	{
		char path[255];
		getcwd(path,sizeof(path));
		puts(path);
	}
	else
	{
		pid_t pid;
		pid=fork();
		if(pid==0)
		{
			redirect(&order);    //redirect
			int error=execvp(order.argv[0],order.argv);
			if(error<0)
				perror("execv");
		}
		wait(NULL);
	}
}
int main()
{
	char str[100];
	while(1)
	{
		printf(">");
		gets(str);
		mysys(str);
	}
	return 0;
}
