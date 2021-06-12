# LINUX学习笔记

# 系统目录

- **/bin**：bin是Binary的缩写, 这个目录存放着最经常使用的命令。
- **/boot：** 这里存放的是启动Linux时使用的一些核心文件，包括一些连接文件以及镜像文件。
- **/dev ：** dev是Device(设备)的缩写, 存放的是Linux的外部设备，在Linux中访问设备的方式和访问文件的方式是相同的。
- **/etc：** 这个目录用来存放所有的系统管理所需要的配置文件和子目录。
- **/home**：用户的主目录，在Linux中，每个用户都有一个自己的目录，一般该目录名是以用户的账号命名的。
- **/lib**：这个目录里存放着系统最基本的动态连接共享库，其作用类似于Windows里的DLL文件。
- **/lost+found**：这个目录一般情况下是空的，当系统非法关机后，这里就存放了一些文件。
- **/media**：linux系统会自动识别一些设备，例如U盘、光驱等等，当识别后，linux会把识别的设备挂载到这个目录下。
- **/mnt**：系统提供该目录是为了让用户临时挂载别的文件系统的，我们可以将光驱挂载在/mnt/上，然后进入该目录就可以查看光驱里的内容了。
- **/opt**：这是给主机额外安装软件所摆放的目录。比如你安装一个ORACLE数据库则就可以放到这个目录下。默认是空的。
- **/proc**：这个目录是一个虚拟的目录，它是系统内存的映射，我们可以通过直接访问这个目录来获取系统信息。
- **/root**：该目录为系统管理员，也称作超级权限者的用户主目录。
- **/sbin**：s就是Super User的意思，这里存放的是系统管理员使用的系统管理程序。
- **/srv**：该目录存放一些服务启动之后需要提取的数据。
- **/sys**：这是linux2.6内核的一个很大的变化。该目录下安装了2.6内核中新出现的一个文件系统 sysfs 。
- **/tmp**：这个目录是用来存放一些临时文件的。
- **/usr**：这是一个非常重要的目录，用户的很多应用程序和文件都放在这个目录下，类似于windows下的program files目录。
- **/usr/bin：** 系统用户使用的应用程序。
- **/usr/sbin：** 超级用户使用的比较高级的管理程序和系统守护程序。
- **/usr/src：** 内核源代码默认的放置目录。
- **/var**：这个目录中存放着在不断扩充着的东西，我们习惯将那些经常被修改的目录放在这个目录下。包括各种日志文件。
- **/run**：是一个临时文件系统，存储系统启动以来的信息。当系统重启时，这个目录下的文件应该被删掉或清除。

# 关机

```
sync # 将数据由内存同步到硬盘中。

shutdown # 关机指令，你可以man shutdown 来看一下帮助文档。例如你可以运行如下命令关机：

shutdown –h 10 # 这个命令告诉大家，计算机将在10分钟后关机

shutdown –h now # 立马关机

shutdown –h 20:25 # 系统会在今天20:25关机

shutdown –h +10 # 十分钟后关机

shutdown –r now # 系统立马重启

shutdown –r +10 # 系统十分钟后重启

reboot # 就是重启，等同于 shutdown –r now

halt # 关闭系统，等同于shutdown –h now 和 poweroff
```

# 创建文件

touch

# 文件、目录操作

## ls: 列出目录

-a ：全部的文件，连同隐藏文件( 开头为 . 的文件) 一起列出来(常用)

-l ：长数据串列出，包含文件的属性与权限等等数据；(常用)

## cd：切换目录

## pwd：显示目前的目录

```
# 单纯显示出目前的工作目录
[root@kuangshen ~]# pwd
/root

# 如果是链接，要显示真实地址，可以使用 -P参数
[root@kuangshen /]# cd bin
[root@kuangshen bin]# pwd -P
/usr/bin
```

## mkdir：创建一个新的目录

```
# 进入我们用户目录下
[root@kuangshen /]# cd /home

# 创建一个 test 文件夹
[root@kuangshen home]# mkdir test

# 创建多层级目录
[root@kuangshen home]# mkdir test1/test2/test3/test4
mkdir: cannot create directory ‘test1/test2/test3/test4’:
No such file or directory  # <== 没办法直接创建此目录啊！

# 加了这个 -p 的选项，可以自行帮你创建多层目录！
[root@kuangshen home]# mkdir -p test1/test2/test3/test4

# 创建权限为 rwx--x--x 的目录。
[root@kuangshen home]# mkdir -m 711 test2
[root@kuangshen home]# ls -l
drwxr-xr-x 2 root root  4096 Mar 12 21:55 test
drwxr-xr-x 3 root root  4096 Mar 12 21:56 test1
drwx--x--x 2 root root  4096 Mar 12 21:58 test2
```

## rmdir：删除一个空的目录

