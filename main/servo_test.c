#include "servo.h"

void app_main(void){
    

    servo_spawn(&servo1, 36, 0);
    servo_spawn(&servo2, 37, 0);
    servo_spawn(&servo3, 38, 1);
    servo_spawn(&servo4, 39, 1);
    servo_spawn(&servo5, 40, 1);

    ESP_LOGI(TAG,"FREERTOS 已启动！");
    xTaskCreate(Task1,"Task1",2048,NULL,1,NULL);//创建任务1
    vTaskDelay(200/portTICK_PERIOD_MS);
    xTaskCreate(Task2,"Task2",2048,NULL,1,NULL);//创建任务2
    vTaskDelay(200/portTICK_PERIOD_MS);
    xTaskCreate(Task3,"Task3",2048,NULL,1,NULL);//创建任务3
    vTaskDelay(200/portTICK_PERIOD_MS);
    xTaskCreate(Task4,"Task4",2048,NULL,1,NULL);//创建任务4
    vTaskDelay(200/portTICK_PERIOD_MS);
    xTaskCreate(Task5,"Task5",2048,NULL,1,NULL);//创建任务5
    
}
