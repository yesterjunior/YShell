/*必要的头文件包含*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<ctype.h>
#include<fcntl.h>
#include <signal.h>


/*宏定义*/
#define MAX_LINE 80 //最大命令长度
#define MAX_HISTORY 10  //最大历史命令存储长度
#define MAX_NAME 30 //最大用户名长度
#define END_COMMAND "exit" //退出命令


/*全局变量申明*/
char *args[MAX_LINE / 2 + 1];       //接收到的命令字符串
char *args2[MAX_LINE / 2 + 1];      //管道命令 # 右侧的字符串
char *args_in[MAX_LINE / 2 + 1];    //重定向输入的命令字符串
char *args_out[MAX_LINE / 2 + 1];   //重定向输出的命令字符串
enum function{NORMAL,IN,OUT,IN_OUT,PIPE,OTHER};//功能标识符{正常，<输入重定向命令，>输出重定向命令， #管道命令}，通过它进入不同的判断分支
enum function flag = NORMAL;        //功能标志
char *usr_name="-> ";               //命令提示符
char *usr_name_var="YSHELL_NAME";   //程序向环境提供的变量名
char *usr_name_val="->";            //程序向环境提供的变量内容
char *history[MAX_HISTORY][MAX_LINE / 2 + 1];//历史命令字符串数组
char cmd_buf[MAX_LINE];             //输入命令缓冲
int his_len;        //非空的历史命令
int bg=0;           //bg命令标志
int chYshellname=0; //修改提示符标志
int func_loc=0;     // <, > and # 命令的位置location
int func_in=0;      // < 符号在命令中的位置
int func_out=0;     // > 符号在命令中的位置
int sharp_num=0;    // # 符号咋命令中出现的次数
//进入Yshell的时候的欢迎横幅：
char *YIN_banner=" ____  ____  _            \n|_  _||_  _|(_)           \n  \\ \\  / /  __   _ .--.   \n   \\ \\/ /  [  | [ `.-. |  \n   _|  |_   | |  | | | |  \n  |______| [___][___||__] \n                          \n";


/*自己定义的函数*/
void welcome();                 //打印欢迎信息，带有颜色
void start();                   //接收命令并解析
int getcmd();                   //接收命令command
void parsecmd();                //解析命令command
int exec_function();            //执行 普通的命令 和含有<, >, # 的特殊命令
void mybg(pid_t pid);           //bg，切换进程到后台
void handle_SIGINT();           //处理ctl+C(SIGINT)信号
void goodbye();                 //打印退出信息，带有颜色 
void myjobs();                  //执行“jobs”对应的功能，查看正在运行的进程
void add_envValue();            //添加YSHELL_NAME为局部环境变量
int itoa(char *des, int i);     //转换int到array(为了更加好看的UI)
void append_history();          //添加命令到历史字符串数组