```
# 看看有多少目录存在？
[root@kuangshen home]# ls -l
drwxr-xr-x 2 root root  4096 Mar 12 21:55 test
drwxr-xr-x 3 root root  4096 Mar 12 21:56 test1
drwx--x--x 2 root root  4096 Mar 12 21:58 test2

# 可直接删除掉，没问题
[root@kuangshen home]# rmdir test

# 因为尚有内容，所以无法删除！
[root@kuangshen home]# rmdir test1
rmdir: failed to remove ‘test1’: Directory not empty

# 利用 -p 这个选项，立刻就可以将 test1/test2/test3/test4 依次删除。
[root@kuangshen home]# rmdir -p test1/test2/test3/test4
```



## cp: 复制文件或目录

```
# 找一个有文件的目录，我这里找到 root目录
[root@kuangshen home]# cd /root
[root@kuangshen ~]# ls
install.sh
[root@kuangshen ~]# cd /home

# 复制 root目录下的install.sh 到 home目录下
[root@kuangshen home]# cp /root/install.sh /home
[root@kuangshen home]# ls
install.sh

# 再次复制，加上-i参数，增加覆盖询问？
[root@kuangshen home]# cp -i /root/install.sh /home
cp: overwrite ‘/home/install.sh’? y # n不覆盖，y为覆盖
```



## rm: 移除文件或目录

-f ：就是 force 的意思，忽略不存在的文件，不会出现警告信息；

-i ：互动模式，在删除前会询问使用者是否动作

-r ：递归删除啊！最常用在目录的删除了！这是非常危险的选项！！！

```
# 将刚刚在 cp 的实例中创建的 install.sh删除掉！
[root@kuangshen home]# rm -i install.sh
rm: remove regular file ‘install.sh’? y
# 如果加上 -i 的选项就会主动询问喔，避免你删除到错误的档名！

# 尽量不要在服务器上使用 rm -rf /
```



## mv: 移动文件与目录，或修改文件与目录的名称

```
[root@www ~]# mv [-fiu] source destination
[root@www ~]# mv [options] source1 source2 source3 .... directory
```

-f ：force 强制的意思，如果目标文件已经存在，不会询问而直接覆盖；

-i ：若目标文件 (destination) 已经存在时，就会询问是否覆盖！

-u ：若目标文件已经存在，且 source 比较新，才会升级 (update)

```
# 复制一个文件到当前目录
[root@kuangshen home]# cp /root/install.sh /home

# 创建一个文件夹 test
[root@kuangshen home]# mkdir test

# 将复制过来的文件移动到我们创建的目录，并查看
[root@kuangshen home]# mv install.sh test
[root@kuangshen home]# ls
test
[root@kuangshen home]# cd test
[root@kuangshen test]# ls
install.sh

# 将文件夹重命名，然后再次查看！
[root@kuangshen test]# cd ..
[root@kuangshen home]# mv test mvtest
[root@kuangshen home]# ls
mvtest
```

# 文件属性

![微信图片_20210303211345](D:\操作系统\微信图片_20210303211345.jpg)

- 当为[ **d** ]则是目录
- 当为[ **-** ]则是文件；
- 若是[ **l** ]则表示为链接文档 ( link file )；
- 若是[ **b** ]则表示为装置文件里面的可供储存的接口设备 ( 可随机存取装置 )；
- 若是[ **c** ]则表示为装置文件里面的串行端口设备，例如键盘、鼠标 ( 一次性读取装置 )。

![微信图片_20210303211334](D:\操作系统\微信图片_20210303211334.jpg)

# 文件内容查看

## cat 由第一行开始显示文件内容

- -A ：相当於 -vET 的整合选项，可列出一些特殊字符而不是空白而已；

- -b ：列出行号，仅针对非空白行做行号显示，空白行不标行号！

- -E ：将结尾的断行字节 $ 显示出来；

- -n ：列印出行号，连同空白行也会有行号，与 -b 的选项不同；

- -T ：将 [tab] 按键以 ^I 显示出来；

- -v ：列出一些看不出来的特殊字符

  ```
  # 查看网络配置: 文件地址 /etc/sysconfig/network-scripts/
  [root@kuangshen ~]# cat /etc/sysconfig/network-scripts/ifcfg-eth0
  DEVICE=eth0
  BOOTPROTO=dhcp
  ONBOOT=yes
  ```

  

## tac 

从最后一行开始显示，可以看出 tac 是 cat 的倒着写！

##  nl  显示的时候，顺道输出行号！

- -b ：指定行号指定的方式，主要有两种：-b a ：表示不论是否为空行，也同样列出行号(类似 cat -n)；-b t ：如果有空行，空的那一行不要列出行号(默认值)；
- -n ：列出行号表示的方法，主要有三种：-n ln ：行号在荧幕的最左方显示；-n rn ：行号在自己栏位的最右方显示，且不加 0 ；-n rz ：行号在自己栏位的最右方显示，且加 0 ；
- -w ：行号栏位的占用的位数

##  more 一页一页的显示文件内容

