#pragma once

#define NB_TIM_PWM              6

#define I2C_DRIVER              (&I2CD1)
#define I2C_100K                0x00303D5D
#define I2C_400K                0x0010071B
#define I2C_1M                  0x208

#define NB_EXBUS_UART           2
#define EXBUS_UARTS             {&SD4, &SD5}

#define GPS_UART_DRIVER         (&SD1)

#define USB_PORT                (&PORTAB_SDU1)

#define SERVO_LUT               {4, 10, 9, 8, 5, 12, 14, 13, 1, 2, 3, 15, 6, 0, 11, 7}
