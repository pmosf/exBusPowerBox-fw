# exBusPowerBox

SERVO1: PA0, T2_1, AF1
SERVO2: PA1, T2_2, AF1
SERVO3: PA2, T9_1, AF3
SERVO4: PA3, T9_2, AF3
SERVO5: PB10, T2_3, AF1
SERVO6: PB11, T2_4, AF1
SERVO7: PE9, T1_1, AF1
SERVO8: PE11, T1_2, AF1
SERVO9: PE13, T1_3, AF1
SERVO10: PE14, T1_4, AF1
SERVO11: PA5, T8_1N, AF3
SERVO12: PA6, T3_1, AF2
SERVO13: PA7, T3_2, AF2
SERVO14: PB0, T3_3, AF2
SERVO15: PB1, T3_4, AF2
SERVO16: PB14, T12_1, AF9

timer clock : 24Mhz
T1: 6 ch
T2: 4 ch
T3: 4 ch
T8: 6 ch, ch2->6 disabled
T9: 2ch
T12: 2 ch, ch2 disabled

PCB front view, usb at top
| left | right
|------|-------
|S1    |  S8
|S14 | S9
|S13 | S10
|S12 | S16
|S2 | S5
|S11 | S7
|S4 | S15
|S3 | S6

GPS: USART1
EXBUS1: PC10, AF8, UART4
EXBUS2: PC12, AF8, UART5
SPARE_UART: PC6, AF8, USART6

* low-speed sensor acquisition thread
0.5Hz
temperatures
battery capacity

* high-speed sensor acquisition thread
2Hz
battery voltage
battery current
gps

* I2C addresses
MAX6639: 0x58
LTC2943: 0x64
PCA9543: 0x70

* I2C timing register
100kHz: 0x00303D5D
400kHz: 0x0010071B
1MHz:  0x208