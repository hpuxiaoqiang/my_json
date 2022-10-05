# 启程

## 项目架构

本次学习的`json`一共包含三个文件

1. myjson.h: myjson的头文件，含有对外的类型和API函数声明
2. myjson.cpp：leptjson 的实现文件(implementation file),含有内部的类型声明和函数实现。此文件会编译成库
3. test.cpp：我们使用测试驱动开发(test driven development, TDD)。此文件包含测试程序，需要链接 leptjson 库
