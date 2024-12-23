idf_component_register(
    SRCS "servo_move.c" "MPU6050_main.c"
    INCLUDE_DIRS "."
    REQUIRES "driver" "freertos" "unity" "mpu6050"
)
