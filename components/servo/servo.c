/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"

#include "servo.h"

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


//角度转成脉冲输出时间长度
static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

void servo_spawn(int gpio){
    ESP_LOGI(TAG, "Create timer and operator");//创建定时器和执行器
    mcpwm_timer_handle_t timer = NULL;
    mcpwm_timer_config_t timer_config = {
        .group_id = 0,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &timer));

    mcpwm_oper_handle_t oper = NULL;
    mcpwm_operator_config_t operator_config = {
        .group_id = 0, // operator must be in the same group to the timer  执行器必须和定时器同一个组
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &oper));

    ESP_LOGI(TAG, "Connect timer and operator");//连接定时器和执行器
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(oper, timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");//从执行器创建比较器和生成器
    mcpwm_cmpr_handle_t comparator = NULL;
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(oper, &comparator_config, &comparator));

    mcpwm_gen_handle_t generator = NULL;
    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = gpio,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(oper, &generator_config, &generator));

    // set the initial compare value, so that the servo will spin to the center position
    //设置初始比较值，使伺服系统旋转到中心位置
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");//在计时器上设置生成器动作并比较事件
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");//使能并开始定时器
    ESP_ERROR_CHECK(mcpwm_timer_enable(timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(timer, MCPWM_TIMER_START_NO_STOP));

    int angle = 0;
    int step = 2;


    vTaskDelay(pdMS_TO_TICKS(3000));
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(180)));
    vTaskDelay(pdMS_TO_TICKS(3000));

    /*while (1) {
        ESP_LOGI(TAG, "Angle of rotation: %d", angle);
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(angle)));
        //Add delay, since it takes time for servo to rotate, usually 200ms/60degree rotation under 5V power supply
        //增加延迟，因为伺服旋转需要时间，通常在5V电源下旋转200ms/60度
        vTaskDelay(pdMS_TO_TICKS(100));
        if ((angle + step) > 170 || (angle + step) < -170) {
            step *= -1;
            vTaskDelay(pdMS_TO_TICKS(3000));
            ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(comparator, example_angle_to_compare(0)));


        }
        angle += step;
    }*/
}

