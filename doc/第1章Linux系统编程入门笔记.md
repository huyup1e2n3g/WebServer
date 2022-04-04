
# GCC
## GCC 与 G++ 的区别
gcc 和 g++都是GNU(组织)的一个编译器。  
误区一:gcc 只能编译 c 代码，g++ 只能编译 c++ 代码。两者都可以，请注意：

- 后缀为 .c 的，gcc 把它当作是 C 程序，而 g++ 当作是 c++ 程序。
- 后缀为 .cpp 的，两者都会认为是 C++ 程序，C++ 的语法规则更加严谨一些。 
- 编译阶段，g++ 会调用 gcc，对于 C++ 代码，两者是等价的，但是因为 gcc 命令不能自动和 C++ 程序使用的库联接，所以通常用 g++ 来完成链接，为了统 一起见，干脆编译/链接统统用 g++ 了，这就给人一种错觉，好像 cpp 程序只 能用 g++ 似的。

误区二：gcc 不会定义 __cplusplus 宏，而 g++ 会  
- 实际上，这个宏只是标志着编译器将会把代码按 C 还是 C++ 语法来解释。  
- 如上所述，如果后缀为 .c，并且采用 gcc 编译器，则该宏就是未定义的，否则，就是已定义。

误区三：编译只能用 gcc，链接只能用 g++
- 严格来说，这句话不算错误，但是它混淆了概念，应该这样说：编译可以用 gcc/g++，而链接可以用 g++ 或者 gcc -lstdc++。
- gcc 命令不能自动和C++程序使用的库联接，所以通常使用 g++ 来完成联接。 但在编译阶段，g++ 会自动调用 gcc，二者等价。

| gcc 编译选项                            | 说明                                                         |
| --------------------------------------- | ------------------------------------------------------------ |
| -E                                      | 预处理指定的源文件，不进行编译                               |
| -S                                      | 编译指定的源文件，但是不进行汇编                             |
| -c                                      | 编译、汇编指定的源文件，但是不进行链接                       |
| -o [file1] [file2] / [file2] -o [file1] | 将文件 file2 编译成可执行文件 file1                          |
| -I directory                            | 指定 include 包含文件的搜索目录                              |
| -g                                      | 在编译的时候，生成调试信息，该程序可以被调试器调试           |
| -D                                      | 在程序编译的时候，指定一个宏                                 |
| -w                                      | 不生成任何警告信息                                           |
| -wall                                   | 生成所有警告信息                                             |
| -On                                     | n的取值范围:0~3。编译器的优化选项的4个级别，-O0表 示没有优化，-O1为缺省值，-O3优化级别最高 |
| -l                                      | 在程序编译的时候，指定使用的库                               |
| -L                                      | 指定编译的时候，搜索的库的路径                               |
| -fPIC/fpic                              | 生成与位置无关的代码                                         |
| -shared                                 | 生成共享目标文件，通常用在建立共享库时                       |
| -std                                    | 指定 c 方言，如：-std=c99，gcc默认的方言是GNU C              |

## GCC 的编译过程

![image-20220324104346747](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174243.png)


# 静态库与动态库
## 什么是库

- 库文件是计算机上的一类文件，可以简单的把库文件看成一种代码仓库，它提供给使用 者一些可以直接拿来用的变量、函数或类。

- 库是特殊的一种程序，编写库的程序和编写一般的程序区别不大，只是库不能单独运行。
- 库文件有两种，静态库和动态库(共享库)，区别是:静态库在程序的链接阶段被复制 到了程序中;动态库在链接阶段没有被复制到程序中，而是程序在运行时由系统动态加载到内存中供程序调用。
- 库的好处：1.代码保密 2.方便部署和分发

## 静态库的制作和使用

命名规则：**libxxx.a**

> lib : 前缀(固定) 
>
> xxx : 库的名字，自己起 
>
> .a : 后缀(固定)

静态库的制作：

1. gcc 获得 .o 文件
2. 将 .o 文件打包，使用 ar 工具（archive）

```
ar rcs libxxx.a xxx.o xxx.o
// r - 将文件插入备存文件中
// c - 建立备存文件
// s - 索引
```

制作实操：

```
// tree 目录
.
├── calc
│   ├── add.c
│   ├── div.c
│   ├── head.h
│   ├── main.c
│   ├── mult.c
│   └── sub.c
└── library
    ├── include
    │   └── head.h
    ├── lib
    ├── main.c
    └── src
        ├── add.c
        ├── div.c
        ├── mult.c
        └── sub.c
```

calc 文件夹下是要制作成库的源文件，library 是完整的工程文件库，library 下的 src 文件夹中是制作静态库的源文件。

进入到 src 文件夹中，将源文件编译生成 -o 文件，因为源文件中使用到了 include 下的 head.c 文件，所以用 `-I` 搜索头文件所在目录，如果不加就是在当前目录下搜索：

```
gcc -c add.c div.c mult.c sub.c -I ../include/
```

生成后的文件：

```
.
├── add.c
├── add.o
├── div.c
├── div.o
├── mult.c
├── mult.o
├── sub.c
└── sub.o
```

将 .o 文件打包，生成 libcalc.a 库：

```
ar rcs libcalc.a add.o div.o mult.o sub.o
```

将 libcalc.a 移动到 lib 目录下：

```
mv libcalc.a ../lib/
```

静态库的使用，编译生成 app：

```
gcc main.c -o app -I ./include/ -L ./lib/ -l calc
```

`-I` 为搜索 `head.h` 所在目录，`-L` 为搜索库所在目录，`-l` 为所使用到的库的名称。

运行 app

```
./app
```



## 动态库的制作和使用

命名规则：**libxxx.so**

> lib : 前缀(固定) 
>
> xxx : 库的名字，自己起 
>
> .so : 后缀(固定)
>
> 在Linux下是一个可执行文件



动态库的制作：

1. gcc 得到 .o 文件，得到和位置无关的代码

```
gcc -c –fpic/-fPIC a.c b.c
```

2. gcc 得到动态库

```
gcc -shared a.o b.o -o libcalc.so
```



制作实操：

```
// tree 目录
.
├── calc
│   ├── add.c
│   ├── div.c
│   ├── head.h
│   ├── main.c
│   ├── mult.c
│   └── sub.c
└── library
    ├── include
    │   └── head.h
    ├── lib
    ├── main.c
    └── src
        ├── add.c
        ├── div.c
        ├── mult.c
        └── sub.c
```

进入到 calc 文件夹下用 .c 文件制作动态库，gcc 得到 .o 文件，得到和位置无关的代码：

