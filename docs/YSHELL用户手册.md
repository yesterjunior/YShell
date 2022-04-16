# YSHELL用户手册

[toc]

> 文档更新日期：2021.6.24
>
> 文档版本：1.0.0
>
> 作者：yin@bupt.edu.cn

## 程序的安装

​	本程序主要内容为源代码编译，本程序提供测试环境（Linux ubuntu 4.13.0-36-generic #40~16.04.1-Ubuntu SMP Fri Feb 16 23:25:58 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux）下的可执行文件和C语言源代码文件（均放在 /codes 文件夹下）。**如果你运行本程序的操作系统与本测试环境相同，可以跳过本章内容**，直接使用可执行文件运行即可。你可以使用一下命令查看你自己的操作系统信息。

```bash
uname -a
```

本程序的安装过程主要为C语言源代码的编译，主要过程如下：

1. 找到codes/文件夹。

2. 右键"在终端中打开"。

3. 输入命令：

   ```bash
   make Yshell
   ```

   或者:

   ```bash
   gcc Yshell.c -o Yshell
   ```



4. 如果使用make编译本Yshell源码，您将会看到如下信息:

   ```bash
   cc     Yshell.c   -o Yshell
   ```

   这代表编译通过。
   再次编译，您将会收到如下信息：

   ```bash
   make: 'Yshell' is up to date.
   ```

   这代表本次编译与上次编译的源码并没有不同。

   如果使用gcc编译本Yshell源码，成功编译时提示符将不会输出信息。

5. 如果成功编译，您将会在codes/文件夹下看到一个新的Yshell可执行文件

## 程序的启动

启动步骤如下：

1. 运行本Yshell，您需要在上一步操作之后在同一个文件夹内：

2. 右键"在终端中打开"。

3. 输入命令：

   ```bash
   ./Yshell
   ```

4. 如果程序运行正确，您将会看到如下提示：

   ```bash
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

   这时候您就可以输入自己的命令并开始与Yshell的愉快之旅了。



## 程序的配置（可选）

本Yshell提供的标准命令提示符为`->`,如果您想个性化地修改该提示符号，您可以输入：

```bash
name=您自己的个性化符号
```

例如：

```bash
name=Yin:
```

就可以将`->`提示符号修改为`Yin:`。

或者您也可以通过修改变量`YSHELL_NAME`变量的值的方式来修改您自己的个性化提示符。

例如：

```bash
YSHELL_NAME=Yin:
```

您还可以通过`printenv`命令来查看当前`YSHELL_NAME`的值。

## 程序的运行与使用

程序的功能包括：

### 1. 一般命令的执行

如果命令中不含有输入输出重定向符号`< `, `>` 与管道符号 `#`，那么请直接输入到命令行并运行。

如果该命令无效或者您的计算机并未含有对应的库/包的话，系统将会提示您 当前输入的命令无法被识别：

```bash
Yshell: Sorry,I have NO idea of your command!
```

需要注意的是，本Shell中的`jobs`命令已重构，用于查看本计算机的所有进程，功能相当于其他类型Shell的`ps ax`命令。



### 2. 含有 `< `符号

`<` 在本Shell中为输入重定向符号，示例：

```bash
cat < in.txt
```



### 3. 含有 `> `符号

`>` 在本Shell中为输入重定向符号，示例：

```bash
ls > out.txt
```



### 4.  同时含有 `<` 和 `>`

本Yshell支持 ` 输出1 < 文件 > 输出2 ` 的形式来同时使用输出和输入重定向符号，示例：

```bash
cat < out.txt > in.txt
```



### 5. 含有管道符号 # 

本shell支持简单的管道功能，管道命令符号 #,您的命令可以包含一个管道符号， 示例：

```
ls -s # sort -n
```

遗憾的是本Yshell仅支持一个管道符号，含有一个以上的管道符号在执行的时候会提示本Shell功能尚未完善：

```bash
Yshell: I'm SORRY this program CAN'T support this command. 
But I will improve this part of the function in the future work. 
To Be Continued...
```



### 6. 查看历史命令

使用快捷键`ctrl+c`命令查看您输入的命令历史，最多包含10条。

示例：

```bash
->^C
1. cat < ls > a.txt 
2. cat < out.txt > a.txt 
3. ls -s # sort -n 
4. ls -s # sort -n # asda 
5. fuck 
```



### 7. 启用后台执行任务

本Yshell支持两种方式来把任务放到后台执行：

1. 方式一： `bg 任务`，示例（该示例与对应的示例代码在测试报告中有详尽的示例）：

   ```bash
   ->./任务1
   
   ->bg %任务1所对应的pid
   
   ```

   

2. 方式二：`命令 &`，更加推荐使用这种方式来执行命令，示例：

   ```bash
   ->ls &
   ```



### 8. 退出本YShell

本Yshell支持两种退出方式：

1. 快捷键`ctrl-d`:

   ```bash
   ->Yshell will close with EOF...
   ==================GoodBye====================
   ```

2. 直接输入命令`quit`:

   ```bash
   ->quit
   ==================GoodBye====================
   ```

如图所示会有两种不同的提示信息。

## 常见错误与注意事项

### 1. 输入无效的命令或者Shell尚未安装的命令

例如输入命令`fuck`,会提示当前命令无法识别：

[![RKehUU.png](https://z3.ax1x.com/2021/06/24/RKehUU.png)](https://imgtu.com/i/RKehUU)

```bash
->fuck
Yshell: Sorry,I have NO idea of your command!
```

### 2. 一个命令中使用了多个管道符号 #

当使用类似`ls -s # sort -n # a.txt`的带有两个管道符号的命令的时候，就不太行了,但是我对于意外情况也提供了处理，Yshell会提示当前的命令现在暂时不能支持，但是在之后的处理当中会进一步完善：

[![RKZJmV.png](https://z3.ax1x.com/2021/06/24/RKZJmV.png)](https://imgtu.com/i/RKZJmV)

```bash
->ls -s # sort -n # cat
Yshell: I'm SORRY this program CAN'T support this command. 
But I will improve this part of the function in the future work. 
To Be Continued...

->
```

### 3. 提供了无效的输入/输出重定向文件类型

不提供输出文件/提供的文件无效 的时候，会提示需要目标文件（下图为无效输入的情况，无效输出同理）：

![RKSXEn.png](https://z3.ax1x.com/2021/06/23/RKSXEn.png)

### 4. 使用 `< `和` > `管道命令时，提供非法参数：

错误示例,输入命令`cat < ls > a.txt`, 提示ls并非有效可读文件：

```bash
->cat < ls > a.txt
Open Error when RDONLY with '<' and '>' ,'ls' is NOT readable!
```

正确示例：

```bash
->cat < out.txt > a.txt
a.txt
b.txt
env1
```



## 附录 程序错误列表与解决方式

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