- 空白键 (space)：代表向下翻一页；
- Enter   ：代表向下翻『一行』；
- /字串   ：代表在这个显示的内容当中，向下搜寻『字串』这个关键字；
- :f    ：立刻显示出档名以及目前显示的行数；
- q    ：代表立刻离开 more ，不再显示该文件内容。
- b 或 [ctrl]-b ：代表往回翻页，不过这动作只对文件有用，对管线无用

##  less 与 more 类似，但是比 more 更好的是，他可以往前翻页！

- 空白键  ：向下翻动一页；
- [pagedown]：向下翻动一页；
- [pageup] ：向上翻动一页；
- /字串  ：向下搜寻『字串』的功能；
- ?字串  ：向上搜寻『字串』的功能；
- n   ：重复前一个搜寻 (与 / 或 ? 有关！)
- N   ：反向的重复前一个搜寻 (与 / 或 ? 有关！)
- q   ：离开 less 这个程序；

##  head 只看头几行

```
[root@kuangshen etc]# head -n 20 /etc/csh.login
```



##  tail 只看尾巴几行

# 四种安装方式

## rpm安装

## yum安装

### 安装Docker（yum安装）

> 基于 CentOS 7 安装

1. 官网安装参考手册：https://docs.docker.com/install/linux/docker-ce/centos/

2. 确定你是CentOS7及以上版本

   ```
   [root@192 Desktop]# cat /etc/redhat-release
   CentOS Linux release 7.2.1511 (Core)
   ```

3. yum安装gcc相关（需要确保 虚拟机可以上外网 ）

   ```
   yum -y install gcc
   yum -y install gcc-c++
   ```

4. 卸载旧版本

   ```
   yum -y remove docker docker-common docker-selinux docker-engine
   # 官网版本
   yum remove docker \
             docker-client \
             docker-client-latest \
             docker-common \
             docker-latest \
             docker-latest-logrotate \
             docker-logrotate \
             docker-engine
   ```

5. 安装需要的软件包

   ```
   yum install -y yum-utils device-mapper-persistent-data lvm2
   ```

6. 设置stable镜像仓库

   ```
   # 错误
   yum-config-manager --add-repo https://download.docker.com/linux/centos/docker-ce.repo
   ## 报错
   [Errno 14] curl#35 - TCP connection reset by peer
   [Errno 12] curl#35 - Timeout
   
   # 正确推荐使用国内的
   yum-config-manager --add-repo http://mirrors.aliyun.com/docker-ce/linux/centos/docker-ce.repo
   ```

7. 更新yum软件包索引

   ```
   yum makecache fast
   ```

8. 安装Docker CE

   ```
   yum -y install docker-ce docker-ce-cli containerd.io
   ```

9. 启动docker

   ```
   systemctl start docker
   ```

10. 测试

    ```
    docker version
    
    docker run hello-world
    
    docker images
    ```

## 宝塔面板安装

## 解压缩安装

## 安装jdk

解压缩并移置相应目录**

　　1.在 /usr/local/目录下通过mkdir 创建install目录，该目录用于存储将来安装的程序，操作如图：

![img](https://img2018.cnblogs.com/i-beta/1931176/202002/1931176-20200201223543559-466496345.png)

 

　　2.找到压缩包目录 通过解压缩命令（tar -zxvf jdk-11_linux-x64_bin.tar.gz ）进行解压，如图：

![img](https://img2018.cnblogs.com/i-beta/1931176/202002/1931176-20200201222918915-1178116612.png)

 

　　3.解压完毕后通过mv命令将解压后的目录剪切到/usr/local/install目录下，操作如图（蓝色字体文件为解压缩的JDK）：

 ![img](https://img2018.cnblogs.com/i-beta/1931176/202002/1931176-20200201223727708-1176668407.png)

**三、配置环境变量**

　　1.通过vim /etc/profile命令，打开etc目录下的环境变量配置文件profile，按“i”键在最底端输入如下代码：

```
export JAVA_HOME=/usr/local/install/jdk-11
export PATH=$JAVA_HOME/bin:$PATH
```

　　2.输入完毕后，按“esc”键退出编辑，输入“：wq”保存并退出profile配置文件操作如图：

![img](https://img2018.cnblogs.com/i-beta/1931176/202002/1931176-20200201224649023-1575394472.png)

 

 　3.通过source /etc/profile命令更新配置文件，操作如图：

![img](https://img2018.cnblogs.com/i-beta/1931176/202002/1931176-20200201225014570-1910925673.png)

 

 **四、验证结果**

　　通过输入Java -version查看是否安装成功，操作如图：

![img](https://img2018.cnblogs.com/i-beta/1931176/202002/1931176-20200201225207000-665962082.png)

## 防火墙端口

firewall-cmd --list-ports（查看端口信息）

firewall-cmd --zone=public --add-port=8081/tcp --permanent（开启端口）

systemctl restart firewalld.service（重启防火墙）