```
gcc -c -fpic add.c div.c mult.c sub.c
```

生成后的文件：

```
.
├── add.c
├── add.o
├── div.c
├── div.o
├── head.h
├── main.c
├── mult.c
├── mult.o
├── sub.c
└── sub.o
```

gcc 得到动态库：

```
gcc -shared add.o div.o mult.o sub.o -o libcalc.so
```

ll 查看文件，可以看到 libcalc.so 是可执行文件：

```
ll
```

![image-20220326122132226](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174222.png)

使用动态库：

将动态库 libcalc.so 复制到 lib 文件下

```
cp calc/libcalc.so library/lib
```

使用动态库，编译生成 main 文件：

```
gcc main.c -o main -I include/ -L lib/ -l calc
```

`-I` 为搜索 `head.h` 所在目录，`-L` 为搜索库所在目录，`-l` 为所使用到的库的名称。

执行 main 文件：

```
./main
```

出现错误，找不到动态库文件：

```
./main: error while loading shared libraries: libcalc.so: cannot open shared object file: No such file or directory
```

#### 动态库加载失败的原因

工作原理：

- 静态库：GCC 进行链接时，会把静态库中代码打包到可执行程序中。
- 动态库：GCC 进行链接时，动态库的代码不会被打包到可执行程序中。
- 程序启动之后，动态库会被动态加载到内存中，通过 ldd (list dynamic dependencies)命令检查动态库依赖关系。
- 如何定位共享库文件呢：当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路 径。此时就需要系统的动态载入器来获取该绝对路径。对于elf格式的可执行程序，是 由ld-linux.so来完成的，它先后搜索elf文件的 **DT_RPATH**段 ——> **环境变量** **LD_LIBRARY_PATH** ——> **/etc/ld.so.cache**文件列表 ——> **/lib/**，/usr/lib 目录找到库文件后将其载入内存。



ldd 命令检查动态库依赖关系：

```
ldd main
```

发现 `libcalc.so => not found` ：

```
linux-vdso.so.1 =>  (0x00007ffcd55f8000)
libcalc.so => not found
libc.so.6 => /lib64/libc.so.6 (0x00007fe2ddf8f000)
/lib64/ld-linux-x86-64.so.2 (0x000055996189f000)
```



动态库加载失败的原因：查找动态库的时候找不到动态库，因为没有指定动态库的绝对路径。

解决方法：在搜索 elf 文件的路径下配置，这样在查找的时候就能找到。

**DT_RPATH**段改变不了，就需要修改环境变量。

#### 配置动态库环境变量

##### 修改**环境变量** **LD_LIBRARY_PATH**。

方法一：临时修改 **环境变量** **LD_LIBRARY_PATH**。

首先确定动态库所在路径：

```
pwd
/root/Linux/lesson1.6/library/lib
```

改变环境变量：

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/Linux/lesson1.6/library/lib
```

(`:` 表示拼接)

查看设置好的环境变量：

```
echo $LD_LIBRARY_PATH
```

(备注：清除环境变量命令)

```
unset LD_LIBRARY_PATH
```

结果：

```
:/root/Linux/lesson1.6/library/li
```

用 ldd 命令检查动态库依赖关系：

```
ldd main
```

这时候发现 `libcalc.so` 查找到了路径：

```
linux-vdso.so.1 =>  (0x00007ffe8efa3000)
libcalc.so => /root/Linux/lesson1.6/library/lib/libcalc.so(0x00007f907702f000)
libc.so.6 => /lib64/libc.so.6 (0x00007f9076c65000)
/lib64/ld-linux-x86-64.so.2 (0x00005597336f6000)
```

运行 main 程序成功：

```
./main
a = 20, b = 12
a + b = 32
a - b = 8
a * b = 240
a / b = 1.666667
```

但是该方法不是永久的，因为环境变量是在终端里配置的，所以重新打开终端就要重新配置环境变量。



永久配置方法：

有两种，用户级别的配置和系统级别的配置。

用户级别的配置：

home 文件（root）下，其实有一个 `.bashrc` 文件，配置该文件即可。

```
vim .bashrc
```

`shift + G` 跳到最后一行，按 `o` 插入。

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/Linux/lesson1.6/library/lib
```

`:wq` 保存并退出：`esc` 键退出 -> `:` (符号输入)-> `wq` (保存退出)

使得修改生效：

```
. .bashrc
// 或者
source .bashrc
```

在 library 用 ldd 命令检查动态库依赖关系：

```
ldd main
```

这时候发现 `libcalc.so` 查找到了路径：

```
linux-vdso.so.1 =>  (0x00007ffe8efa3000)
libcalc.so => /root/Linux/lesson1.6/library/lib/libcalc.so(0x00007f907702f000)
libc.so.6 => /lib64/libc.so.6 (0x00007f9076c65000)
/lib64/ld-linux-x86-64.so.2 (0x00005597336f6000)
```



系统级别的设置：

```
sudo vim /etc/profile
```

```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/root/Linux/lesson1.6/library/lib
```

```
source /etc/profile
```

```
echo $LD_LIBRARY_PATH
```

发现拼接问题：

```
:/root/Linux/lesson1.6/library/lib:/root/Linux/lesson1.6/library/lib:/root/Linux/lesson1.6/library/lib
```

这是因为前面设置了用户级别的环境变量，导致出错，重新打开 shell 即可。



##### 修改**/etc/ld.so.cache**文件

```
vim /etc/ld.so.cache
```

发现是二进制文件，无法修改，需要间接地修改。

```
sudo vim /etc/ld.so.conf
```

写入路径：

```
/root/Linux/lesson1.6/library/lib
```

更新：

```
sudo ldconfig
```



##### 放到**/lib/**或/usr/lib下

不建议用这种方法，因为该目录下有很多系统库，如果自己的库与系统库重名，放进去就会把系统库覆盖掉。



## 静态库和动态库的对比

### 程序编译成可执行程序的过程

![image-20220326152148290](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174300.png)



静态库、动态库区别来自链接阶段如何处理，链接成可执行程序。分别称为静态链接方式和动态链接方式。

