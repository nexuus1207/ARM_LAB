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
&emsp;&emsp;在eMMC为空的时候，是不能从eMMC启动的，这时就需要通过其他方法，把bootloader (u-boot)烧写到eMMC。烧录完bootloader之后，系统才能从eMMC启动，继而烧录其他镜像。烧录系统用的是fastboot命令，关于fastboot，主要用到下面几个功能。
* fastboot devices：查看设备信息。
* fastboot reboot：重启系统。
* fastboot flash：把PC机里的某个镜像文件烧录到指定分区。
* fastboot erase：擦除某个指定分区。
* fastboot update：烧录升级包。
## 2.制作可启动的SD卡
&emsp;&emsp;制作SD卡是在Ubuntu系统下进行的，我们需要首先检测插入SD卡时能否识别到SD卡。
当SD卡连接到电脑时，会在系统的`/dev`目录下建立对应的设备节点。插入SD卡，进入目录检测设备节点是否生成。

插入SD卡之前
![[Pasted image 20250708230055.png]]
插入SD卡之后
![[Pasted image 20250708230220.png]]
可以确定SD卡为sdb设备。
进入Linux源码目录，编辑`./u-boot/tf_tools/sd_fusing.sh`文件
![[Pasted image 20250708230924.png]]
`reader_type1`改为我们的SD卡设备
由于我的是新卡，所以将`disk_tf_enable`设置为1，如果是旧卡就设置为0
设置好后，进入`./u-boot/tf_tools/`文件夹执行烧录脚本，输入命令
```shell
sudo ./sd_fusing.sh /dev/sdb
```
![[Pasted image 20250708232147.png]]
这样就成功完成了SD卡的制作。

## 3.从SD卡启动
将SW1[1]拨至OFF即可将开发板启动方式设置为“SD卡启动模式”
![[d4bb76958b7f684c24fa973cf3865e41.jpeg]]
