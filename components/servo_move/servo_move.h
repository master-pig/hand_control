/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
// #ifndef SERVO_DEMO_H
// #define SERVO_DEMO_H

#include "driver/mcpwm_prelude.h"

// 在使用extren的时候还是要有定义先的
typedef struct {
    mcpwm_timer_handle_t timer;
    mcpwm_oper_handle_t oper;
    mcpwm_cmpr_handle_t comparator;
    mcpwm_gen_handle_t generator;
    } mcpwm_instance_t;

extern mcpwm_instance_t servo1;
extern mcpwm_instance_t servo2;
extern mcpwm_instance_t servo3;
extern mcpwm_instance_t servo4;
extern mcpwm_instance_t servo5;


extern int servo_enable1;
extern int servo_enable2;
extern int servo_enable3;
extern int servo_enable4;
extern int servo_enable5;

void servo_spawn(mcpwm_instance_t *instance, int gpio, int group);
void Task1(void *param); // 传入空指针方便后期传入参数:
void Task2(void *param); // 传入空指针方便后期传入参数:
void Task3(void *param); // 传入空指针方便后期传入参数:
void Task4(void *param); // 传入空指针方便后期传入参数:
void Task5(void *param); // 传入空指针方便后期传入参数:

// #endif