- 静态库：GCC 进行链接时，会把静态库中代码打包到可执行程序中。
- 动态库：GCC 进行链接时，动态库的代码不会被打包到可执行程序中。
- 程序启动之后，动态库会被动态加载到内存中，通过 ldd (list dynamic dependencies)命令检查动态库依赖关系。
- 如何定位共享库文件呢：当系统加载可执行代码时候，能够知道其所依赖的库的名字，但是还需要知道绝对路 径。此时就需要系统的动态载入器来获取该绝对路径。对于elf格式的可执行程序，是 由ld-linux.so来完成的，它先后搜索elf文件的 **DT_RPATH**段 ——> **环境变量** **LD_LIBRARY_PATH** ——> **/etc/ld.so.cache**文件列表 ——> **/lib/**，/usr/lib 目录找到库文件后将其载入内存。



### 静态库制作过程

![image-20220326152227377](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174308.png)

### 动态库制作过程

![image-20220326152247231](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174318.png)



### 静态库的优缺点

优点:

- 静态库被打包到应用程序中加载速度快
- 发布程序无需提供静态库，移植方便

缺点:

- 消耗系统资源，浪费内存
- 更新、部署、发布麻烦

![image-20220326152407161](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174327.png)

### 动态库的优缺点

优点:

- 可以实现进程间资源共享(共享库) 
- 更新、部署、发布简单
- 可以控制何时加载动态库

缺点：

- 加载速度比静态库慢
- 发布程序时需要提供依赖的动态库

![image-20220326152509018](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174336.png)

所以，库较小时建议制作静态库，库较大时建议制作动态库。



# Makefile

## 什么是 Makefile

一个工程中的源文件不计其数，其按类型、功能、模块分别放在若干个目录中， Makefile 文件定义了一系列的规则来指定哪些文件需要先编译，哪些文件需要后编译，哪些文件需要重新编译，甚至于进行更复杂的功能操作，因为 Makefile 文件就 像一个 Shell 脚本一样，也可以执行操作系统的命令。

Makefile 带来的好处就是“自动化编译” ，一旦写好，只需要一个 make 命令，整 个工程完全自动编译，极大的提高了软件开发的效率。make 是一个命令工具，是一个 解释 Makefile 文件中指令的命令工具，一般来说，大多数的 IDE 都有这个命令， 比如 Delphi 的 make，Visual C++ 的 nmake，Linux 下 GNU 的 make。

## Makefile 文件命名和规则

文件命名：makefile 或者 Makefile

Makefile 规则：

- 一个 Makefile 文件中可以有一个或者多个规则

```
目标 ...: 依赖 ...
    命令(Shell 命令)
    ...
```

目标:最终要生成的文件(伪目标除外)

依赖:生成目标所需要的文件或是目标

命令:通过执行命令对依赖操作生成目标(命令前必须 Tab 缩进)

- Makefile 中的其它规则一般都是为第一条规则服务的。

make默认只执行Makefile第一条语句，如果其他语句与第一条无关，则不执行。

### 通过 makefile 自动化编译工程

创建 makefile 文件

```
vim Makefile
```

编辑 makefile ：

```makefile
app: sub.c add.c mult.c div.c main.c
        gcc sub.c add.c mult.c div.c main.c -o app
```

`make` 命令后，找到当前目录下的 makefile 去自动执行命令，生成了 app。

```
make
-> gcc sub.c add.c mult.c div.c main.c -o app
```

### 工作原理

- 命令在执行之前，需要先检查规则中的依赖是否存在
  - 如果存在，执行命令
  - 如果不存在，向下检查其它的规则，检查有没有一个规则是用来生成这个依赖的，如果找到了，则执行该规则中的命令。
- 检测更新，在执行规则中的命令时，会比较目标和依赖文件的时间
  - 如果依赖的时间比目标的时间晚，需要重新生成目标。
  - 如果依赖的时间比目标的时间早，目标不需要更新，对应规则中的命令不需要被执行。

案例：

编辑 makefile 文件。

```makefile
app: sub.o add.o mult.o div.o main.o
        gcc sub.o add.o mult.o div.o main.o -o app

sub.o:sub.c
        gcc -c sub.c -o sub.o

add.o:add.c
        gcc -c add.c -o add.o

mult.o:mult.c
        gcc -c mult.c -o mult.o

div.o:div.c
        gcc -c div.c -o div.o

main.o:main.c
        gcc -c main.c -o main.o
```

```
make
->
gcc -c sub.c -o sub.o
gcc -c add.c -o add.o
gcc -c mult.c -o mult.o
gcc -c div.c -o div.o
gcc -c main.c -o main.o
gcc sub.o add.o mult.o div.o main.o -o app
```

与前面的 makefile 写法对比：

```makefile
app: sub.c add.c mult.c div.c main.c
        gcc sub.c add.c mult.c div.c main.c -o app
```

当 .c 文件更新时，makefile 在执行规则中的命令时，会比较目标和依赖文件的时间，如果依赖的时间比目标的时间晚，会更新目标。这种写法一旦更新某一个文件，都会执行该命令，重新编译所有的 .o 文件，再链接更新 app 。而分规则的写法，只会重新编译有修改的 .o 文件，再链接生成 app，效率更高。



## 变量

- 自定义变量

  `变量名=变量值 `  `var=hello`

- 预定义变量

| 预定义变量 |                                 |
| ---------- | ------------------------------- |
| AR         | 归档维护程序的名称，默认值为 ar |
| CC         | C 编译器的名称，默认值为 cc     |
| CXX        | C++ 编译器的名称，默认值为 g++  |
| $@         | 目标的完整名称                  |
| $<         | 第一个依赖文件的名称            |
| $^         | 所有的依赖文件                  |

- 获取变量的值 

  `$(变量名)`  `$(var)`



用变量改写：

```makefile
app:main.c a.c b.c
			gcc -c main.c a.c b.c
```

```makefile
$(CC) -c $^ -o $@
# 自动变量只能在规则的命令中使用 app:main.c a.c b.c
```

```makefile
# 定义变量
src=sub.o add.o mult.o div.o main.o
target=app
$(target):$(src)
        $(CC) $(src) -o $(target)
        
sub.o:sub.c
        gcc -c sub.c -o sub.o

add.o:add.c
        gcc -c add.c -o add.o

mult.o:mult.c
        gcc -c mult.c -o mult.o

div.o:div.c
        gcc -c div.c -o div.o

main.o:main.c
        gcc -c main.c -o main.o       		 
```



### 模式匹配

```makefile
%.o:%.c
```

- %: 通配符，匹配一个字符串
- 两个%匹配的是同一个字符串

```makefile
%.o:%.c
    gcc -c $< -o $@
```

```makefile
%.o:%.c
    $(CC) -c $< -o $@
```

- `$<` 为第一个依赖的名称，即 `%.c`
- `$@` 为目标的完整名称，即 `%.o` 

改写后：

```makefile
# 定义变量
src=sub.o add.o mult.o div.o main.o
target=app
$(target):$(src)
        $(CC) $(src) -o $(target)
        
%.o:%.c
        $(CC) -c $< -o $@
```



## 函数

查找指定文件：

```makefile
$(wildcard PATTERN...)
```

- 功能：获取指定目录下指定类型的文件列表。

- 参数：PATTERN 指的是某个或多个目录下的对应的某种类型的文件，如果有多个目录，一般使用空格间隔。

- 返回：得到的若干个文件的文件列表，文件名之间使用空格间隔

- 示例：

```makefile
$(wildcard *.c ./sub/*.c) 
# 返回值格式: a.c b.c c.c d.c e.c f.c
```



查找指定文件并替换：

```makefile
$(patsubst <pattern>,<replacement>,<text>)
```

- 功能：查找<text>中的单词(单词以“空格”、“Tab”或“回车”“换行”分隔)是否符合

  模式<pattern>，如果匹配的话，则以<replacement>替换。

- <pattern>可以包括通配符`%`，表示任意长度的字串。如果<replacement>

  中也包含`%`，那么，<replacement>中的这个`%`将是<pattern>中的那个%

  所代表的字串。(可以用`\`来转义，以`\%`来表示真实含义的`%`字符)

- 返回：函数返回被替换过后的字符串

- 示例：

```makefile
$(patsubst %.c, %.o, x.c bar.c) 
#返回值格式: x.o bar.o
```



优化后：

```makefile
# 定义变量
# add.c sub.c main.c mult.c div.c
src=$(wildcard ./*.c)
objs=$(patsubst %.c, %.o, $(src))

target=app
$(target):$(objs)
        $(CC) $(objs) -o $(target)

%.o:%.c
        $(CC) -c $< -o $@
```



编译链接后不需要 .o 文件，可以删除，在 makefile 文件里后面加上一条规则。

```makefile
# 定义变量
# add.c sub.c main.c mult.c div.c
src=$(wildcard ./*.c)
objs=$(patsubst %.c, %.o, $(src))

target=app
$(target):$(objs)
        $(CC) $(objs) -o $(target)


%.o:%.c
        $(CC) -c $< -o $@

clean:
        rm $(objs) -f
```

可以直接执行命令删除：

```
make clean
```

 `clean` 规则与第一条规则无关，执行 `make` 时不会执行 `clean` 规则。

但当有一个名为 clean 的文件时，因为`clean` 没有依赖，执行 `make clean`与名为 clean 的文件对比而更新 clean 文件，导致无法执行 `clean` 规则。

将 `clean` 定义为伪目标，这样不会生成特定的文件，就不会与名为 clean 的文件对比，而无法执行 `clean` 规则。

```makefile
.PHONY:clean
clean:
        rm $(objs) -f
```



# GDB

## 什么是 GDB

- GDB 是由 GNU 软件系统社区提供的调试工具，同 GCC 配套组成了一套完整的开发环 境，GDB 是 Linux 和许多类 Unix 系统中的标准开发环境。

- 一般来说，GDB 主要帮助你完成下面四个方面的功能:
  1. 启动程序，可以按照自定义的要求随心所欲的运行程序。
  2. 可让被调试的程序在所指定的调置的断点处停住(断点可以是条件表达式) 。
  3. 当程序被停住时，可以检查此时程序中所发生的事。
  4. 可以改变程序，将一个 BUG 产生的影响修正从而测试其他 BUG。

## 准备工作

- 通常，在为调试而编译时，我们会()关掉编译器的优化选项(`-O`)， 并打开调试选项(`-g`)。另外，`-Wall` 在尽量不影响程序行为的情况下选项打开所有 warning，也可以发现许多问题，避免一些不必要的 BUG。

```
gcc -g -Wall program.c -o program
```

- ` -g` 选项的作用是在可执行文件中加入源代码的信息，比如可执行文件中第几条机器指令对应源代码的第几行，但并不是把整个源文件嵌入到可执行文件中，所以在调试时必须保证 gdb 能找到源文件。



实操：

编译程序时加入调试信息：

```
gcc test.c -o test -g 
```

（出现错误：只允许在 C99 模式下使用‘**for**’循环初始化声明）

按照提示修改：使用 -std=c99 或 -std=gnu99 来编译您的代码

```
gcc test.c -o test -g -std=gnu99
```

执行GDB，进入到 GDB 模式，gdb 是 shell 命令：

```
gdb test
```

在 GDB 模式下，给程序设置参数：



## GDB 命令

### 启动、退出、查看代码

启动和退出

```
gdb 可执行程序
quit
```

给程序设置参数/获取设置参数

```
set args 10 20
show args
```

GDB使用帮助

```
help
```

查看当前文件代码

```
list/l					// 从默认位置显示
list/l 行号			 // 从指定的行显示
list/l 函数名		// 从指定的函数显示
```

查看非当前文件代码 

```
list/l 文件名:行号 
list/l 文件名:函数名
```

设置显示的行数

```
show list/listsize 
set list/listsize 行数
```

### 断点操作

设置断点 

```
b/break 行号
b/break 函数名 
b/break 文件名:行号 
b/break 文件名:函数
```

查看断点

```
 i/info b/break
```

删除断点

```
 d/del/delete 断点编号
```

设置断点无效 

```
dis/disable 断点编号
```

设置断点生效 

```
ena/enable 断点编号
```

设置条件断点(一般用在循环的位置) 

```
b/break 10 if i==5
```

### 调试命令

运行GDB程序

```
start(程序停在第一行) 
run(遇到断点才停)
```

继续运行，到下一个断点停

```
c/continue
```

向下执行一行代码(不会进入函数体) 

```
n/next
```

变量操作

```
 p/print 变量名(打印变量值) 
 ptype 变量名(打印变量类型)
```

向下单步调试(遇到函数进入函数体) 

```
s/step
finish(跳出函数体)
```

自动变量操作

```
display 变量名(自动打印指定变量的值) 
i/info display
undisplay 编号
```

其它操作

```
set var 变量名=变量值 (循环中用的较多) 
until (跳出循环)
```

# 文件IO

[1.17 文件IO](课件/1.17%20文件IO.pdf)

## 标准C库IO函数和Linux系统IO函数对比

![image-20220328130316589](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174357.png)

![image-20220328130330587](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174406.png)



## 虚拟地址空间

![image-20220328130352951](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174413.png)

程序和进程的区别：

程序是可执行文件，可执行文件只占用磁盘的空间，不占用内存空间。

当程序运行起来后，操作系统会为程序的启动或运行分配一些资源，这时候操作系统就会创建一个进程，进程就是为程序运行所分配资源的东西。

进程就是正在运行的程序，进程占用内存空间，一个程序启动以后，会生成一个虚拟地址空间，虚拟地址空间会通过 CPU 当中的 MMU 逻辑管理单元把虚拟地址空间中的数据映射到物理内存中去。



## 文件描述符

![image-20220328130409222](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174423.png)



对可执行文件进行读取或写的操作，可执行程序运行起来后，操作系统会生成一个进程，在对应的文件比如 english.txt ，进行读取或写入等操作，用标准 C 的 IO 文件库去操作，fopen 打开文件，调用 fread 读取数据，或者调用 fwrite 写入数据，为什么在程序当中用 fopen 打开文件后，调用 fread ，或者调用 fwrite ，它就能够去找到磁盘上的 english.txt 文件，对它进行操作呢？

因为它里面有文件描述符，调用 fopen 函数，会返回一个 `FILE * fp` 文件指针，这个文件指针里就封装了文件描述符，文件描述符就是用来定位要操作的文件的。

文件描述符在文件的内核区(3G - 4G)，由内核进行管理，在内核区的进程管理模块里 有一个 PCB 进程控制块，由它管理文件描述符。

内核就是一个程序，PCB 是一个非常复杂的结构体，PCB 是进程控制块，需要管理很多东西，可以把需要管理的数据都封装到 PCB 结构体里。PCB 结构体里有一个数组，这个数组就是文件描述符表，这个数组就用来存储很多个文件描述符文件。

为什么用数组来存储很多个文件描述符呢？

文件描述符就是用来定位磁盘中的某个文件的。一个进程可以同时打开多个文件，所以要用数组来存储多个文件描述符，这样才可以同时定位多个文件。

这个数组的默认大小是 1024，每一个进程里都有一个进程控制块，都有一个文件描述符表，每个进程默认能打开的文件个数就是 1024。

这个文件描述符表的前三个是默认被占用的，默认被 标准输入、标准输出、标准错误 使用，而且默认是打开的状态，它们对应的文件就是当前终端，为什么都指向当前的终端？Linux 系统一切皆文件，不管是什么硬件设备，它最终都会虚拟成一个文件，通过文件对它进行管理，这三个也就是设备文件。

一个文件可以被打开多次，但是打开多次的文件描述符是不一样的。

用 fclose 关闭文件描述符的时候，这个文件描述符就会被释放，被释放后就可以重新去使用，如果文件描述符被占用，它需要到文件描述符表里找一个最小的没有被占用的，文件描述符去使用。

## Linux系统IO函数

```
int open(const char *pathname, int flags);
int open(const char *pathname, int flags, mode_t mode);
int close(int fd);
ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);
off_t lseek(int fd, off_t offset, int whence);
int stat(const char *pathname, struct stat *statbuf);
int lstat(const char *pathname, struct stat *statbuf);
```

### open函数

centOS安装manpages

```
yum -y install man-pages
```

查看说明文档

```
man 2 open
// 第二章是Linux系统函数说明文档，open 在第二章查找
// 第三章是标准C库说明文档
```

![image-20220403111757618](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174431.png)



open 函数：

```c
// 头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// open 函数的声明
// 打开一个已经存在的文件
int open(const char *pathname, int flags);

// 创建一个新的文件
int open(const char *pathname, int flags, mode_t mode);
```

为什么要有三个头文件？

函数的申明在 `fcntl.h` 头文件里，`flags` 在Linux系统中定义为一个宏放在其他的头文件里。

为什么可以有两个同名的 `open` 函数？

C语言没有函数重载，通过可变参数 `mode_t mode` 实现同名函数的效果。

#### open打开文件

open函数打开文件的参数和返回值详解：

```c
// 头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// open 函数的声明
// 打开一个已经存在的文件
int open(const char *pathname, int flags);
		参数：
				- pathname：要打开的文件路径
				- flags：对文件的操作权限设置还有其他设置
						- O_RDONLY：只读
						- O_WRONLY：只写
						- O_RDWR：可读可写
						这三个设置是互斥的(必选项)
		返回值：返回一个新的文件描述符，如果调用失败，返回 -1 ，系统会把错误号赋值给errno
		errno：属于Linux系统函数库，库里面的一个全局变量，记录的是最近的错误号
		perror：查看具体的错误

// 创建一个新的文件
int open(const char *pathname, int flags, mode_t mode);
```

查看返回值

```
/return value
```

![image-20220403113343555](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174505.png)

perror 函数（说明文档 `man 3 perror` ）

```c
#include <stdio.h>
void perror(const char *s); 
		作用：打印 errno 对应的错误描述
		s 参数：用户描述，比如 hello，最终输出的内容是 hello:xxx(实际的错误描述)
```

close 函数（说明文档 `man 2 close` ）

```c
#include <unistd.h>
int close(int fd);
```

查找 `a.txt` 文件，找不到文件，文件描述符返回 -1，用 `perror` 打印错误描述，关闭文件

```c
// open.c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  	// 打开一个文件
    int fd = open("a.txt", O_RDONLY);
    if(fd == -1) {
        perror("open");
    }
    // 关闭
    close(fd);
    return 0;
}
```

编译 `open.c` 文件 `gcc open.c -o open` ，执行输出错误描述：

```
open: No such file or directory
```

#### open创建新文件

```c
// 头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

// open 函数的声明
// 创建一个新的文件
int open(const char *pathname, int flags, mode_t mode);
		参数：
				- pathname：要打开的文件路径
				- flags：对文件的操作权限设置还有其他设置
						- 必选项：O_RDONLY，O_WRONLY，O_RDWR，这三个设置是互斥的
						- 可选项：O_CREAT 文件不存在，创建新文件
      	- mode：八进制的数，表示用户对创建出的新的文件的操作权限，比如：0775
      	最终的权限是：mode & ~umask
      	umask -> 0002, ~umaske = 0777-0002 = 0775
      	0777	-> 111111111
      & 0775	-> 111111101
      ---------------------
      					 111111101	-> 0775
      按位与：0 和任何数都为 0
      umask 的作用是抹去某些权限，umask 可以自己设计
		返回值：返回一个新的文件描述符，如果调用失败，返回 -1 ，系统会把错误号赋值给errno
		errno：属于Linux系统函数库，库里面的一个全局变量，记录的是最近的错误号
		perror：查看具体的错误


```



```c
// create.c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main() {
  	// 创建一个新的文件
    int fd = open("create.txt", O_RDWR | O_CREAT, 0777);
    if(fd == -1) {
        perror("open");
    }
    // 关闭
    close(fd);
    return 0;
}
```

编译 `create.c` 文件 `gcc create.c -o create` ，创建了新文件 `create.txt` 。

查看权限，`ll create.txt` 。

```
-rwxr-xr-x 1 root root 0 4月   3 14:49 create.txt
```

`-rwxr-xr-x` 对应 0775 权限。

open 函数中的 flags 为什么用按位或 `|` 来连接？

flags函数是一个int类型的数据，占4个字节，32位；

flags 32个位，每一位就是一个标志位。用 `|` 相当于把后一个 flag加到前一个flag上。

### read函数和write函数

查看说明文档

```
man 2 read
man 2 write
```

```c
// read 函数
#include <unistd.h>
ssize_t read(int fd, void *buf, size_t count);
		参数：
      	- fd: 文件描述符，open 得到的，通过这个文件描述符操作某个文件
        - buf: 需要读取数据存放的地方，数组的地址（传出参数）
        - count: 指定的数组的大小
    返回值：
        - 成功: 
          	> 0: 返回实际的读取到的字节数
            = 0: 文件已经读取完了
        - 失败: -1 ，并且设置 errno
```

```c
// write 函数
#include <unistd.h>
ssize_t write(int fd, const void *buf, size_t count);
		参数：
      	- fd: 文件描述符，open 得到的，通过这个文件描述符操作某个文件
        - buf: 要往磁盘写入的数据
        - count: 要写的数据的实际的大小
    返回值：
        - 成功: 实际写入的字节数
        - 失败: -1 ，并且设置 errno
```

文件拷贝：

```cpp
// copyfile.c
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // 1. 通过 open 打开 english.txt 文件
    int srcfd = open("english.txt", O_RDONLY);  // 源文件的文件描述符
    if (srcfd == -1){
        perror("open");
        return -1;
    }
    // 2. 创建一个心的文件（拷贝文件）
    int destfd = open("cpy.txt", O_WRONLY | O_CREAT, 0664); // 目标文件的描述符
    if (destfd == -1){
        perror("open");
        return -1;
    }
    // 3. 频繁的读写操作 
    char buf[1024] = {0};
    int len = 0;
    while((len = read(srcfd, buf, sizeof(buf))) > 0) {
        write(destfd, buf, len);
    }

    // 4. 关闭文件
    close(destfd);
    close(srcfd);
    return 0;
}
```

### lseek函数

查看说明文档

```
man 2 lseek
man 3 fseek
```

```c
// 标准C库的函数
#include <stdio.h>
int fseek(FILE *stream, long offset, int whence);

// Linux系统函数
#include <sys/types.h>
#include <unistd.h>
off_t lseek(int fd, off_t offset, int whence);
		- 参数：
      	- fd: 文件描述符，通过 open 得到的，通过这个 fd 操作某个文件
        - offset: 偏移量
        - whence: 
						SEEK_SET
              设置文件指针的偏移量
            SEEK_CUR
              设置偏移量：当前位置 + 第二个参数 offset 的值
            SEEK_END
              设置偏移量：文件大小 + 第二个参数 offset 的值
		- 返回值：返回文件指针的位置
              
作用：
		1. 移动文件指针到头文件
    lseek(fd, 0, SEEK_SET);
		2. 获取当前文件指针的位置
    lseek(fd, 0, SEEK_CUR);
		3. 获取文件的长度
   	lseek(fd, 0, SEEK_END);
		4. 拓展文件的长度，当前文件 10b，110b，增加了 100 个字节
    lseek(fd, 100, SEEK_END);
```



拓展文件长度：

```c
// lseek.c
// 原 hello.txt 为 12 字节，拓展了 100 个字节，写入了 1 个空数据，最后为 113 字节大小
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    // 打开文件
    int fd = open("hello.txt", O_RDWR);
    if (fd == -1) {
        perror("open");
        return -1;
    }
    //拓展文件
    int ret = lseek(fd, 100, SEEK_END);   // 从文件末尾开始拓展 100 字节
    if (ret == -1) {
        perror("lseek");
        return -1;
    }

    // 写入一个空数据
    write(fd, " ", 1);

    // 关闭文件
    close(fd);
    return 0;
}
```

为什么要写入空数据？

lseek()不是扩展数据的功能，只是可以利用lseek()去扩展文件，lseek()只是单纯的移动文件指针偏移。而写入数据后才能扩展数据大小，显示出字节数。

拓展文件有什么实际用途？

在下载文件时，先按照文件大小拓展出来，然后再慢慢往里面下载，以防磁盘空间不足。

### stat函数和lstat函数

查看说明文档

```
man 2 stat
```



```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int stat(const char *path, struct stat *buf);
		作用：获取一个文件相关的一些信息
    参数：
      	- pathname：操作的文件的路径
        - statbuf：结构体变量，传出参数，用于保存获取到的文件的信息
    返回值：
      	成功：返回 0
      	失败：返回 -1 ，设置 errno
int lstat(const char *path, struct stat *buf);
		作用：获取软链接文件相关的一些信息
    参数：
      	- pathname：操作的文件的路径
        - statbuf：结构体变量，传出参数，用于保存获取到的文件的信息
    返回值：
      	成功：返回 0
      	失败：返回 -1 ，设置 errno
```

在 Linux 中也有一个命令 stat ，通过它可以查看一个文件的信息。

```
stat a.txt
```

```
  文件："a.txt"
  大小：13        	块：8          IO 块：4096   普通文件
设备：fd01h/64769d	Inode：658603      硬链接：1
权限：(0644/-rw-r--r--)  Uid：(    0/    root)   Gid：(    0/    root)
最近访问：2022-04-03 16:07:04.823402070 +0800
最近更改：2022-04-03 16:07:04.823402070 +0800
最近改动：2022-04-03 16:07:04.826402123 +0800
创建时间：-
```

stat结构体

```c
struct stat {
		dev_t		st_dev;						// 文件的设备编号
    ino_t		st_ino;						// 节点
		mode_t		st_mode;				// 文件的类型和存取的权限
		nlink_t		st_nlink;				// 连到该文件的硬连接数目
    uid_t			st_uid;					// 用户ID
    gid_t			st_gid;					// 组ID
    dev_t			st_rdev;				// 设备文件的设备编号
    off_t			st_size;				// 文件字节数(文件大小)
    blksize_t		st_blksize;		// 块大小
    blkcnt_t 		st_blocks;		// 块数
    time_t 			st_atime;			// 最后一次访问时间
    time_t 			st_mtime;			// 最后一次修改时间
    time_t			st_ctime;			// 最后一次改变时间(指属性)
 };
```

st_mode变量

![image-20220403162503909](https://cdn.jsdelivr.net/gh/huyup1e2n3g/WebServer/img/20220403174524.png)

获取 a.txt 的信息，返回大小

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct stat statbuf;
    int ret = stat("a.txt", &statbuf);
    if(ret == -1){
        perror("stat");
        return -1;
    }
    printf("size: %ld\n", statbuf.st_size);
    return 0;
}
```

创建软链接

```
ln -s a.txt b.txt
```

创建出软链接 b.txt ，指向了 a.txt

```
lrwxrwxrwx 1 root root    5 Apr  3 16:41 b.txt -> a.txt
```

用 `stat b.txt` 查看信息

```
  File: ‘b.txt’ -> ‘a.txt’
  Size: 5               Blocks: 0          IO Block: 4096   symbolic link
Device: fd01h/64769d    Inode: 658601      Links: 1
Access: (0777/lrwxrwxrwx)  Uid: (    0/    root)   Gid: (    0/    root)
Access: 2022-04-03 16:41:44.509931636 +0800
Modify: 2022-04-03 16:41:44.412929931 +0800
Change: 2022-04-03 16:41:44.412929931 +0800
 Birth: -
```

用 `stat` 获取软链接文件的信息，其实获取的是软链接 b.txt 指向的 a.txt 的信息，如果要获取 软链接 b.txt 的信息就要用 `lstat` 。

### 模拟实现 ls -l 命令

模拟实现 ls-l 命令的程序代码：

[ls-l.c](https://github.com/huyup1e2n3g/WebServer/blob/main/code/lesson1.25/ls-l.c)

### 文件属性操作函数

```c
// 判断文件的权限或判断文件是否存在
int access(const char *pathname, int mode);
// 修改文件的权限
int chmod(const char *filename, int mode);
// 修改文件的所有者或者所在组
int chown(const char *path, uid_t owner, gid_t group);
// 缩减或扩展文件的大小
int truncate(const char *path, off_t length);
```

查看说明文档：

```
man 2 access
man 2 chmod
man 2 chown
man 2 truncate
```

#### access函数

```c
#include <unistd.h>
int access(const char *pathname, int mode);
		作用：判断某个文件是否有某个权限，或者判断文件是否存在
    参数：
      - pathname：判断的文件路径
      - mode：
      	R_OK: 判断是否有读权限
        W_OK: 判断是否有写权限
        X_OK: 判断是否有执行权限
        F_OK: 判断文件是否存在
		返回值：
      成功：返回 0
      失败：返回 -1
```

```c
// access.c
#include <unistd.h>
#include <stdio.h>
int main() {
    // 判断文件是否存在
    int ret = access("a.txt", F_OK);
    if(ret == -1){
        perror("access");
    }
    printf("文件存在\n");
    return 0;
}
```

#### chmod函数

```c
#include <sys/stat.h>
int chmod(const char *path, mode_t mode);
		作用：修改文件的权限
    参数：
      - pathname: 需要修改的文件的路径
      - mode_t: 需要修改的权限值，八进制的数
    返回值：
      成功：返回 0
      失败：返回 -1
        
```

```c
// chmod.c
#include <sys/stat.h>
#include <stdio.h>
int main(){
  	// 修改文件权限
    int ret = chmod("a.txt", 0775);
    if(ret == -1){
        perror("chmod");
        return -1;
    }
    return 0;
}
```

修改前：

```
-rw-r--r-- 1 root root   14 Apr  4 15:52 a.txt
```

修改后：

```
-rwxrwxr-x 1 root root   14 Apr  4 15:52 a.txt
```

#### truncate函数

```c
#include <unistd.h>
#include <sys/types.h>
int truncate(const char *path, off_t length);
		作用：缩减或扩展文件的尺寸至指定的大小
    参数：
      - path: 需要修改的文件的路径
      - length: 需要最终文件变成的大小
    返回值：
      成功：返回 0
      失败：返回 -1
```

```c
// truncate.c
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
int main(){
    // 扩大文件大小
    int ret = truncate("b.txt", 20);
    if(ret == -1){
        perror("truncate");
        return -1;
    }
    return 0;
}
```

扩展前：

```
-rw-r--r-- 1 root root   13 Apr  4 16:03 b.txt
```

扩展后：

```
-rw-r--r-- 1 root root   20 Apr  4 16:03 b.txt
```



### 目录操作函数

```c
// 创建目录
int mkdir(const char *pathname, mode_t mode);
// 删除空目录
int rmdir(const char *pathname);
// 重命名目录
int rename(const char *oldpath, const char *newpath); 
// 更改当前的目录
int chdir(const char *path);
// 获取当前的路径
char *getcwd(char *buf, size_t size);
```

查看说明文档

```
man 2 mkdir
```

#### mkdir函数

```c
#include <sys/stat.h>
#include <sys/types.h>
int mkdir(const char *pathname, mode_t mode);
		作用：创建一个目录
    参数：
      - pathname: 创建的目录的路径
      - mode: 权限，八进制的数
    返回值：
    	成功：返回 0
      失败：返回 -1   
```

```c
// mkdir.c
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
int main(){
    // 创建一个 aaa 目录
    int ret = mkdir("aaa", 0777);
    if(ret == -1){
        perror("mkdir");
        return -1;
    }
    return 0;
}
```

生成 aaa 目录：

```
drwxr-xr-x 2 root root 4096 Apr  4 16:20 aaa
```

#### rename函数

```c
#include <stdio.h>
int rename(const char *oldpath, const char *newpath);
```

```c
// rename.c
#include <stdio.h>
int main(){
    // 将 aaa 目录改名为 bbb
    int ret = rename("aaa", "bbb");
    if(ret == -1){
        perror("rename");
        return -1;
    }
    return 0;   
}
```

#### chdir函数

```c
#include <unistd.h>
int chdir(const char *path);
		作用：修改进程的工作目录
    	比如在 /home/nowcoder 启动了一个可执行程序 a.out ，进程的工作目录就是 /home/nowcoder
    参数：
      - path: 需要修改的工作目录
```

#### getcwd函数

```c
#include <unistd.h>
char *getcwd(char *buf, size_t size);
		作用：获取当前工作目录
    参数：
      - buf: 存储的路径，指向的是一个数组（传出参数）
      - size: 数组的大小
    返回值：
      返回的指向的一块内存，这个数据就是第一个参数
```

```c
// chdir.c
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
    // 获取当前的工作目录
    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("当前的工作目录是：%s\n", buf);

    // 修改工作目录
    int ret = chdir("/root/Linux/lesson1.27/bbb");
    if(ret == -1){
        perror("chdir");
        return -1;
    }

    // 创建一个新的文件
    int fd = open("chdir.txt", O_CREAT | O_RDWR, 0664);
    if(fd == -1){
        perror("open");
        return -1;
    }
    // 关闭文件
    close(fd);

    // 获取当前的工作目录
    char buf1[128];
    getcwd(buf1, sizeof(buf1));
    printf("当前的工作目录是：%s\n", buf1);

    return 0;
}
```



### 目录遍历函数

```c
// 打开目录
DIR *opendir(const char *name);
// 读取目录里的内容
struct dirent *readdir(DIR *dirp);
// 关闭目录
int closedir(DIR *dirp);
```

查看说明文档

```
man 3 opendir
```

#### opendir函数

```c
#include <sys/types.h>
#include <dirent.h>
DIR *opendir(const char *name);
		参数：
      - name: 需要打开的目录的名称
    返回值：
      DIR * 类型，理解为目录流
      错误返回 NULL
```

#### readdir函数

```c
#include <dirent.h>
struct dirent *readdir(DIR *dirp);
		作用：读取目录中的数据‘
    参数：dirp 是 opendir 返回的结果
    返回值：
      	struct dirent 代表读取到的文件的信息
        读取到了末尾或者失败了，返回NULL
```

dirent 结构体

```c
struct dirent {
  	// 此目录进入点的inode
		ino_t		d_ino;       /* inode number */
    // 目录文件开头至此目录进入点的位移
  	off_t   d_off;       /* not an offset; see NOTES */
    // d_name 的长度，不包含 NULL 字符
 		unsigned short d_reclen;    /* length of this record */
    // d_name 所指的文件类型
 		unsigned char  d_type;      /* type of file; not supported by all file system types */
    // 文件名
  	char		d_name[256]; /* filename */
};
```

d_type

```
DT_BLK      This is a block device.			块设备
DT_CHR      This is a character device.	字符设备
DT_DIR      This is a directory.				目录
DT_FIFO     This is a named pipe (FIFO).	软连接
DT_LNK      This is a symbolic link.			管道
DT_REG      This is a regular file.				件
DT_SOCK     This is a UNIX domain socket.	套普通文接字
DT_UNKNOWN  The file type is unknown.			未知
```



#### closedir函数

```c
#include <sys/types.h>
#include <dirent.h>
int closedir(DIR *dirp);
		作用：关闭目录
