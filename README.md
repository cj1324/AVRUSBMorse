# AVRUSBMorse

基于 AVR8 USB 芯片的 Morse Code 练习器


# 硬件配置

+ ATmega16u2 带有 USB 接口的主控芯片
+ 耳机接口，可以接入半自动键
+ 三个可编程 LED 灯
+ 通过 NMOS 管驱动蜂鸣器 (可变电阻调压)
+ 带有用户可编程的按键一个
+ 可变电阻接入 ADC 引脚，支持简易配置
+ 带有芯片 RST 按键 (注意：对应电容需要移除)
+ 预留 UART 和 CLKO 引脚。
+ 带有 AVR-ISP 芯片编程接口(该芯片支持 USB DFU 更方便一些)

# 现有功能

+ 内置按键触发 200ms 蜂鸣器工作
+ 通过耳机口链接自动键，触发蜂鸣器发出长短音。

# TODO

+ 通过 PWM 驱动蜂鸣器 可配置工作在 300 - 800 Hz 默认 600Hz
+ 使用 USB 口 简单的解码，实现通过USB串口发送解码内容到电脑终端
+ 实现 HID 版本协议。控制 WPM 速度和声音频率
+ 编写 HID 版本的电脑端桌面应用 (可 YY)

# 原理图
[AVRUSBMorse.pdf](/schematic/AVRUSBMorse.pdf)
