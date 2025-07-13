---
date: 2025-07-08
aliases:
  - program Linux to Flash
---
## 1.实验原理
### (1)启动方式
&emsp;&emsp;嵌入式系统实验箱支持多种启动方式，包括SD、eMMC等，可以通过SW1跳线来切换CPU的启动方式，分别如下：
&emsp;&emsp;&emsp;SD卡启动模式对应设置为：SW[1:2] = OFF X ;
&emsp;&emsp;&emsp;eMMC卡启动模式对应设置为：SW[1:2] = ON X ;
>[!note] eMMC
>eMMC是一种集成控制器和存储芯片的存储设备,采用内置控制器，可以更加方便地实现读写操作和数据管理。所以eMMC可以作为内置存储器使用，并且由于其内置控制器的作用，可以实现快速的读写操作。因此，eMMC适合于移动设备、嵌入式系统和一些消费电子产品等领域的应用。然而，随着用户对性能要求的不断提高，eMMC的半双工工作模式和接口速度逐渐成为瓶颈。在智能手机等平台逐渐被速度更快、支持全双工的***UFS***所取代
### (2)烧录方式
&emsp;&emsp;在eMMC为空的时候，是不能从eMMC启动的，这时就需要通过其他方法，把bootloader (u-boot)烧写到eMMC。烧录完bootloader之后，系统才能从eMMC启动，继而烧录其他镜像。烧录系统需要使用fastboot命令。
>[! note] Fastboot
>Fastboot 是一种存在于设备引导加载程序（Bootloader）中的**通信协议和工具集**。它允许电脑通过 USB 线，在设备的操作系统（Android/Linux）启动**之前**，直接与设备底层闪存进行数据读写。
>&emsp;
>常用fastboot命令：
>* fastboot devices：查看设备信息。
>* fastboot reboot：重启系统。
>* fastboot flash：把PC机里的某个镜像文件烧录到指定分区。
>* fastboot erase：擦除某个指定分区。
>* fastboot update：烧录升级包。

## 2.制作可启动的SD卡
&emsp;&emsp;制作SD卡是在Ubuntu系统下进行的，我们需要首先检测插入SD卡时能否识别到SD卡。
当SD卡连接到电脑时，会在系统的`/dev`目录下建立对应的设备节点。插入SD卡，进入目录检测设备节点是否生成。

插入SD卡之前
![[Pasted image 20250708230055.png]]
插入SD卡之后
![[Pasted image 20250708230220.png]]
可以确认插入的SD卡为`sdb`设备。

进入Linux源码目录，编辑`./u-boot/tf_tools/sd_fusing.sh`文件
![[Pasted image 20250708230924.png]]
`reader_type1`改为我们的SD卡设备
如果使用的是新卡，将`disk_tf_enable`设置为1，如果是旧卡就设置为0

设置好后，进入`./u-boot/tf_tools/`文件夹执行烧录脚本，输入命令
```shell
sudo ./sd_fusing.sh /dev/sdb
```
这样就成功完成了SD卡的制作。
![[Pasted image 20250708232147.png]]
## 3.从SD卡启动
将SW1[1]拨至OFF即可将开发板启动方式设置为“SD卡启动模式”
![[d4bb76958b7f684c24fa973cf3865e41.jpeg]]
使用命令`ls /dev/ttyUSB*`查找我们的调试串口
![[Pasted image 20250712213908.png]]

使用`sudo minicom -s`配置minicom
选择串口设置选项，串口的配置如下
![[Pasted image 20250712214234.png]]
返回到选项页面，选择`保存设置为dfl`，然后退出minicom设置

输入`sudo minicom`，打开开发板电源，在倒计时结束之前按下任意按键即可进入开发板的uboot
![[Pasted image 20250712214511.png]]

## 4.烧写系统

进入uboot命令行后，在uboot终端输入`fastboot`命令后开发板会进入fastboot模式
![[871c76cb29dff6e23568651f58a958d.jpg]]

使用数据线连接电脑和开发板的OTG接口，在Ubuntu终端输入`fastboot devices`命令即可查看连接到电脑的设备信息
![[Pasted image 20250713034916.png]]
>[!info]
>如果系统中没有fastboot命令可以使用`sudo apt install fastboot`安装
>或者使用linux源码文件中自带的fastboot工具`sudo ./device/nrisc/tools/fastboot`


在Linux源码目录下，执行`sudo ./device/nrisc/tools/update.sh -d sd2`即可进行系统烧录
系统烧录完成后会自动重启
![[Pasted image 20250713035424.png]]


烧录完成后关掉电源，将SW1[1]拨至`ON`，打开开发板电源，LCD显示屏会显示一个图片
![[微信图片_20250713035851.jpg]]

但是开发板没有进入Linux系统，在串口中可以看到错误
![[Pasted image 20250713040242.png]]

这是因为emmc中保存着安卓系统的引导信息。设置当前Linux引导信息，需要重新进入uboot，输入以下命令：
``` bash
$ env default -a
$ saveenv
$ reset
```
输入完命令后开发板会重启，成功进入Linux系统。默认只有一个root用户，没有密码
![[Pasted image 20250713041210.png]]

>[! note] env命令
>env命令是environment的缩写，它可以用来显示或设置当前shell的**环境变量**。环境变量是一些存储在系统中的**键值对**，可以影响程序的运行。env命令可以用来查看所有的环境变量，也可以用来**临时**改变环境变量，或者在一个空白的环境中运行一个程序。
>&emsp;
>env常用命令：
>``` shell
>env        查看系统中所有
>env PATH=/home/nexuus        设置环境变量`PATH`为`/home/nexuus`
>env -u PATH        删除环境变量`PATH`
>```



## 5.单独烧录
在开发中，单独烧录某个镜像可以节省我们的时间。以下的操作空间均在Linux源码根目录下

**烧录 2ndboot.bin 镜像**
``` shell
sudo fastboot flash 2ndboot ./result/2ndboot.bin
```


**烧录U-Boot.bin镜像**
``` shell
sudo fastboot flash bootloader ./result/u-boot.bin
```


**烧录boot.img镜像**
``` shell
sudo fastboot flash boot ./result/boot.img
```


**烧录rootfs.ext4镜像**
``` shell
sudo fastboot flash system ./result/rootfs.ext4
```
