/*
 * SPDX-FileCopyrightText: 2022-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/mcpwm_prelude.h"
#include "servo_move.h"

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

mcpwm_instance_t servo1;
mcpwm_instance_t servo2;
mcpwm_instance_t servo3;
mcpwm_instance_t servo4;
mcpwm_instance_t servo5;

int servo_enable1 = 0;
int servo_enable2 = 0;
int servo_enable3 = 0;
int servo_enable4 = 0;
int servo_enable5 = 0;


// 角度转成脉冲输出时间长度
static inline uint32_t example_angle_to_compare(int angle)
{
    return (angle - SERVO_MIN_DEGREE) * (SERVO_MAX_PULSEWIDTH_US - SERVO_MIN_PULSEWIDTH_US) / (SERVO_MAX_DEGREE - SERVO_MIN_DEGREE) + SERVO_MIN_PULSEWIDTH_US;
}

void servo_spawn(mcpwm_instance_t *instance, int gpio, int group){
    ESP_LOGI(TAG, "Create timer and operator");//创建定时器和执行器
    mcpwm_timer_config_t timer_config = {
        .group_id = group,
        .clk_src = MCPWM_TIMER_CLK_SRC_DEFAULT,
        .resolution_hz = SERVO_TIMEBASE_RESOLUTION_HZ,
        .period_ticks = SERVO_TIMEBASE_PERIOD,
        .count_mode = MCPWM_TIMER_COUNT_MODE_UP,
    };
    ESP_ERROR_CHECK(mcpwm_new_timer(&timer_config, &(instance->timer)));

    mcpwm_operator_config_t operator_config = {
        .group_id = group, // operator must be in the same group to the timer  执行器必须和定时器同一个组
    };
    ESP_ERROR_CHECK(mcpwm_new_operator(&operator_config, &(instance->oper)));

    ESP_LOGI(TAG, "Connect timer and operator");//连接定时器和执行器
    ESP_ERROR_CHECK(mcpwm_operator_connect_timer(instance->oper, instance->timer));

    ESP_LOGI(TAG, "Create comparator and generator from the operator");//从执行器创建比较器和生成器
    mcpwm_comparator_config_t comparator_config = {
        .flags.update_cmp_on_tez = true,
    };
    ESP_ERROR_CHECK(mcpwm_new_comparator(instance->oper, &comparator_config, &(instance->comparator)));

    mcpwm_generator_config_t generator_config = {
        .gen_gpio_num = gpio,
    };
    ESP_ERROR_CHECK(mcpwm_new_generator(instance->oper, &generator_config, &(instance->generator)));

    // set the initial compare value, so that the servo will spin to the center position
    //设置初始比较值，使伺服系统旋转到中心位置
    ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(instance->comparator, example_angle_to_compare(0)));

    ESP_LOGI(TAG, "Set generator action on timer and compare event");//在计时器上设置生成器动作并比较事件
    // go high on counter empty
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_timer_event(instance->generator,
                                                              MCPWM_GEN_TIMER_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, MCPWM_TIMER_EVENT_EMPTY, MCPWM_GEN_ACTION_HIGH)));
    // go low on compare threshold
    ESP_ERROR_CHECK(mcpwm_generator_set_action_on_compare_event(instance->generator,
                                                                MCPWM_GEN_COMPARE_EVENT_ACTION(MCPWM_TIMER_DIRECTION_UP, instance->comparator, MCPWM_GEN_ACTION_LOW)));

    ESP_LOGI(TAG, "Enable and start timer");//使能并开始定时器
    ESP_ERROR_CHECK(mcpwm_timer_enable(instance->timer));
    ESP_ERROR_CHECK(mcpwm_timer_start_stop(instance->timer, MCPWM_TIMER_START_NO_STOP));

}
void servo_move(mcpwm_instance_t *instance, int enable){
    if (enable==1){
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(instance->comparator, example_angle_to_compare(180)));
        printf("up success");
    }
    else if (enable==-1){
        ESP_ERROR_CHECK(mcpwm_comparator_set_compare_value(instance->comparator, example_angle_to_compare(0)));
        printf("down success");
    }
    // else if (enable==0){
    //     print("enable=0");
    // }

}


void Task1(void* param) //传入空指针方便后期传入参数:
{
    while(1)
    {
        ESP_LOGI(TAG,"执行任务1");
        servo_move(&servo1, servo_enable1);
        //printf("Hello Task!\n");//打印Hello Task!
        vTaskDelay(100/portTICK_PERIOD_MS);//延时1000ms=1s,使系统执行其他任务
    }

}

void Task2(void* param) //传入空指针方便后期传入参数:
{
    while(1)
    {
        ESP_LOGI(TAG,"执行任务2");
        servo_move(&servo2, servo_enable2);
        //printf("Hello Task!\n");//打印Hello Task!
        vTaskDelay(100/portTICK_PERIOD_MS);//延时1000ms=1s,使系统执行其他任务
    }

}

void Task3(void* param) //传入空指针方便后期传入参数:
{
    while(1)
    {
        ESP_LOGI(TAG,"执行任务3");
        servo_move(&servo3, servo_enable3);
        //printf("Hello Task!\n");//打印Hello Task!
        vTaskDelay(100/portTICK_PERIOD_MS);//延时1000ms=1s,使系统执行其他任务
    }

}
void Task4(void* param) //传入空指针方便后期传入参数:
{
    while(1)
    {
        ESP_LOGI(TAG,"执行任务4");
        servo_move(&servo4, servo_enable4);
        //printf("Hello Task!\n");//打印Hello Task!
        vTaskDelay(100/portTICK_PERIOD_MS);//延时1000ms=1s,使系统执行其他任务
    }

}
void Task5(void* param) //传入空指针方便后期传入参数:
{
    while(1)
    {
        ESP_LOGI(TAG,"执行任务5");
        servo_move(&servo5, servo_enable5);
        //printf("Hello Task!\n");//打印Hello Task!
        vTaskDelay(100/portTICK_PERIOD_MS);//延时1000ms=1s,使系统执行其他任务
    }

}


// void app_main(void){
    

//     servo_spawn(&servo1, 36, 0);
//     servo_spawn(&servo2, 37, 0);
//     servo_spawn(&servo3, 38, 1);
//     servo_spawn(&servo4, 39, 1);
//     servo_spawn(&servo5, 40, 1);

//     ESP_LOGI(TAG,"FREERTOS 已启动！");
//     xTaskCreate(Task1,"Task1",2048,NULL,1,NULL);//创建任务1
//     vTaskDelay(200/portTICK_PERIOD_MS);
//     xTaskCreate(Task2,"Task2",2048,NULL,1,NULL);//创建任务2
//     vTaskDelay(200/portTICK_PERIOD_MS);
//     xTaskCreate(Task3,"Task3",2048,NULL,1,NULL);//创建任务3
//     vTaskDelay(200/portTICK_PERIOD_MS);
//     xTaskCreate(Task4,"Task4",2048,NULL,1,NULL);//创建任务4
//     vTaskDelay(200/portTICK_PERIOD_MS);
//     xTaskCreate(Task5,"Task5",2048,NULL,1,NULL);//创建任务5
    
// }
