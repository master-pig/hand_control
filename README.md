
#### 1 rtc_brownout_isr_handler
理解错误信息：0x40375613: rtc_brownout_isr_handler
错误信息含义
这个错误信息通常出现在ESP32开发过程中，它指示程序在运行时遇到了一个异常，并定位到了发生异常的具体位置。

0x40375613: 这个十六进制数字是程序出错时指令指针（Program Counter，PC）所指向的内存地址。它可以帮助开发者定位到代码中的哪一行。
rtc_brownout_isr_handler: 这是一个函数名，表示在ESP32的实时时钟控制器（RTC）检测到棕色故障（Brownout）时，会调用这个函数来处理中断。
D:/Espressif/v5.3.1/esp-idf/components/esp_system/port/brownout.c:61: 这个路径和行号指出了发生异常的代码所在的文件和具体行数。
总结来说，这个错误信息表示程序在执行RTC棕色故障中断服务程序时发生了异常，导致程序崩溃。

棕色故障（Brownout）是什么？
棕色故障是指电源电压低于一个临界值，但又没有完全断电的一种状态。当ESP32检测到棕色故障时，为了保护系统，会触发中断，执行相应的处理函数。

#### 2 为什么group 0 不能用三个但是group 1 可以？
