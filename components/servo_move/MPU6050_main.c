#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// 日志头文件
#include "esp_log.h"
// 测试头文件
#include "unity.h"


#include "driver/i2c.h"
#include "mpu6050.h"
#include "esp_system.h"
#include "MPU6050_main.h"


#define I2C_MASTER_SCL_IO 19      /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 20      /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0  /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 100000 /*!< I2C master clock frequency */

#define MPU6050_ADDR1 0x68
#define MPU6050_ADDR2 0x69


static const char *TAG = "HK";
static mpu6050_handle_t mpu6050 = NULL;

/**
 * @brief i2c master initialization
 */
static void i2c_bus_init(void)
{
    i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    esp_err_t ret = i2c_param_config(I2C_MASTER_NUM, &conf);
    TEST_ASSERT_EQUAL_MESSAGE(ESP_OK, ret, "I2C config returned error");

    ret = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    TEST_ASSERT_EQUAL_MESSAGE(ESP_OK, ret, "I2C install returned error");
}

/**
 * @brief i2c master initialization
 */
static void i2c_sensor_mpu6050_init(void)
{
    esp_err_t ret;

    i2c_bus_init();
    mpu6050 = mpu6050_create(I2C_MASTER_NUM, MPU6050_I2C_ADDRESS);
    TEST_ASSERT_NOT_NULL_MESSAGE(mpu6050, "MPU6050 create returned NULL");

    ret = mpu6050_config(mpu6050, ACCE_FS_4G, GYRO_FS_500DPS);
    TEST_ASSERT_EQUAL(ESP_OK, ret);

    ret = mpu6050_wake_up(mpu6050);
    TEST_ASSERT_EQUAL(ESP_OK, ret);
}

void set_servo_enable(){
    servo_enable1 = 1;
    servo_enable2 = 1;
    servo_enable3 = 1;
    servo_enable4 = 1;
    servo_enable5 = 1;
}

void set_servo_disable(){
    servo_enable1 = -1;
    servo_enable2 = -1;
    servo_enable3 = -1;
    servo_enable4 = -1;
    servo_enable5 = -1;
}

void Task_monitor(void* param)
{
    esp_err_t ret;
    uint8_t mpu6050_deviceid;
    mpu6050_acce_value_t acce;
    mpu6050_gyro_value_t gyro;
    complimentary_angle_t angle;

    i2c_sensor_mpu6050_init();

    ret = mpu6050_get_deviceid(mpu6050, &mpu6050_deviceid);
    TEST_ASSERT_EQUAL(ESP_OK, ret);

    ret = mpu6050_get_acce(mpu6050, &acce);
    TEST_ASSERT_EQUAL(ESP_OK, ret);

    ret = mpu6050_get_gyro(mpu6050, &gyro);
    TEST_ASSERT_EQUAL(ESP_OK, ret);

    float now_pitch = 0;
    float before_pitch = 0;

    while (1)
    {
        ret = mpu6050_complimentory_filter(mpu6050, &acce,&gyro,&angle);
        TEST_ASSERT_EQUAL(ESP_OK, ret);
        ESP_LOGI(TAG, "pitch:%.2f roll:%.2f \n", angle.pitch,angle.roll);
        vTaskDelay(1000/portTICK_PERIOD_MS);//延时200ms=0.2s,使系统执行其他任务

        before_pitch = now_pitch;
        now_pitch = angle.pitch;
        ESP_LOGI(TAG, "true_pitch:%.2f", now_pitch-before_pitch);

        if ((now_pitch-before_pitch>=50) && (before_pitch != 0)){
            set_servo_enable();
            }
        if ((now_pitch-before_pitch<=-50) && (before_pitch != 0)){
            set_servo_disable();
        }
    }
    
}

