# 简单win系统fnm 安装工具

## 简介

安装fnm软件到系统目录,配置对应环境变量,以及自动node版本检查

## 使用

双击instll.exe
显示"安装成功!"
即可

## 程序主要步骤

* 复制fnm.exe和fnm_init.cmd 文件到C:\\Program Files (x86)\\fnm
* 将此目录添加到环境变量中
* 在注册表SOFTWARE\\Microsoft\\Command Processor内添加AutoRun项值为"C:\\Program Files (x86)\\fnm\\fnm_init.cmd(目的是进入某目录时自动检查node版本)

> 附件fnm为上传时使用版本,最新版请到此处自行下载 https://github.com/Schniz/fnm/releases
