# YSHELL 需求分析

[toc]

> docs/中所有markdown所引用的**图片**已经托管至**在线的图床服务器**，如果您想有良好的阅读体验请保证您的计算机能够联网。

## 一、核心功能

### 1. 获取用户输入的命令并执行：

​	作为一个shell程序, Yshell应该能够读取用户输入的命令并且解析,执行该命令.

#### 分析

​	由于最终必须进行系统调用交给kernal执行，因此本程序可以在逻辑上视为用户命令的中转. 具体可分为两步：

1. 获取用户输入的命令并且解析,判断是否合法是否可执行；
2. 解析后的命令分别去执行.

#### 拟解决方案

1. Unix的 `exec Function Family` 均可以用于执行command命令，只需要指定将命令名称和参数分别传入. 

   ```cpp
   #include <unistd.h>
   int execl(const char *pathname , const char *arg0 , ...);
   int execv(const char *pathname , char *const argv[]);
   int execle(const char *pathname , const char *arg0 , ...,
   /* (char *)0, char *const envp[] */);
   int execve(const char *pathname , char *const argv[],
   char *const envp[]);
   int execlp(const char *filename , const char *arg0 , ...);
   int execvp(const char *filename , char *const argv[]);
   int fexecve(int fd, char *const argv[],
   char *const envp[]);
   ```

   函数`execl`、`execle`和`execlp`要求要指定的新程序的命令行参数作为单独的参数，结尾为空指针. 对于其他四个函数，我们必须建立一个指向参数，这个数组的地址就是参数这三个功能. 

   名字以e结尾的三个函数允许我们将指针传递到指向环境的指针数组串. 但是，其他四个函数使用调用进程中的变量，以复制现有的新程序的环境. 

   此外，exec函数簇的函数执行成功后是无返回的，一般需要和fork()函数同时使用. 在使用时需要另外的fork一个进程. 

2. system Function 将通过调用/bin/sh\-c string执行string中指定的命令，并在命令完成后返回. 传入参数为完成的命令、空格、参数的组合字符串. 

   ```cpp
   #include <stdlib.h>c
   int system(const char *string );
   ```

   虽然可以执行command但是此函数系统依赖性强，使用后可能会降低本程序的健壮性和可移植性. 

   在执行命令期间，`SIGCHLD`将被阻止，`SIGINT`和`SIGQUIT`将被忽略. 

3. `popen() `函数通过创建一个管道，调用 fork 产生一个子进程，执行一个 shell 以运行命令来开启一个进程. 这个进程必须由 `pclose() `函数关闭，而不是 `fclose()` 函数. 

### 2. 基本的输入输出交互

​	作为一个shell程序, Yshell应该能够有文件形式的基本输入输出交互.

#### 分析

​	当程序运行后必须能够通过终端与用户进行交互，包括读取用户的输入命令和在屏幕中打印提示信息、程序运行信息等, 并且一个shell应该能够定制用户名提示符,在进入和退出shell的时候应该也能有提示信息.

#### 拟解决方案

1. 输入读取：需要调用 Standard I/O Library 从 `stdin` 流读取用户输入，使用 `fgets` 函数. 

```cpp
#include <stdio.h>
char *fgets(char *restrict buf , int n, FILE *restrict fp);
```

此函数可以读取用户的一整行输入，由于`fgets`函数需要指定读取字符数量，因此需要在程序中定义固定大小的宏. 

2. 输出写入：向屏幕打印信息，需要调用 `Standard I/O Library` 中的 `Formatted Output Functions`. 

```cpp
#include <stdio.h>
int printf(const char *restrict format , ... );
int fprintf(FILE *restrict fp,
const char *restrict format , ... );
int dprintf(int fd, const char *restrict format , ...);
int sprintf(char *restrict buf ,
const char *restrict format , ...);
int snprintf(char *restrict buf , size_t n,
const char *restrict format , ...);
```

 	通过这些函数以特定格式向 stdout 流输出信息，也可以使用以下函数. 

```cpp
#include <stdarg.h>
#include <stdio.h>
int vprintf(const char *restrict format , va_list arg);
int vfprintf(FILE *restrict fp ,
const char *restrict format , va_list arg);
int vdprintf(int fd , const char *restrict format , va_list arg);
int vsprintf(char *restrict buf , const char *restrict format , va_list arg);
int vsnprintf(char *restrict buf , size_t n, const char *restrict format , va_list arg);
```

