# YSHELL 测试报告

[toc]



> docs/中所有markdown所引用的**图片**已经托管至**在线的图床服务器**，如果您想有良好的阅读体验请保证您的计算机能够联网。

## 一、启动Yshell

### ./Yshell


```
./Yshell
```

[![Ruqb2d.png](https://z3.ax1x.com/2021/06/23/Ruqb2d.png)](https://imgtu.com/i/Ruqb2d)



```bash
yin@ubuntu:/mnt/hgfs/Shared/yshell$ ./Yshell
Lucky! There is no value named with YSHELL_NAME
Successful to add YSHELL_NAME
[YSHELL_NAME]: ->
============Welcome to myshell===============
        YShell(YinXiaojie's Shell)           
 ____  ____  _            
|_  _||_  _|(_)           
  \ \  / /  __   _ .--.   
   \ \/ /  [  | [ `.-. |  
   _|  |_   | |  | | | |  
  |______| [___][___||__] 
                          
=================Have Fun====================
->
```

启动Yshell之后，

1. 进入`main()`, `add_envValue()`函数成功运行，提示“`Lucky! There is no value named with YSHELL_NAME`”和“`Successful to add YSHELL_NAME`” 表示添加`YSHELL_NAME`到环境变量成功，如上图（如下代码片）。并且提示当前`[YSHELL_NAME]: ->`。
2. 之后调用`welcome()`函数，输出欢迎信息并且打印出`YIN_banner`字符图案。



## 二、执行命令以测试



### 1. 输入有效命令，如ls，clear：

1. ls执行测试结果如下：

```bash
->ls
a.txt		 jmp1.c		 test2.c	       text.txt
b.txt		 mine.txt	 test2 (第3个复件).c   time1
env1		 niubi		 test2 (第4个复件).c   time2
env1.c		 niubi.c	 test2 (复件).c        time2.c
env2		 niubi (复件).c  test2 (另一个复件).c  timeout1
env2.c		 sig1		 test3		       timeout1.c
exec		 sig1.c		 test3.c	       timeout2
exec1		 sig2		 test4		       timeout2.c
exec1.c		 sig2.c		 test4.c	       timeout3
exec2		 sig3		 test.c		       timeout3.c
exec2.c		 sig3.c		 testdup	       Yshell
exec3		 TEMP_code.txt	 testdup1.5	       Yshell.c
exec3.c		 tempfile	 testdup2	       Yshell (复件).c
file		 test0		 testdup2.c	       Yshell (另一个复件).c
file_shared	 test0.c	 testdup.c	       无标题文档
initServer_bg	 test1		 testIO.c
initServer_bg.c  test1.c	 testshell
jmp1		 test2		 testshell.c
->
```

[![RuOR76.png](https://z3.ax1x.com/2021/06/23/RuOR76.png)](https://imgtu.com/i/RuOR76)



2. clear执行结果如下：

```bash
=================Have Fun====================
->clear







->
```

  [![RuOCTK.png](https://z3.ax1x.com/2021/06/23/RuOCTK.png)](https://imgtu.com/i/RuOCTK)

可以看到clear命令执行正常（为了显示命令我把shell页面回滚了）



### 2. 输入 无效 或者当前shell库中未安装的 命令：

例如输入命令`fuck`,会提示当前命令无法识别：

[![RKehUU.png](https://z3.ax1x.com/2021/06/24/RKehUU.png)](https://imgtu.com/i/RKehUU)

```bash
->fuck
Yshell: Sorry,I have NO idea of your command!
```



### 3. 输入命令修改命令行提示符

- 输入命令printenv,打印输出YSHELL_NAME的值为默认的->，说明setenv()的相关函数功能正常:

  [![RuXHGF.png](https://z3.ax1x.com/2021/06/23/RuXHGF.png)](https://imgtu.com/i/RuXHGF)

- 并修改YSHELL_NAME的值, 提示信息：`Yshell: Username has been changed into YinXiaojie:.`并且命令行提示符名称修改为了“YinXiaojie：”说明`parsecmd()`相关的修改变量以及重命名代码正常：

  [![RujUiT.png](https://z3.ax1x.com/2021/06/23/RujUiT.png)](https://imgtu.com/i/RujUiT)

- 再次printenv，显示变量YSHELL_NAME已经被修改为了“YinXiaojie：”：

  [![RuvltK.png](https://z3.ax1x.com/2021/06/23/RuvltK.png)](https://imgtu.com/i/RuvltK)





### 4. 测试输出重定向

运行命令`ls > out.txt`并`cat out.txt`：

```bash
YinXiaojie:ls > out.txt
YinXiaojie:cat out.txt
a.txt
b.txt
env1
env1.c
env2
env2.c
exec
exec1
exec1.c
exec2
exec2.c
```

out.txt本不存在，重定向命令在执行的时候就会创建对应的文件，并且将输出重定向到out.txt中：

![image-20210623082020757](/home/yin/.config/Typora/typora-user-images/image-20210623082020757.png)

不提供输入文件/提供的文件无效 的时候，会提示需要目标文件：

[![RuxS3D.png](https://z3.ax1x.com/2021/06/23/RuxS3D.png)](https://imgtu.com/i/RuxS3D)



### 5. 测试输入重定向

执行命令`cat < out.txt`，输入重定向正常显示对应内容：

```bash
YinXiaojie:cat < out.txt
a.txt
b.txt
env1
env1.c
env2
env2.c
exec
exec1
exec1.c
...
```

[![Ruxlbn.png](https://z3.ax1x.com/2021/06/23/Ruxlbn.png)](https://imgtu.com/i/Ruxlbn)

不提供输出文件/提供的文件无效 的时候，会提示需要目标文件：

[![RKSXEn.png](https://z3.ax1x.com/2021/06/23/RKSXEn.png)](https://imgtu.com/i/RKSXEn)







### 6. 同时使用输入和输出重定向：

1. 使用 `< `和` > `管道命令时，提供非法参数，错误示例,输入命令`cat < ls > a.txt`, 提示ls并非有效可读文件：

```bash
->cat < ls > a.txt
Open Error when RDONLY with '<' and '>' ,'ls' is NOT readable!
```

2. 正确示例：

```bash
->cat < out.txt > a.txt
a.txt
b.txt
env1
```

[![RKeT29.png](https://z3.ax1x.com/2021/06/24/RKeT29.png)](https://imgtu.com/i/RKeT29)



### 7. 测试简单的管道命令 #

执行命令`ls -s # sort -n`：ls的结果会传给sort做参数，并且按照行号排序：

[![RKEzCj.png](https://z3.ax1x.com/2021/06/24/RKEzCj.png)](https://imgtu.com/i/RKEzCj)

但是这个功能并不完善，目前做了单次管道的功能，在多次管道的时候就不管用了：

当使用类似`ls -s # sort -n # a.txt`的带有两个管道符号的命令的时候，就不太行了,但是我对于意外情况也提供了处理，Yshell会提示当前的命令现在暂时不能支持，但是在之后的处理当中会进一步完善：

[![RKZJmV.png](https://z3.ax1x.com/2021/06/24/RKZJmV.png)](https://imgtu.com/i/RKZJmV)

```bash
->ls -s # sort -n # cat
Yshell: I'm SORRY this program CAN'T support this command. 
But I will improve this part of the function in the future work. 
To Be Continued...

->
```



### 8. 测试后台运行bg ，&和 jobs

#### 测试bg命令——示例8. 1：time2

测试bg命令不是太好测试，我自己写了一个测试程序time2：

```bash
yin@ubuntu:/mnt/hgfs/Shared/yshell$ cat time2.c
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
  
int n = 0;
  
void timefunc(int sig){
        fprintf(stderr, "Alarm %d\n", n++);
        signal(SIGALRM, timefunc);
        alarm(1);
}
  
int main()
{
        int status;
  
        signal(SIGALRM, timefunc);
        alarm(1);
        while(1);
  
        return 0;
}
yin@ubuntu:/mnt/hgfs/Shared/yshell$ 

```

执行time2，每隔一秒会打印一次Alarm和计数值，之后我们使用ctrl+z挂起time2,使time2在后台暂停，暂时不再输出Alarm：

[![RKp2xU.png](https://z3.ax1x.com/2021/06/24/RKp2xU.png)](https://imgtu.com/i/RKp2xU)

使用jobs命令查看所有进程找到./time2对应的进程号30203，使用bg %30203使在**后台暂停**的time2任务改为在**后台进行**，可以看到./time2继续在后台不断进行并且会打印输出到前台：

[![RKCFt1.png](https://z3.ax1x.com/2021/06/24/RKCFt1.png)](https://imgtu.com/i/RKCFt1)



该示例在Yshell下与在Ubuntu的shell下执行的输出结果完全一致，说明自己写的`bg`命令实现了unix自带的`bg`命令效果。

#### 测试 & 命令——示例8.2：

使用命令` cat out.txt &` ,运行之后Yshell将任务挂在后台执行，并且立即返回新的提示符，之后cat输出的结果在新的提示符后面显示：

[![RKAqk4.png](https://z3.ax1x.com/2021/06/24/RKAqk4.png)](https://imgtu.com/i/RKAqk4)

ls命令也是这样：

[![RKEaHU.png](https://z3.ax1x.com/2021/06/24/RKEaHU.png)](https://imgtu.com/i/RKEaHU)

#### 测试 jobs 命令：

返回当前计算机的所有进程,结果正常：

[![RKEs3R.png](https://z3.ax1x.com/2021/06/24/RKEs3R.png)](https://imgtu.com/i/RKEs3R)





### 9. 查看历史

使用`ctrl+c`发送中断信号，信号被注册为查看历史命令的快捷键，最大历史记录条书被`MAX_HISTORY`所规定,历史记录条数前面还会显示次序：

[![RKZdfJ.png](https://z3.ax1x.com/2021/06/24/RKZdfJ.png)](https://imgtu.com/i/RKZdfJ)



### 10. 关闭和退出Yshell

#### 10.1 使用ctrl-d退出Yshell

特定组合键为EOF(ctrl-d),按组合键位退出的时候会提示“Yshell will close with EOF...” ，并且有GoodBye 的字符串友好提示:
[![RKVePJ.png](https://z3.ax1x.com/2021/06/24/RKVePJ.png)](https://imgtu.com/i/RKVePJ)



#### 10.2 使用quit命令退出Yshell

直接使用quit退出的时候，只会有GoodBye 的字符串友好提示。

[![RKV0qf.png](https://z3.ax1x.com/2021/06/24/RKV0qf.png)](https://imgtu.com/i/RKV0qf)





### 11. 其他测试

其他测试再次不再赘述，我已经收录了这些测试到本文第三章的部分"三、附录 程序错误列表与解决方式",详情请见下文：





## 三、附录 程序错误列表与解决方式

本列表收录了大部分本YShell中可能遇到的错误，并且提供了相应的解决方案。

| 异常/错误提示                                                | 错误原因                                                     | 解决方式                                                     |
| ------------------------------------------------------------ | ------------------------------------------------------------ | ------------------------------------------------------------ |
| Failed to add %s                                             | 未能向环境提供局部变量YSHELL_NAME                            | 手动添加变量YSHELL_NAME到环境变量中                          |
| Yshell: bg: no job assigned                                  | 在使用bg命令的时候未能提供有效的任务pid                      | 供有效的任务pid或创建有效的任务pid                           |
| Yshell: jobs: incorrect use of jobs                          | jobs命令使用错误                                             | 使用ps ax查看当前进程并且正确地使用jobs命令                  |
| Yshell: chYshellname:incorrect input of chYshellname         | 自定义提示符的时候提供了无效的参数                           | 提供有效的参数给YSHELL_NAME                                  |
| Yshell: Sorry,I have NO idea of your command!                | 提供了无效的命令或者本shell未安装的命令                      | 安装对应的命令或者修改为有效的命令                           |
| YShell：Close Error.                                         | 未能正确地关闭已打开的文件                                   | 停止可能引发已打开文件读写出错的行为或者操作                 |
| Command Error:Output Target file needed!                     | 使用含有 > 命令的时候未提供有效的输出文件参数                | 提供有效的输出文件参数或者修改您的命令                       |
| Open Error when O_WRONLY\|O_CREAT\|O_TRUNC with '>'          | 未能正确地关闭已打开 > 命令中所指向的文件                    | 停止可能引发已打开文件读写出错的行为或者操作                 |
| Dup2 Error when STDOUT with '>' ！                           | > 所包含的dup2指令未能正确执行                               | 停止可能引发已打开文件读写出错的行为或者操作                 |
| Open Error when RDONLY with '<' and '>' ,'%s' is NOT readable! | 在使用同时包含 <和> 的命令的时候未能提供有效的输出文件       | 停止可能引发已打开文件读写出错的行为或者操作或者提供有效的用于输出文件 |
| Open Error when RDONLY with '#'                              | 使用含有 # 命令的时候未保证计算机的磁盘正常工作或者系统正常运作，或者恰有有名为"tempfile"的文件被读写 | 更换计算机磁盘或正确修正操作系统或者删除当前文件夹内为"tempfile"的文件 |
| I'm SORRY this program CAN'T support this command. \nBut I will improve this part of the function in the future work. \nTo Be Continued... | 输入了含有多个管道符号#的命令或者输入了本Shell尚未完成的功能的命令 | 减少命令中的管道符号为一个或者等待本Yshell的进一步完善       |