```

遍历目录下文件数量程序代码：

[readFileNum.c](https://github.com/huyup1e2n3g/WebServer/blob/main/code/lesson1.28/readFileNum.c)



### dup、dup2 函数

```c
// 复制文件描述符
int dup(int oldfd); 
// 重定向文件描述符
int dup2(int oldfd, int newfd); 
```

```c
#include <unistd.h>
int dup(int oldfd);
		作用：复制一个新的文件描述符
    fd = 3, int fd1 = dup(fd);
		fd 指向的是 a.txt，fd1 也是指向 a.txt
    从空闲的文件描述符中找一个最小的，作为新的拷贝的文件描述符
int dup2(int oldfd, int newfd);
  	作用：重定向文件描述符
    oldfd 指向 a.txt, newfd 指向 b.txt
    调用函数成功后：newfd 和 b.txt 做 close, newfd 指向了 a.txt
    oldfd 必须是一个有效的文件描述符
    oldfd 和 newfd 值相同，相当于什么都没有做
```

```c
// dup.c
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main() {
    int fd = open("a.txt", O_RDWR | O_CREAT, 0664);
    int fd1 = dup(fd);
    if(fd1 == -1) {
        perror("dup");
        return -1;
    }
    printf("fd : %d , fd1 : %d\n", fd, fd1);
    close(fd);
    char * str = "hello,world";
    int ret = write(fd1, str, strlen(str));
    if(ret == -1) {
        perror("write");
        return -1;
    }
    close(fd1);
    return 0;
}
```

```c
// dup2.c
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    int fd = open("1.txt", O_RDWR | O_CREAT, 0664);
    if(fd == -1) {
        perror("open");
        return -1;
    }
    int fd1 = open("2.txt", O_RDWR | O_CREAT, 0664);
    if(fd1 == -1) {
        perror("open");
        return -1;
    }
    printf("fd : %d, fd1 : %d\n", fd, fd1);
    int fd2 = dup2(fd, fd1);
    if(fd2 == -1) {
        perror("dup2");
        return -1;
    }
    // 通过fd1去写数据，实际操作的是1.txt，而不是2.txt
    char * str = "hello, dup2";
    int len = write(fd1, str, strlen(str));
    if(len == -1) {
        perror("write");
        return -1;
    }
    printf("fd : %d, fd1 : %d, fd2 : %d\n", fd, fd1, fd2);

    close(fd);
    close(fd1);

    return 0;
}
```



### fcntl 函数

```c
int fcntl(int fd, int cmd, ... /* arg */ ); 
// 复制文件描述符
// 设置/获取文件的状态标志
```

```c
#include <unistd.h>
#include <fcntl.h>
int fcntl(int fd, int cmd, ... /* arg */ );
    参数：
        fd : 表示需要操作的文件描述符
        cmd: 表示对文件描述符进行如何操作
            - F_DUPFD : 复制文件描述符,复制的是第一个参数fd，得到一个新的文件描述符（返回值）
                int ret = fcntl(fd, F_DUPFD);

            - F_GETFL : 获取指定的文件描述符文件状态flag
              获取的flag和我们通过open函数传递的flag是一个东西。

            - F_SETFL : 设置文件描述符文件状态flag
              必选项：O_RDONLY, O_WRONLY, O_RDWR 不可以被修改
              可选性：O_APPEND, O)NONBLOCK
                O_APPEND 表示追加数据
                NONBLOK 设置成非阻塞
        
        阻塞和非阻塞：描述的是函数调用的行为。
```

```c
// fcntl.c
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main() {

    // 1.复制文件描述符
    // int fd = open("1.txt", O_RDONLY);
    // int ret = fcntl(fd, F_DUPFD);

    // 2.修改或者获取文件状态flag
    int fd = open("1.txt", O_RDWR);
    if(fd == -1) {
        perror("open");
        return -1;
    }

    // 获取文件描述符状态flag
    int flag = fcntl(fd, F_GETFL);
    if(flag == -1) {
        perror("fcntl");
        return -1;
    }
    flag |= O_APPEND;   // flag = flag | O_APPEND

    // 修改文件描述符状态的flag，给flag加入O_APPEND这个标记
    int ret = fcntl(fd, F_SETFL, flag);
    if(ret == -1) {
        perror("fcntl");
        return -1;
    }

    char * str = "nihao";
    write(fd, str, strlen(str));

    close(fd);

    return 0;
}
```

