/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"



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