3. 程序需要即时与用户交互响应，而这些函数由于 Buffering 机制可能不能实时显示到屏幕上，因此在调用完成后需要再次调用 `fflush` 强制输出流刷新. 

```cpp
#include <stdio.h>
int fflush (FILE *fp);
```



### 3. 根据环境变量确定命令提示符

​	作为一个shell程序, Yshell应该能够使用户自定义命令提示符, 只要修改特定的环境变量就好.

#### 分析

​	用户可以通过给特定的环境变量赋值来改变命令提示符的形式. 程序需要读取特定名称的环境变量并分类讨论.

​	unix的环境变量有两种,一种是全局的环境变量需要写进系统的配置文件可以脱离程序继续存在,另一种是程序附带的局部变量,只有在程序的生命期内才有效,对于一个自定义的shell,我不希望它修改`全局变量`, 而应该去创建一个但对于的`局部变量`就可以.

​	实际shell运行的时候,如果未设置，则遵从默认的命令提示符`->`，如果已经设置，判断是否合法，如果内容合法（非空且不与其他关键字冲突）则替换为新的命令提示符进行展示，此处应该在程序开头运行一次.

#### 拟解决方案

​	c程序在unix中有两种环境变量的管制函数,一种是`putenv`另一种是`setenv`:

1. putenv()函数
  ```cpp
  int putenv(char *string);
  ```

2. setenv()函数

   ```cpp
   int setenv(const char*name, const cahr *vlaue, int overwrite);
   ```

   `putenv` 函数和 `setenv` 函数虽然功能类似，但是这两个函数在实现方面还是有区别的，区别如下：

   `putenv`函数：

   - `putenv` 函数会将参数 string 直接填写到环境表中，不会再为 "name=value" 这个字符串再去分配内存。如果是在一个函数中定义的string，那么在调用该函数后，string 指向的内容可能会被释放，就找不到name环境变量的值了。

    `setenv` 函数：

   - `setenv` 函数和 `putenv` 函数不同，它会将name和value指向的内容复制一份并为其分配内存，形成 "name=value"  的字符串，并将其地址写入到环境表中。所以就不会出现上面`putenv` 的情况，就算函数返回了，name 和  value指向的内容被释放了，仍然有一份拷贝在。

     使用环境变量管制函数添加变量到环境之后，用户就可以通过修改变量来设置个性化的shell提示符.





###  4. 任务后台运行功能

​	作为一个shell程序, Yshell应该能够通过`bg`命令 或者 `&` 后缀提供任务后台运行的功能, 当用户后台运行时，程序将任务交给子进程并不监听等待，马上返回给用户一个新的提示符`->`.

#### 分析

​	`bg`命令 或者 `&` 后缀应该能够切换子进程到后台并且继续执行, 那么就需要处理父进程的信号`SIGCHLD`.

#### 拟解决方案

​	用户提交的任务可以通过某种指示使之在后台运行，程序首先需要定义特定的命令关键字标识用户希望后台运行，其次命令的执行需要由产生新的进程执行. 当用户不后台执行，程序需要监听子进程结果等待返回信息然后提示用户.

​	这里对于父进程我考虑两个`waitpid()`的参数:

 1. WNOHANG：如果没有任何已经结束的子进程则马上返回, 不予以等待.

 2. WUNTRACED：如果子进程进入暂停执行情况则马上返回, 但结束状态不予以理会. 子进程的结束状态返回后存于status, 底下有几个宏可判别结束情况.

    对于子进程我考虑应该处理SIGCHLD信号:

    ```cpp
    signal(SIGCHLD,SIG_IGN);	//忽视SIGCHLD信号
    ```

    

### 5. 输出和输出重定向

​	作为一个shell程序, Yshell应该能够提供输入输出重定向:

1. 提供输出重定向。通过指定文件名将任务的所有输出覆盖写到文件中而不
   是送到标准输出上;
2. 提供输入重定向。通过指定文件名使得任务从相应的文件中去获取所需的
   数据，而不是从标准输入上。

#### 分析

​	程序需要支持用户的输出和输出重定向，默认情况下输入从标准输入（0号文件描述符）读入，输出从标准输出（1号文件描述符）写入，错误信息从错误输出（3号文件描述符）写入. 在重定向情况下，这些输出和输出需要替换为用户给定的输入和输出. 此外，输入和输出是可选的，不是每一条命令都一定有，因此需要根据用户输入确定，进而程序需要约定相应的操作符号并进行判断. 

