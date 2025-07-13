---
date: 2025-07-08
aliases:
  - Generate Linux image
---
>[!info]
>本文所有操作的工作空间均为Linux源码文件夹，`./`目录即为Linux源码文件夹

## 1.安装系统工具包

在终端中输入以下命令安装编译所需的工具包
```shell
sudo apt update
sudo apt install bison g++-multilib git gperf libxml2-utils make python-networkx zlib1g-dev:i386 zip u-boot-tools libncurses5-dev vim rar fastboot adb
```

>[! warning]
>可能会出现apt无法定位到python-networkx包位置的情况，可以通过pip进行python包管理，可以使用`sudo pip install networkx`解决这个问题

>[!Tip]
>由于网络环境原因可能执行 `sudo apt update`的速度很慢，可以更换国内的镜像源加快更新软件包信息的过程
>[换源教程](https://mirrors.ustc.edu.cn/help/ubuntu.html)

## 2.编译前的工作

&emsp;&emsp;镜像的编译包括u-boot,kernel,rootfs的编译，最后需要生成的镜像有：u-boot.bin、boot.img、rootfs.ext4。其中2ndboot.bin是烧录系统的过程中合成的，不需要编译。

&emsp;&emsp;将linux系统源码解压，可得到系统源码
![[Pasted image 20250708195848.png]]

>[! note] U-Boot
>u-boot全称 Universal Boot Loader(通用的引导程序），意思是说，在各种地方都可以用，它是一款开源的引导加载程序（Bootloader）。在嵌入式 Linux 生态中，U-Boot是**最流行**的 Bootloader 之一。现代版本的 U-Boot 已经内置了对 Fastboot 协议的支持。开发者在为特定硬件编译 U-Boot 时，只需开启相应的配置选项，就能让这台嵌入式 Linux 设备支持 Fastboot 功能。
>Uboot的最终目的是为了启动内核；Linux 内核在设计的时候，设计为是可以被传参的，也就是说我们可以在 Uboot 中事先给 Linux 内核准备一些启动参数放在内存中特定位置，然后再传给Linux内核，Linux内核启动后就会到这个特定的位置拿 Uboot传给它的参数，然后再 Linux 内核中解析这些参数，这些参数将被用来指导 Linux内核的启动过程。

## 3.编译 U-Boot
进入Linux源码文件夹，执行下列命令编译U-Boot
```shell
./build/build_uboot.sh
```

>[! Failure]
>输入命令后会出现如下错误
>![[Pasted image 20250708201932.png]]

进入构建脚本，脚本进入了`$top/u-boot`文件夹(即Linux源码文件夹中的u-boot文件夹)进行make，找到出现报错的文件，如./u-boot/tools/common/image-fit.c
![[Pasted image 20250708202416.png]]
发现该文件包含的文件使用绝对路径，需要我们逐个进行修改
![[Pasted image 20250708202609.png]]
![[Pasted image 20250708202710.png]]
只需要修改到linux-3.4.39文件夹前的路径即可
>[!Success]
>修改完后在result文件夹成功生成uboot
>![[Pasted image 20250708203942.png]]

## 4.编译Kernel
进入Linux源码根目录，执行下面命令编译Kernel
```shell
./build/build_kernel.sh
```
编译完成后，在result文件夹下生成了新的`u-boot.bin`镜像文件
![[Pasted image 20250708204014.png]]
## 5.编译Rootfs
进入源码根目录，执行下面命令编译Rootfs
```shell
./build/build_rootfs.sh
```
>[!Failure]
>输入命令后会出现错误
>![[Pasted image 20250708205146.png]]

该原因为构建时无法找到`libfakeroot.so`，进入`./buildroot/output/host/usr/bin/`目录下的fakeroot文件
![[Pasted image 20250708221018.png]]
需要对这几个路径进行修改
修改后的内容为
![[Pasted image 20250708221221.png]]
>[!Success]
>再次输入命令，编译成功，生成了新的Rootfs文件
>![[Pasted image 20250708221324.png]]

>[! info] 其他编译错误
>如果碰到其他错误问题，可以参考实验一目录下的`编译Linux时出现的问题及解决方法.txt`