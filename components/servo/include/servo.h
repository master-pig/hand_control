#ifndef SERVO_H
#define SERVO_H


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

// Please consult the datasheet of your servo before changing the following parameters
//在更改以下参数之前，请查阅您的伺服系统数据手册
#define SERVO_MIN_PULSEWIDTH_US 500  // Minimum pulse width in microsecond 最小脉冲宽度（微秒）
#define SERVO_MAX_PULSEWIDTH_US 2500  // Maximum pulse width in microsecond最大脉冲宽度（微秒）
#define SERVO_MIN_DEGREE        0   // Minimum angle  最小角度
#define SERVO_MAX_DEGREE        300    // Maximum angle  最大角度

#define SERVO_PULSE_GPIO_1             39        // GPIO connects to the PWM signal line  连接到伺服电机信号的管脚
#define SERVO_PULSE_GPIO_2             40        // GPIO connects to the PWM signal line  连接到伺服电机信号的管脚
#define SERVO_TIMEBASE_RESOLUTION_HZ 1000000  // 1MHz, 1us per tick  时钟频率
#define SERVO_TIMEBASE_PERIOD        20000    // 20000 ticks, 20ms   基准信号周期

void servo_spawn(int gpio);

#endif