/* 主函数 */
int main()
{
    signal(SIGINT,handle_SIGINT);
    add_envValue();
    his_len =0;
    welcome();//欢迎信息
    for (;;)
    {
        bg=0;
        chYshellname=0;
        func_loc=0;// <, > and # 命令的位置location
        func_in=0;// < 符号在命令中的位置
        func_out=0;// > 符号在命令中的位置
        usr_name=getenv(usr_name_var);
        printf("\e[34m%s\e[0m",usr_name);

        /* 等待输入要并解析为args */
		start();
        if (args[0] == NULL)    /* input only \n */
			continue; 
        
        /* fork一个子进程*/
		pid_t pid = fork();
        if(pid<0){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        if(pid==0){
            int thispid=getpid();//获取子进程pid
            signal(SIGINT,SIG_DFL);//默认是终止进程
			signal(SIGTSTP,SIG_DFL);//默认是暂停进程
			signal(SIGCONT,SIG_DFL);//默认是继续这个进程
            if(bg==1){//如果命令需要在后台执行
				printf("Yshell: in background: the job's pid: [%d]\n",thispid);
				exec_function();
				return 0;
			}
            exec_function();//执行主要命令
            break;
        }
        else if(pid>0){
        signal(SIGTSTP,SIG_IGN);//忽视信号
		signal(SIGCONT,SIG_DFL);//默认是继续
		    if(bg==1){//如果命令要在后台执行
			    signal(SIGCHLD,SIG_IGN);//忽视SIGCHLD信号
		    }
		    else{
			    waitpid(pid,NULL,WUNTRACED);//后台执行的关键代码，必须用WUNTRACED
		    }
        }
        else{
				perror("Yshell: fork");
				break;
		}
        for (int i = 0; args[i] != NULL; i ++) {    /* 释放内存 */
			free(args[i]);
			args[i] = NULL;
		}
    }
    return 0;
}

/* 进入shell时候打印欢迎和友好的提示信息 */
void welcome(){
	//如下是欢迎信息
	//为了是程序更友好，加入了颜色
	//颜色是紫色，背景色与shell相同
	printf("\e[1;36m============Welcome to myshell===============\033[0m\n");
	printf("\e[1;36m        YShell(YinXiaojie's Shell)           \e[0m\n");
    printf("%s",YIN_banner);
	printf("\e[1;36m=================Have Fun====================\e[0m\n");
    
}

/* 离开shell时候打印goodbye和友好的提示信息 */
void goodbye(){
    //为了是程序更友好，加入了颜色
	printf("\e[1;36m==================GoodBye====================\e[0m\n");
	sleep(1);//暂停1s，看上去视觉效果好一些
	exit(0);
}

/* 处理中断信号SIGINT，使ctrl+c成为“打印命令输入历史” 功能的快捷键 */
void handle_SIGINT(){
	char buf[MAX_LINE + 20];
	int top = 0;
	for (int i = 0; i < his_len && i < MAX_HISTORY; i++) {
		top = 0;
		buf[top++] = '\n';
		int num = (his_len > MAX_HISTORY) ?
			  (his_len - 9 + i) :
			  i + 1;
		top += itoa(buf + 1, num);      //给历史输出命令加上1. 2. 等行标
		buf[top++] = '.';
		buf[top++] = ' ';
		for (int j = 0; history[i][j] != NULL; j++) {
			int len = strlen(history[i][j]);
			memcpy(buf + top, history[i][j], len);
			top += len;
			buf[top++] = ' ';
		}
		write(STDOUT_FILENO, buf, top);
	}
	top = 0;            
	buf[top++] = '\n';
	write(STDOUT_FILENO, buf, top); //输出历史命令到标准输出并换行
}

/* 把行标int转换为des字符串 */
int itoa(char *des, int i)
{
	char *s = des;
	do {
		*s++ = i % 10 + '0';
		i /= 10;
	} while (i > 0);
	int res = s - des;
	s--;
	while (des < s) {
		char tmp = *des;
		*des = *s;
		*s = tmp;
		des++;
		s--;
	}
	return res;
}

/* bg，切换进程到后台 */
void mybg(pid_t pid){
	if(kill(pid,SIGCONT)<0){//发送SIGCONT信号
		printf("Yshell: bg: no such job\n");//如果有错就打印提示信息
	}
	else{
		waitpid(pid,NULL,WUNTRACED);//和myfg()一样，必须用WUNTRACED
	}
}

/* 给环境变量添加自定义的局部环境变量YSHELL_NAME */
void add_envValue(){
    //Add a value to env
    //printf("添加YSHELL_NAME为局部环境变量\n");
    usr_name_val = getenv(usr_name_var);
    if (!usr_name_val)
    {
        printf("Yshell:Lucky! There is no value named with %s\n", usr_name_var);
    }
    char c1[MAX_NAME] = "YSHELL_NAME";
	char c2[MAX_NAME] = "->";
    if (setenv(c1,c2,0) == 0)       // putenv()仅仅只能起效一次, 所以我改为了setenv()
    {
        printf("Yshell:Successful to add %s\n",usr_name_var);
    }
    else
    {
        printf("Yshell:Failed to add %s\n",usr_name_var);
        exit(EXIT_FAILURE);
    }
    usr_name_val = getenv(usr_name_var);
    if (usr_name_val)
    {
        printf("[%s]: %s\n", usr_name_var, usr_name_val);
    } 
    //注意：这个新增的环境变量仅仅对这个程序本身有效
    //这是因为变量的值不会从子进程（本程序）传播到父进程（Shell）
}

/* 通过getcmd()开始读取命令输入，并且发送给parsecmd进行解析 */
void start(){       
    if (getcmd() == -1){    /* -1 代表仅输入了 \n */
        args[0]= NULL;
        return;
    }
    /* 设置 命令历史 */
	
    parsecmd();
}

/* 读取命令输入 */
int getcmd(){
    flag=NORMAL;
    int c;
    int len=0;
    memset(cmd_buf,0,sizeof(cmd_buf));  /* 初始化buf */
    rewind(stdin);
    c=getchar();
    while(len+1<sizeof(cmd_buf) && c!='\n' && c!=EOF){
        cmd_buf[len++]=c;
        c=getchar();
    }
    if(len==0){
        if (c=='\n'){       /* 仅输入了 \n */
            return -1;
        }
        printf("Yshell will close with EOF...\n");
        goodbye();
        exit(EXIT_SUCCESS); /* 退出Yshell的办法1：EOF(ctl-D) */
    }
    return 0;
}


/* 解析输入的命令，如果有特殊命令< , > ,#的话则打上 标识符flag */
void parsecmd(){
    char *p,*q,*t1,*t2;     /* p q指针用来记录命令对应 的字符， t1，t2用来记录含有=，即修改提示符时候的名称字符 */
    int i=0;
    p=q=t1=t2=cmd_buf;
    while(*q++!='\0'){
        if(*q == '='){     //=代表命令提示符被修改和自定义
            t1=t2 =q;
            chYshellname=1;
            while(*t2++!='\0'){
                //只是为了计数而已
                continue;
            }
            args2[0]=(char *) malloc(sizeof(t2 - t1 + 1));
            int k=0;
            t1++;
            while(t1!=t2)
                args2[0][k++]=*t1++;
            args2[0][k]='\0';
        }
        if (*q != ' ')  /* 在一个词语中 */
			continue;
		args[i] = (char *) malloc(sizeof(q - p + 1));
		int j = 0;
		while (*p != ' ')
			args[i][j++] = *p++;
		args[i++][j] = '\0';
		while (*q == ' ')   /* 忽视多个空格连在一起的情况 */
			*q++;
		p = q;
        t1 =q;
        t2 =q;
    }
    if (*p != ' ' && *p != '\0') {
		args[i] = (char *) malloc(sizeof(q - p + 1));
		int j = 0;
		while (p != q)
			args[i][j++] = *p++;
		args[i++][j] = '\0';
	}

    append_history();

	if (strcmp(args[0],"quit") == 0) {
        goodbye();
	    exit(EXIT_SUCCESS);	    /* 退出Yshell的办法2：输入 quit */
	}

    else if (strcmp(args[0],"bg")==0){
        pid_t pid_bg;
        if(args[1]!=NULL){
            pid_bg=atoi(args[1]);   //用atoi转换，获取pid
        }
        else{
            printf("Yshell: bg: no job assigned\n");
        }
        char *temp_arg[MAX_LINE / 2 + 1];
        for(int y=0;y<i-1;y++){
            temp_arg[y]=args[y+1];
            args[y]=temp_arg[y];
        }
        i--;
        flag=NORMAL;
        mybg(pid_bg);
    }
    else if(strcmp(args[0],"jobs")==0){//jobs，查看运行的进程
        if(args[1]!=NULL){
            printf("Yshell: jobs: incorrect use of jobs\n");
        }
        else
            myjobs();
    }

    else if (chYshellname==1) {
	    if(args[1]!=NULL){
            printf("Yshell: chYshellname:incorrect input of chYshellname\n");
        }
        else {
            printf("Yshell: Username has been changed into %s.\n",args2[0]);
            //printf("args2[0]=%s\n",args2[0]);
            setenv("YSHELL_NAME",args2[0],1);      //修改提示符变量YSHELL_NAME
        }
	}
 
    
    if (strcmp(args[i - 1], "&") == 0) {    /* bg功能 */
		bg = 1;
        args[i] = NULL;
		//free (args[--i]);     //必须减掉1，因为&已经被替换了,但是通过free (args[--i])会使得原本的ls & 命令变成ls ls 命令，就很迷，难道free会复制前一个指针的内容吗？
        args[--i] = NULL;       //必须减掉1，因为&已经被替换了，使用free不行只能用NULL了；
	}
    

    /* 检测 >输入, <输出和 #管道功能*/
    int n=0;
    sharp_num=0; //用来计数#出现的次数
    for (; args[func_loc] != NULL; ++func_loc )
    {
        if (strcmp("<", args[func_loc]) == 0)
        {
            func_in=func_loc;
            flag = IN;
            i=func_loc;
            args[i] = NULL;
            continue;
        }

        if (strcmp(">", args[func_loc]) == 0)
        {
            func_out=func_loc;
            if(flag==NORMAL)
                flag = OUT;
            else
                flag =IN_OUT;
            i=func_out;
            args[i] = NULL;
            continue;
        }

        else if (strcmp("#", args[func_loc]) == 0)
        {
            if(sharp_num>=1){
                flag=OTHER;
                continue;   //暂时不去处理多个#管道符号的命令
            }
            flag = PIPE;
            for(n=func_loc+1;n<i;n++){
                args2[n-func_loc-1]=args[n];
            }
            args2[n-func_loc-1]=NULL;
            i=func_loc;
            args[i] = NULL;
            sharp_num++;
            continue;
        }
    }

    for(int in=0;in<=func_in+1;in++){       /* < */
            args_in[in]=args[in];
    }
    if(flag ==IN_OUT){
        func_out=func_loc-func_in-2;
        for(int out=0;out<=func_loc-func_in-2 ;out++){        /* > */
            args_out[out]=args[func_in+3+out];
        }
        args_out[func_loc]=NULL;
    }
    else if(flag ==OUT){
        for(int out=0;out<=func_out+1;out++) {/* 有时候会有 <  file  > 两个符号连用的情况 */  
            args_out[out]=args[out];
        }            
    }
}


/* 模拟jobs功能 */
void myjobs(){
	//可以使用ps命令来实现查看进程
	pid_t pid;
	pid=fork();//必须fork，否则会出现myshell退出这种奇怪的bug
	if(pid<0){
		perror("myshell: fork");
	}
	else if(pid==0){//子进程	
		execlp("ps","ps","ax",NULL);//使用ps
        // execlp("jobs","jobs",NULL);//使用jobs不可见父类进程创建的job，所以应该用ps
	}
	else{//父进程
		waitpid(pid,NULL,0);
	}
}

/* 添加命令到历史字符串数组 */
void append_history()
{
	int i, j;
	if (++his_len > MAX_HISTORY) {	/* move history entries to their next */
		for (j = 0; history[0][j] != NULL; j++)
			free(history[0][j]);
		for (i = 0; i < MAX_HISTORY - 1; i++)
			for (j = 0; history[i + 1][j] != NULL; j++)
				history[i][j] = history[i + 1][j];
	} else {
		i = his_len - 1;
	}
	for (j = 0; args[j] != NULL; j++)
		history[i][j] = strdup(args[j]);
	history[i][j] = NULL;
}

/* 执行 普通的命令 和含有<, >, # 的特殊命令 */
int exec_function(){
    int fd=0;   /* for output and input ID*/
    int fd2=0;
    int pid_pipe=0; //pipe 用的子进程
    int isnormal=0;
    switch(flag)
    {
    //普通的命令
    case NORMAL:
        isnormal=execvp(args[0], args);	
        if(isnormal!=0){
            printf("Yshell: Sorry,I have NO idea of your command!\n");
        }
        break;

    //带有<的命令
    case IN: //printf("Get '<'\n"); 
        if (args_in[func_in+1] == NULL)
        {
            printf("Command Error:Input Target file needed!\n");
            exit(1);
        }
        fd = open(args_in[func_in+1], O_RDONLY);
        if(fd < 0)
        {
            printf("Open Error when RDONLY with '<'\n");
            return -1;
        }
        fd2 = dup2(fd,STDIN_FILENO);
        if(fd2<0)
        {
            printf("Dup2 Error when STDIN with '<' \n");
            return -1;
        }
        //执行命令
        isnormal=execvp(args_in[0], args_in);
        if(isnormal!=0){
            printf("Yshell: Sorry,I have NO idea of your command!\n");
        }
        //清空每一次输入输出重定向的内容：
        fd=0;
        fd2=0;    
        //close
        if(close(fd)<0){
            perror("YShell：Close Error.");
            exit(1);
        }
        else{
            break;
        }
        
    //带有>的命令       
    case OUT: //printf("Get '>'\n"); 
        if (args_out[func_out+1] == NULL)
        {
            //printf("%d",func_out);
            printf("Command Error:Output Target file needed!\n");
            exit(1);
        }
        fd = open(args_out[func_out+1], O_WRONLY|O_CREAT|O_TRUNC,0666);
        if(fd < 0)
        {
            printf("Open Error when O_WRONLY|O_CREAT|O_TRUNC with '>'\n");
            return -2;
        }
        fd2 = dup2(fd,STDOUT_FILENO);
        if(fd2<0)
        {
            printf("Dup2 Error when STDOUT with '>' !\n");
            return -2;
        }
        //执行命令
        isnormal=execvp(args_out[0], args_out);
        if(isnormal!=0){
            printf("Yshell: Sorry,I have NO idea of your command!\n");
        }
        //清空每一次输入输出重定向的内容：
        fd=0;
        fd2=0;    
        //close
        if(close(fd)<0){
            perror("YShell：Close Error.");
            exit(1);
        }
        else{
            break;
        }

    //同时有 < > 两种符号的情况
    case IN_OUT: //printf("Get '>' and '<' both.\n");    
        fd = open(args_in[func_in+1], O_RDONLY);
        if(fd < 0)
        {
            printf("Open Error when RDONLY with '<' and '>' ,'%s' is NOT readable!\n",args_in[func_in+1]);
            return -3;
        }
        fd2 = dup2(fd,STDIN_FILENO);
        if(fd2<0)
        {
            printf("Dup2 Error when STDIN with '<' and '>' !\n");
            return -3;
        }
        //执行命令
        isnormal=execvp(args_in[0], args_in);
        if(isnormal!=0){
            printf("Yshell: Sorry,I have NO idea of your command!\n");
        }
        isnormal=execvp(args_out[0], args_out);
        if(isnormal!=0){
            printf("Yshell: Sorry,I have NO idea of your command!\n");
        }
        //清空每一次输入输出重定向的内容：
        fd=0;
        fd2=0;    
        //close
        if(close(fd)<0){
            perror("YShell：Close Error.");
            exit(1);
        }
        else{
            break;
        }

    //带有#的管道命令
    case PIPE: //printf("Get '#'\n");   
        pid_pipe=fork();
        if(pid_pipe==0)
        {
            fd2=open("tempfile",O_WRONLY|O_CREAT|O_TRUNC,0600);
            dup2(fd2,STDOUT_FILENO);
            if(fd2<0)
            {
                printf("Dup2 Error when STDOUT with '#'!\n");
                return -4;
            }
            
            isnormal=execvp(args[0], args);
            if(isnormal!=0){
                printf("Yshell: Sorry,I have NO idea of your command!\n");
            }
            //close
            if(close(fd2)<0){
                perror("YShell：Close Error.");
                exit(1);
            }
        }else{
            waitpid(pid_pipe,NULL,0);
            fd=open("tempfile",O_RDONLY);
            if(fd < 0)
            {
                printf("Open Error when RDONLY with '#'\n");
                return -4;
            }
            dup2(fd,STDIN_FILENO);
            if(fd2<0)
            {
                printf("Dup2 Error when STDIN with '#'!\n");
                return -4;
            }
            isnormal=execvp(args2[0], args2);
            if(isnormal!=0){
                printf("Yshell: Sorry,I have NO idea of your command!\n");
            }
            //close
            if(close(fd)<0){
                perror("YShell：Close Error.");
                exit(1);
            }
        }
        break;

    case OTHER: //目前无法支持的功能   
        printf("\e[1;36mYshell: I'm SORRY this program CAN'T support this command. \nBut I will improve this part of the function in the future work. \nTo Be Continued...\n\033[0m\n");
        break;
    }
    return 0;
}