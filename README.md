# AVRUSBMorse

基于 AVR8 USB 芯片的 Morse Code 练习器


# 硬件配置

+ ATmega16u2 带有 USB 接口的主控芯片
+ 耳机接口，可以接入半自动键
+ 三个可编程 LED 灯
+ 通过 NMOS 管驱动蜂鸣器 (可变电阻调压)
+ 带有用户可编程的按键一个
+ 可变电阻接入 ADC 引脚，支持简易配置  (设计错误，芯片不支持 ADC)
+ 带有芯片 RST 按键 (注意：对应电容需要移除)
+ 预留 UART 和 CLKO 引脚。
+ 带有 AVR-ISP 芯片编程接口(该芯片支持 USB DFU 更方便一些)

# 现有功能

+ 内置按键触发 200ms 蜂鸣器工作
+ 通过耳机口链接自动键，触发蜂鸣器发出长短音。
+ 通过 PWM 驱动蜂鸣器 默认 600Hz 50% (效果不佳)
+ 实现 USE HID 版本协议。推送按键状态 (使用 LUFA usb stack, cmake version.)
+ 使用 CW 自解码，实现通过USB HID发送解码内容到电脑终端

# TODO

+ 编写 HID 版本的电脑端桌面应用
+ 暂无


# 原理图
[AVRUSBMorse.pdf](/schematic/AVRUSBMorse.pdf)

# 成品照片

![PCB](/PCB.jpg)
