# <center>**ReadMe**<center>

# 0 项目概要

## 0.1 概述

## 0.2 开发环境

> - 基于Cortex-A9 Android&Linux（s4418）
> - X4418实验平台
> - `Qt5.0`以上版本
> - `sqlite`
> - 内核（实验室提供）
> - 摄像头

## 0.3 使用

 - 在实验平台运行`/可执行文件/Monitor_Server`<br>
 - 在客户端（与服务器端连通）运行`/可执行文件/Monitor_Client`<br>
 
*Tips: 详细操作可见`/文档/项目开发报告.pdf`*

# 1 项目人员

> 王永杰 16340227<br>
> 韩俊柠 
> 
 
# 2 项目目录

```
│  ReadMe.md
│  
├─内核&操作系统
│  ├─ubootpak.bin
│  │      
│  └─rootfs.img
|
├─可执行文件
│      Monitor_Client
│      Monitor_Server
│      
├─文档
│  ├─设计报告.pdf
│  │      
│  ├─项目开发报告.pdf
|  |
│  ├─经验报告.pdf
│  │      
│  └─内核&文件系统定制实验报告.pdf
│      
├─源代码
│  ├─Monitor_Client
│  │      main.cpp
│  │      monitorclient.cpp
│  │      monitorclient.h
│  │      monitorclient.ui
│  │      Monitor_Client.pro
│  │      
│  └─Monitor_Server
│          main.cpp
│          monitorserver.cpp
│          monitorserver.h
│          monitorserver.ui
│          Monitor_Server.pro
│          
└─演示视频&PPT
│  ├─演示视频.mp4
│  │      
│  └─展示.pptx

```