#### 拟解决方案：

​	首先根据命令判断是否存在重定向关键字，例如`>`和`<`符号，并根据符号确定指定的输入和输出，利用`open `先打开输入输出得到文件描述符，但由于这些是随即给定的，因此需要利用 `dup `和` dup2 `进行拷贝，将指定的输出输出覆盖到0号文件描述符和1号文件描述符，使得命令执行功能直接输入输出即可，不需要考虑重定向问题，此外，在命令结束之后需要对重定向进行恢复，使得用户下一次正常输入可以正确执行. 可能 需要用到的函数如下：

```cpp
int dup(int filedes);
int dup2(int filedes , int filedes2);
FILE *fopen(const char* restrict pathname ,
const char* restrict type);
FILE *freopen(const char* restrict pathname ,
const char* restrict type ,
FILE* restrict fp);
FILE *fdopen(int fd , const char *type);
```







​	

## 二、辅助功能

### 6. 简单的管道功能

​	作为一个shell程序, Yshell应该能够分析和处理 简单的管道命令. 用户可以通过`#`命令来作为管道连接双向的输入输出.

#### 分析

​	管道的执行方向是比较固定的,因此可以通过管道符号来解析和分割来自用户的命令,并且按照从左到右 的顺序执行对应的命令.本shell作为一个简单的shell,我希望它能处理只含有一个管道符号`#`的命令。

#### 拟解决方案：

​	同上一条功能“输出和输出重定向”理类似，这里不再赘述。

### 7. 通过特定的命令和按键组合正常关闭程序

​	作为一个shell程序, Yshell应该能够在用户输入特定的组合键位或者输入特定的指令之后正常退出。

#### 分析

​	组合键位我选用`ctl-d`，它相比`ctrl+c `或 `ctrl+z` 不同，`ctrl-d` 并不是发送信号，而是表示一个特殊的二进制值，表示 `EOF`。

​	特定命令则只需要执行对应的自定义函数即可。

#### 拟解决方案：

​	特定按键组合：使用 signal 机制，在程序运行一开始注册相应 signal 的处理函数，当用户按下后，kernal 将信号发送给本程序，指定的退出函数开始执行. 

```cpp
#include <signal.h>
void (* signal(int signo , void (*func)(int )))( int);
```

​	特定命令则只需要`exit()`函数即可完成。



### 8. 历史命令查询

​	作为一个shell程序, Yshell应该能够存储以往输入的命令并通过一定方式查询显示出来。

#### 分析

​	在这里我选用拦截处理`ctrl+c`发送的信号作为查询历史命令的快捷键。

#### 拟解决方案：

​	将历史命令存储在特定的变量当中并绑定信号作为快捷键，最后提供历史消息的显示功能。



### 9. 友好的提示信息

​	作为一个个性化的 shell程序, Yshell应该能够在进入和退出程序的时候输出友好的提示信息。





## 三、其他功能

### 1. 统一的错误处理机制

*   程序应定义好全局特定的错误信息，包括但不限于使用宏等. 

*   错误信息应分为严重错误和功能错误，功能错误只给出提示、用户可以继续执行，严重错误才应终止程序. 

*   任何情况下，程序退出必须要有提示信息. 

*   对于上述核心功能使用的函数都需要通过返回值判断是否出错并进行相应的错误处理. 

*   错误的处理可以封装一个或多个顺序调用的函数以便主函数使用. 

### 2. 良好的输出提示

*   程序输出内容上应具有良好的可读性. 

*   程序可以提供 help 命令帮助用户使用本程序. 

*   输出信息的格式应该美观整齐. 

### 3. 快速简便的程序安装（编译）过程和运行体验

程序的安装过程应尽量简单，对于测试环境下的其他计算机可以直接拷贝可执行文件到计算机后直接执行，对于其他Linux系统应使用gcc命令直接编译生成可执行文件后直接执行. 由于示例文件加入了 apue.h 的头文件，因此应该将需要的内容（头文件、函数和变量）拷贝出来. 

*   程序的安装（编译）仅使用gcc命令一行语句执行，不应使用make等额外操作. 

*   程序的文件应尽可能简单，只包含一个c语言的文本文件和对于的一个可执行文件，不应依赖于 apue 等其他项目文件. 