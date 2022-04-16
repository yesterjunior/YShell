# README

> 本程序一种类似shell命令解释器，shell 是一个用 C 语言编写的程序，它是用户使用 Linux 的桥梁。命令解释器shell提供了一个界面，用户通过这个界面访问操作系统内核的服务。本程序从控制台启动后可以完成用户输入的各种命令。

初次使用请先阅读doc/目录下的YSHELL用户手册！

docs/中所有markdown所引用的**图片**已经托管至**在线的图床服务器**，如果您想有良好的阅读体验请保证您的计算机能够联网。



主要特点：
1. 友好的界面；
2. 支持自定义命令提示符；
3. 支持快捷键 或者quit命令 快速结束程序；
4. 支持任务后台运行机制；
5. 支持输入输出重定向；
6. 提供简单的管道系统；
7. 提供历史命令查询功能；

运行环境：
Linux操作系统（支持64位）
▶ Ubuntu
▶ Debian
▶ Fedora Core/CentOS
▶ SuSE Linux Enterprise Desktop
▶ FreeBSD
▶ Solaris
▶ MacOS

测试环境：

1. Linux ubuntu 5.3.0-40-generic #32-Ubuntu SMP Fri Jan 31 20:24:34 UTC 2020 x86_64 x86_64 x86_64 GNU/Linux

2. Linux ubuntu 4.13.0-36-generic #40~16.04.1-Ubuntu SMP Fri Feb 16 23:25:58 UTC 2018 x86_64 x86_64 x86_64 GNU/Linux

3. Darwin Mac 20.4.0 Darwin Kernel Version 20.4.0: Thu Apr 22 21:46:47 PDT 2021; root:xnu-7195.101.2~1/RELEASE_X86_64 x86_64

> 代码包文件结构：
> -codes 代码文件夹
>   --myshell.c 程序源代码C语言文件
>   --myshell Linux平台可执行程序（由myshell.c在上述测试环境下编译得到）
> -docs 文档文件夹
>   --YSHELL测试报告.md
>   --YSHELL详细设计文档.md
>   --YSHELL需求分析文档.md
>   --YSHELL用户手册.md
>   --YSHELL总结报告.md
>   --README.md

有关常见问题解答、设计文档和测试报告，请参见同目录下doc文件夹。
