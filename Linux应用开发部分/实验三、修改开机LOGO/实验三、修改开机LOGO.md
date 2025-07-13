## 1.内核配置选项
在终端中进入Linux源码目录下，在终端中输入
``` shell
cd kernel
make manuconfig
```
即可进入内核配置界面，配置LCD设备
Device Drivers ——>
       　　Graphics support ——>
       　　       　　Nexell Graphics ——>
       　　       　　       　　[\*]LVDS
       　　       　　       　　lvds lcd select ——>
       　　       　　       　　       　　(X)LVDS LCS 1280\*800 10.1 inch
![[Pasted image 20250713182902.png]]


## 2.准备一张照片
可以在网上寻找一个自己喜欢的图片，最好不要太大。如果不是bmp格式可以在网上寻找"xxx转bmp"，例如[png转bmp](https://cdkm.com/cn/png-to-bmp)。

把转换好的图片放入到Linux源码目录下的boot目录，并命名为logo.bmp


## 3.编译镜像
在终端中进入Linux源码根目录，重新编译内核
``` shell
sudo ./build/build_kernel.sh
```


## 4.烧录镜像
进入开发板的uboot命令行，输入
``` shell
fastboot
```

使用一个数据线将开发板的OTG接口与电脑相连，在Ubuntu终端中输入
``` shell
sudo fastboot flash boot ./result/boot.img
```
等待烧录完成后，重启开发板，即可看到我们替换好的logo
![[c4a0ca75e147b21935d715dd1430eca6.jpeg]]