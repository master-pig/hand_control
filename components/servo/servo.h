/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

static const char *TAG = "SERVO";

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


typedef struct {
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
    } mcpwm_instance_t;

mcpwm_instance_t servo1;
mcpwm_instance_t servo2;
mcpwm_instance_t servo3;
mcpwm_instance_t servo4;
mcpwm_instance_t servo5;

void servo_spawn(mcpwm_instance_t *instance, int gpio, int group);
void Task1(void *param); // 传入空指针方便后期传入参数:
void Task2(void *param); // 传入空指针方便后期传入参数:
void Task3(void *param); // 传入空指针方便后期传入参数:
void Task4(void *param); // 传入空指针方便后期传入参数:
void Task5(void *param); // 传入空指针方便后期传入参数:
