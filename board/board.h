/*
    ChibiOS - Copyright (C) 2006..2017 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/*
 * This file has been automatically generated using ChibiStudio board
 * generator plugin. Do not edit manually.
 */

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Setup for exBusPowerBox board.
 */

/*
 * Board identifier.
 */
#define BOARD_exBusPowerBox
#define BOARD_NAME                  "exBusPowerBox"

/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                32768U
#endif

#define STM32_LSEDRV                (3U << 3U)

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                16000000U
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330U

/*
 * MCU type as defined in the ST header.
 */
#define STM32F745xx

/*
 * IO pins assignments.
 */
#define GPIOA_SERVO1                0U
#define GPIOA_SERVO2                1U
#define GPIOA_SERVO3                2U
#define GPIOA_SERVO4                3U
#define GPIOA_PIN4                  4U
#define GPIOA_SERVO11               5U
#define GPIOA_SERVO12               6U
#define GPIOA_SERVO13               7U
#define GPIOA_RPM1                  8U
#define GPIOA_RPM2                  9U
#define GPIOA_OTG_FS_ID             10U
#define GPIOA_OTG_FS_DM             11U
#define GPIOA_OTG_FS_DP             12U
#define GPIOA_JTMS                  13U
#define GPIOA_JTCK                  14U
#define GPIOA_JTDI                  15U

#define GPIOB_SERVO14               0U
#define GPIOB_SERVO15               1U
#define GPIOB_PIN2                  2U
#define GPIOB_JTDO                  3U
#define GPIOB_JNRST                 4U
#define GPIOB_PIN5                  5U
#define GPIOB_GPS_TX                6U
#define GPIOB_GPS_RX                7U
#define GPIOB_I2C_SCL               8U
#define GPIOB_I2C_SDA               9U
#define GPIOB_PIN10                 10U
#define GPIOB_SERVO6                11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_SERVO16               14U
#define GPIOB_PIN15                 15U

#define GPIOC_PIN0                  0U
#define GPIOC_PIN1                  1U
#define GPIOC_PIN2                  2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_EXBUS3                6U
#define GPIOC_JETI3                 7U
#define GPIOC_PIN8                  8U
#define GPIOC_PIN9                  9U
#define GPIOC_EXBUS1                10U
#define GPIOC_PIN11                 11U
#define GPIOC_EXBUS2                12U
#define GPIOC_PIN13                 13U
#define GPIOC_PIN14                 14U
#define GPIOC_PIN15                 15U

#define GPIOD_PIN0                  0U
#define GPIOD_PIN1                  1U
#define GPIOD_PIN2                  2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_PIN8                  8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_SERVO7                9U
#define GPIOE_PIN10                 10U
#define GPIOE_SERVO8                11U
#define GPIOE_PIN12                 12U
#define GPIOE_SERVO9                13U
#define GPIOE_SERVO10               14U
#define GPIOE_PIN15                 15U

#define GPIOF_PIN0                  0U
#define GPIOF_PIN1                  1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_PIN6                  6U
#define GPIOF_PIN7                  7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_PIN10                 10U
#define GPIOF_PIN11                 11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_PIN10                 10U
#define GPIOG_PIN11                 11U
#define GPIOG_PIN12                 12U
#define GPIOG_PIN13                 13U
#define GPIOG_PIN14                 14U
#define GPIOG_PIN15                 15U

#define GPIOH_OSC_IN                0U
#define GPIOH_OSC_OUT               1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOI_PIN0                  0U
#define GPIOI_PIN1                  1U
#define GPIOI_PIN2                  2U
#define GPIOI_PIN3                  3U
#define GPIOI_PIN4                  4U
#define GPIOI_PIN5                  5U
#define GPIOI_PIN6                  6U
#define GPIOI_PIN7                  7U
#define GPIOI_PIN8                  8U
#define GPIOI_PIN9                  9U
#define GPIOI_PIN10                 10U
#define GPIOI_PIN11                 11U
#define GPIOI_PIN12                 12U
#define GPIOI_PIN13                 13U
#define GPIOI_PIN14                 14U
#define GPIOI_PIN15                 15U

#define GPIOJ_LCD_R1                0U
#define GPIOJ_LCD_R2                1U
#define GPIOJ_LCD_R3                2U
#define GPIOJ_LCD_R4                3U
#define GPIOJ_LCD_R5                4U
#define GPIOJ_LCD_R6                5U
#define GPIOJ_LCD_R7                6U
#define GPIOJ_LCD_G0                7U
#define GPIOJ_LCD_G1                8U
#define GPIOJ_LCD_G2                9U
#define GPIOJ_LCD_G3                10U
#define GPIOJ_LCD_G4                11U
#define GPIOJ_OTG_FS_VBUS           12U
#define GPIOJ_LCD_B1                13U
#define GPIOJ_LCD_B2                14U
#define GPIOJ_LCD_B3                15U

#define GPIOK_LCD_G5                0U
#define GPIOK_LCD_G6                1U
#define GPIOK_LCD_G7                2U
#define GPIOK_LCD_BL_CTRL           3U
#define GPIOK_LCD_B5                4U
#define GPIOK_LCD_B6                5U
#define GPIOK_LCD_B7                6U
#define GPIOK_LCD_DE                7U
#define GPIOK_PIN8                  8U
#define GPIOK_PIN9                  9U
#define GPIOK_PIN10                 10U
#define GPIOK_PIN11                 11U
#define GPIOK_PIN12                 12U
#define GPIOK_PIN13                 13U
#define GPIOK_PIN14                 14U
#define GPIOK_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_SERVO1                 PAL_LINE(GPIOA, 0U)
#define LINE_SERVO2                 PAL_LINE(GPIOA, 1U)
#define LINE_SERVO3                 PAL_LINE(GPIOA, 2U)
#define LINE_SERVO4                 PAL_LINE(GPIOA, 3U)
#define LINE_SERVO11                PAL_LINE(GPIOA, 5U)
#define LINE_SERVO12                PAL_LINE(GPIOA, 6U)
#define LINE_SERVO13                PAL_LINE(GPIOA, 7U)
#define LINE_RPM1                   PAL_LINE(GPIOA, 8U)
#define LINE_RPM2                   PAL_LINE(GPIOA, 9U)
#define LINE_OTG_FS_ID              PAL_LINE(GPIOA, 10U)
#define LINE_OTG_FS_DM              PAL_LINE(GPIOA, 11U)
#define LINE_OTG_FS_DP              PAL_LINE(GPIOA, 12U)
#define LINE_JTMS                   PAL_LINE(GPIOA, 13U)
#define LINE_JTCK                   PAL_LINE(GPIOA, 14U)
#define LINE_JTDI                   PAL_LINE(GPIOA, 15U)
#define LINE_SERVO14                PAL_LINE(GPIOB, 0U)
#define LINE_SERVO15                PAL_LINE(GPIOB, 1U)
#define LINE_JTDO                   PAL_LINE(GPIOB, 3U)
#define LINE_JNRST                  PAL_LINE(GPIOB, 4U)
#define LINE_GPS_TX                 PAL_LINE(GPIOB, 6U)
#define LINE_GPS_RX                 PAL_LINE(GPIOB, 7U)
#define LINE_I2C_SCL                PAL_LINE(GPIOB, 8U)
#define LINE_I2C_SDA                PAL_LINE(GPIOB, 9U)
#define LINE_SERVO6                 PAL_LINE(GPIOB, 11U)
#define LINE_SERVO16                PAL_LINE(GPIOB, 14U)
#define LINE_EXBUS3                 PAL_LINE(GPIOC, 6U)
#define LINE_JETI3                  PAL_LINE(GPIOC, 7U)
#define LINE_EXBUS1                 PAL_LINE(GPIOC, 10U)
#define LINE_EXBUS2                 PAL_LINE(GPIOC, 12U)
#define LINE_SERVO7                 PAL_LINE(GPIOE, 9U)
#define LINE_SERVO8                 PAL_LINE(GPIOE, 11U)
#define LINE_SERVO9                 PAL_LINE(GPIOE, 13U)
#define LINE_SERVO10                PAL_LINE(GPIOE, 14U)
#define LINE_OSC_IN                 PAL_LINE(GPIOH, 0U)
#define LINE_OSC_OUT                PAL_LINE(GPIOH, 1U)
#define LINE_LCD_R1                 PAL_LINE(GPIOJ, 0U)
#define LINE_LCD_R2                 PAL_LINE(GPIOJ, 1U)
#define LINE_LCD_R3                 PAL_LINE(GPIOJ, 2U)
#define LINE_LCD_R4                 PAL_LINE(GPIOJ, 3U)
#define LINE_LCD_R5                 PAL_LINE(GPIOJ, 4U)
#define LINE_LCD_R6                 PAL_LINE(GPIOJ, 5U)
#define LINE_LCD_R7                 PAL_LINE(GPIOJ, 6U)
#define LINE_LCD_G0                 PAL_LINE(GPIOJ, 7U)
#define LINE_LCD_G1                 PAL_LINE(GPIOJ, 8U)
#define LINE_LCD_G2                 PAL_LINE(GPIOJ, 9U)
#define LINE_LCD_G3                 PAL_LINE(GPIOJ, 10U)
#define LINE_LCD_G4                 PAL_LINE(GPIOJ, 11U)
#define LINE_OTG_FS_VBUS            PAL_LINE(GPIOJ, 12U)
#define LINE_LCD_B1                 PAL_LINE(GPIOJ, 13U)
#define LINE_LCD_B2                 PAL_LINE(GPIOJ, 14U)
#define LINE_LCD_B3                 PAL_LINE(GPIOJ, 15U)
#define LINE_LCD_G5                 PAL_LINE(GPIOK, 0U)
#define LINE_LCD_G6                 PAL_LINE(GPIOK, 1U)
#define LINE_LCD_G7                 PAL_LINE(GPIOK, 2U)
#define LINE_LCD_BL_CTRL            PAL_LINE(GPIOK, 3U)
#define LINE_LCD_B5                 PAL_LINE(GPIOK, 4U)
#define LINE_LCD_B6                 PAL_LINE(GPIOK, 5U)
#define LINE_LCD_B7                 PAL_LINE(GPIOK, 6U)
#define LINE_LCD_DE                 PAL_LINE(GPIOK, 7U)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_VERYLOW(n)       (0U << ((n) * 2U))
#define PIN_OSPEED_LOW(n)           (1U << ((n) * 2U))
#define PIN_OSPEED_MEDIUM(n)        (2U << ((n) * 2U))
#define PIN_OSPEED_HIGH(n)          (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - SERVO1                    (alternate 1).
 * PA1  - SERVO2                    (alternate 1).
 * PA2  - SERVO3                    (alternate 3).
 * PA3  - SERVO4                    (alternate 3).
 * PA4  - PIN4                      (analog).
 * PA5  - SERVO11                   (alternate 3).
 * PA6  - SERVO12                   (alternate 2).
 * PA7  - SERVO13                   (alternate 2).
 * PA8  - RPM1                      (input pulldown).
 * PA9  - RPM2                      (input pulldown).
 * PA10 - OTG_FS_ID                 (alternate 10).
 * PA11 - OTG_FS_DM                 (alternate 10).
 * PA12 - OTG_FS_DP                 (alternate 10).
 * PA13 - JTMS                      (alternate 0).
 * PA14 - JTCK                      (alternate 0).
 * PA15 - JTDI                      (alternate 0).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ALTERNATE(GPIOA_SERVO1) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO2) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO3) |     \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO4) |     \
                                     PIN_MODE_ANALOG(GPIOA_PIN4) |          \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO11) |    \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO12) |    \
                                     PIN_MODE_ALTERNATE(GPIOA_SERVO13) |    \
                                     PIN_MODE_INPUT(GPIOA_RPM1) |           \
                                     PIN_MODE_INPUT(GPIOA_RPM2) |           \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_ID) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DM) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_OTG_FS_DP) |  \
                                     PIN_MODE_ALTERNATE(GPIOA_JTMS) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_JTCK) |       \
                                     PIN_MODE_ALTERNATE(GPIOA_JTDI))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_SERVO1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO11) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO12) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SERVO13) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RPM1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_RPM2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_ID) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DM) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_OTG_FS_DP) |  \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTMS) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTCK) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_JTDI))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOA_SERVO1) |     \
                                     PIN_OSPEED_VERYLOW(GPIOA_SERVO2) |     \
                                     PIN_OSPEED_VERYLOW(GPIOA_SERVO3) |     \
                                     PIN_OSPEED_VERYLOW(GPIOA_SERVO4) |     \
                                     PIN_OSPEED_HIGH(GPIOA_PIN4) |          \
                                     PIN_OSPEED_VERYLOW(GPIOA_SERVO11) |    \
                                     PIN_OSPEED_VERYLOW(GPIOA_SERVO12) |    \
                                     PIN_OSPEED_VERYLOW(GPIOA_SERVO13) |    \
                                     PIN_OSPEED_HIGH(GPIOA_RPM1) |          \
                                     PIN_OSPEED_HIGH(GPIOA_RPM2) |          \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_ID) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DM) |     \
                                     PIN_OSPEED_HIGH(GPIOA_OTG_FS_DP) |     \
                                     PIN_OSPEED_HIGH(GPIOA_JTMS) |          \
                                     PIN_OSPEED_HIGH(GPIOA_JTCK) |          \
                                     PIN_OSPEED_HIGH(GPIOA_JTDI))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOA_SERVO1) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SERVO2) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SERVO3) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SERVO4) |     \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SERVO11) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SERVO12) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOA_SERVO13) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOA_RPM1) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOA_RPM2) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_ID) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DM) |  \
                                     PIN_PUPDR_FLOATING(GPIOA_OTG_FS_DP) |  \
                                     PIN_PUPDR_PULLUP(GPIOA_JTMS) |         \
                                     PIN_PUPDR_PULLDOWN(GPIOA_JTCK) |       \
                                     PIN_PUPDR_PULLUP(GPIOA_JTDI))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_SERVO1) |           \
                                     PIN_ODR_HIGH(GPIOA_SERVO2) |           \
                                     PIN_ODR_HIGH(GPIOA_SERVO3) |           \
                                     PIN_ODR_HIGH(GPIOA_SERVO4) |           \
                                     PIN_ODR_HIGH(GPIOA_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOA_SERVO11) |          \
                                     PIN_ODR_HIGH(GPIOA_SERVO12) |          \
                                     PIN_ODR_HIGH(GPIOA_SERVO13) |          \
                                     PIN_ODR_HIGH(GPIOA_RPM1) |             \
                                     PIN_ODR_HIGH(GPIOA_RPM2) |             \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_ID) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DM) |        \
                                     PIN_ODR_HIGH(GPIOA_OTG_FS_DP) |        \
                                     PIN_ODR_HIGH(GPIOA_JTMS) |             \
                                     PIN_ODR_HIGH(GPIOA_JTCK) |             \
                                     PIN_ODR_HIGH(GPIOA_JTDI))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_SERVO1, 1U) |        \
                                     PIN_AFIO_AF(GPIOA_SERVO2, 1U) |        \
                                     PIN_AFIO_AF(GPIOA_SERVO3, 3U) |        \
                                     PIN_AFIO_AF(GPIOA_SERVO4, 3U) |        \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_SERVO11, 3U) |       \
                                     PIN_AFIO_AF(GPIOA_SERVO12, 2U) |       \
                                     PIN_AFIO_AF(GPIOA_SERVO13, 2U))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_RPM1, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_RPM2, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_ID, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DM, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_OTG_FS_DP, 10U) |    \
                                     PIN_AFIO_AF(GPIOA_JTMS, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_JTCK, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_JTDI, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - SERVO14                   (alternate 2).
 * PB1  - SERVO15                   (alternate 2).
 * PB2  - PIN2                      (analog).
 * PB3  - JTDO                      (alternate 0).
 * PB4  - JNRST                     (alternate 0).
 * PB5  - PIN5                      (input pullup).
 * PB6  - GPS_TX                    (alternate 7).
 * PB7  - GPS_RX                    (alternate 7).
 * PB8  - I2C_SCL                   (alternate 4).
 * PB9  - I2C_SDA                   (alternate 4).
 * PB10 - PIN10                     (alternate 1).
 * PB11 - SERVO6                    (alternate 1).
 * PB12 - PIN12                     (input pullup).
 * PB13 - PIN13                     (input pullup).
 * PB14 - SERVO16                   (alternate 9).
 * PB15 - PIN15                     (input pullup).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_ALTERNATE(GPIOB_SERVO14) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO15) |    \
                                     PIN_MODE_ANALOG(GPIOB_PIN2) |          \
                                     PIN_MODE_ALTERNATE(GPIOB_JTDO) |       \
                                     PIN_MODE_ALTERNATE(GPIOB_JNRST) |      \
                                     PIN_MODE_INPUT(GPIOB_PIN5) |           \
                                     PIN_MODE_ALTERNATE(GPIOB_GPS_TX) |     \
                                     PIN_MODE_ALTERNATE(GPIOB_GPS_RX) |     \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C_SCL) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_I2C_SDA) |    \
                                     PIN_MODE_ALTERNATE(GPIOB_PIN10) |      \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO6) |     \
                                     PIN_MODE_INPUT(GPIOB_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN13) |          \
                                     PIN_MODE_ALTERNATE(GPIOB_SERVO16) |    \
                                     PIN_MODE_INPUT(GPIOB_PIN15))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_SERVO14) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO15) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JTDO) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_JNRST) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_GPS_TX) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_GPS_RX) |     \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C_SCL) |   \
                                     PIN_OTYPE_OPENDRAIN(GPIOB_I2C_SDA) |   \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO6) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_SERVO16) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOB_SERVO14) |    \
                                     PIN_OSPEED_VERYLOW(GPIOB_SERVO15) |    \
                                     PIN_OSPEED_HIGH(GPIOB_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOB_JTDO) |          \
                                     PIN_OSPEED_HIGH(GPIOB_JNRST) |         \
                                     PIN_OSPEED_HIGH(GPIOB_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOB_GPS_TX) |        \
                                     PIN_OSPEED_HIGH(GPIOB_GPS_RX) |        \
                                     PIN_OSPEED_MEDIUM(GPIOB_I2C_SCL) |     \
                                     PIN_OSPEED_MEDIUM(GPIOB_I2C_SDA) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN10) |         \
                                     PIN_OSPEED_VERYLOW(GPIOB_SERVO6) |     \
                                     PIN_OSPEED_HIGH(GPIOB_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOB_PIN13) |         \
                                     PIN_OSPEED_VERYLOW(GPIOB_SERVO16) |    \
                                     PIN_OSPEED_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLDOWN(GPIOB_SERVO14) |    \
                                     PIN_PUPDR_PULLDOWN(GPIOB_SERVO15) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN2) |         \
                                     PIN_PUPDR_FLOATING(GPIOB_JTDO) |       \
                                     PIN_PUPDR_PULLUP(GPIOB_JNRST) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOB_GPS_TX) |       \
                                     PIN_PUPDR_PULLUP(GPIOB_GPS_RX) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C_SCL) |    \
                                     PIN_PUPDR_FLOATING(GPIOB_I2C_SDA) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN10) |        \
                                     PIN_PUPDR_PULLDOWN(GPIOB_SERVO6) |     \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN13) |        \
                                     PIN_PUPDR_PULLDOWN(GPIOB_SERVO16) |    \
                                     PIN_PUPDR_PULLUP(GPIOB_PIN15))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_SERVO14) |          \
                                     PIN_ODR_HIGH(GPIOB_SERVO15) |          \
                                     PIN_ODR_HIGH(GPIOB_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOB_JTDO) |             \
                                     PIN_ODR_HIGH(GPIOB_JNRST) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOB_GPS_TX) |           \
                                     PIN_ODR_HIGH(GPIOB_GPS_RX) |           \
                                     PIN_ODR_HIGH(GPIOB_I2C_SCL) |          \
                                     PIN_ODR_HIGH(GPIOB_I2C_SDA) |          \
                                     PIN_ODR_HIGH(GPIOB_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOB_SERVO6) |           \
                                     PIN_ODR_HIGH(GPIOB_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOB_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOB_SERVO16) |          \
                                     PIN_ODR_HIGH(GPIOB_PIN15))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_SERVO14, 2U) |       \
                                     PIN_AFIO_AF(GPIOB_SERVO15, 2U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_JTDO, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_JNRST, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_GPS_TX, 7U) |        \
                                     PIN_AFIO_AF(GPIOB_GPS_RX, 7U))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_I2C_SCL, 4U) |       \
                                     PIN_AFIO_AF(GPIOB_I2C_SDA, 4U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN10, 1U) |         \
                                     PIN_AFIO_AF(GPIOB_SERVO6, 1U) |        \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_SERVO16, 9U) |       \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0U))

/*
 * GPIOC setup:
 *
 * PC0  - PIN0                      (analog).
 * PC1  - PIN1                      (input pullup).
 * PC2  - PIN2                      (input pullup).
 * PC3  - PIN3                      (analog).
 * PC4  - PIN4                      (input pullup).
 * PC5  - PIN5                      (input pullup).
 * PC6  - EXBUS3                    (alternate 8).
 * PC7  - JETI3                     (alternate 8).
 * PC8  - PIN8                      (input pullup).
 * PC9  - PIN9                      (input pullup).
 * PC10 - EXBUS1                    (alternate 8).
 * PC11 - PIN11                     (input pullup).
 * PC12 - EXBUS2                    (alternate 8).
 * PC13 - PIN13                     (input pullup).
 * PC14 - PIN14                     (input pullup).
 * PC15 - PIN15                     (input pullup).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_ANALOG(GPIOC_PIN0) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN2) |           \
                                     PIN_MODE_ANALOG(GPIOC_PIN3) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN5) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_EXBUS3) |     \
                                     PIN_MODE_ALTERNATE(GPIOC_JETI3) |      \
                                     PIN_MODE_INPUT(GPIOC_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN9) |           \
                                     PIN_MODE_ALTERNATE(GPIOC_EXBUS1) |     \
                                     PIN_MODE_INPUT(GPIOC_PIN11) |          \
                                     PIN_MODE_ALTERNATE(GPIOC_EXBUS2) |     \
                                     PIN_MODE_INPUT(GPIOC_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_EXBUS3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_JETI3) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_EXBUS1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_EXBUS2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_HIGH(GPIOC_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN5) |          \
                                     PIN_OSPEED_VERYLOW(GPIOC_EXBUS3) |     \
                                     PIN_OSPEED_HIGH(GPIOC_JETI3) |         \
                                     PIN_OSPEED_HIGH(GPIOC_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOC_PIN9) |          \
                                     PIN_OSPEED_VERYLOW(GPIOC_EXBUS1) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN11) |         \
                                     PIN_OSPEED_VERYLOW(GPIOC_EXBUS2) |     \
                                     PIN_OSPEED_HIGH(GPIOC_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOC_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_FLOATING(GPIOC_PIN0) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_EXBUS3) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_JETI3) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_EXBUS1) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_EXBUS2) |       \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOC_EXBUS3) |           \
                                     PIN_ODR_HIGH(GPIOC_JETI3) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOC_EXBUS1) |           \
                                     PIN_ODR_HIGH(GPIOC_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOC_EXBUS2) |           \
                                     PIN_ODR_HIGH(GPIOC_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_EXBUS3, 8U) |        \
                                     PIN_AFIO_AF(GPIOC_JETI3, 8U))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_EXBUS1, 8U) |        \
                                     PIN_AFIO_AF(GPIOC_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_EXBUS2, 8U) |        \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0U))

/*
 * GPIOD setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - PIN1                      (input pullup).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (analog).
 * PD5  - PIN5                      (analog).
 * PD6  - PIN6                      (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (analog).
 * PD13 - PIN13                     (analog).
 * PD14 - PIN14                     (analog).
 * PD15 - PIN15                     (analog).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN3) |           \
                                     PIN_MODE_ANALOG(GPIOD_PIN4) |          \
                                     PIN_MODE_ANALOG(GPIOD_PIN5) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN11) |          \
                                     PIN_MODE_ANALOG(GPIOD_PIN12) |         \
                                     PIN_MODE_ANALOG(GPIOD_PIN13) |         \
                                     PIN_MODE_ANALOG(GPIOD_PIN14) |         \
                                     PIN_MODE_ANALOG(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_HIGH(GPIOD_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOD_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOD_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOD_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOD_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOD_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(GPIOD_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3) |         \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN5) |       \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN11) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOD_PIN11) |            \
                                     PIN_ODR_LOW(GPIOD_PIN12) |             \
                                     PIN_ODR_LOW(GPIOD_PIN13) |             \
                                     PIN_ODR_LOW(GPIOD_PIN14) |             \
                                     PIN_ODR_LOW(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0U))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0U))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                      (analog).
 * PE1  - PIN1                      (analog).
 * PE2  - PIN2                      (input floating).
 * PE3  - PIN3                      (analog).
 * PE4  - PIN4                      (input floating).
 * PE5  - PIN5                      (input floating).
 * PE6  - PIN6                      (input floating).
 * PE7  - PIN7                      (input floating).
 * PE8  - PIN8                      (input floating).
 * PE9  - SERVO7                    (alternate 1).
 * PE10 - PIN10                     (input floating).
 * PE11 - SERVO8                    (alternate 1).
 * PE12 - PIN12                     (input floating).
 * PE13 - SERVO9                    (alternate 1).
 * PE14 - SERVO10                   (alternate 1).
 * PE15 - PIN15                     (input floating).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_ANALOG(GPIOE_PIN0) |          \
                                     PIN_MODE_ANALOG(GPIOE_PIN1) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN2) |           \
                                     PIN_MODE_ANALOG(GPIOE_PIN3) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN8) |           \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO7) |     \
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO8) |     \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO9) |     \
                                     PIN_MODE_ALTERNATE(GPIOE_SERVO10) |    \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO8) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO9) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOE_SERVO10) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_HIGH(GPIOE_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOE_PIN8) |          \
                                     PIN_OSPEED_VERYLOW(GPIOE_SERVO7) |     \
                                     PIN_OSPEED_HIGH(GPIOE_PIN10) |         \
                                     PIN_OSPEED_VERYLOW(GPIOE_SERVO8) |     \
                                     PIN_OSPEED_HIGH(GPIOE_PIN12) |         \
                                     PIN_OSPEED_VERYLOW(GPIOE_SERVO9) |     \
                                     PIN_OSPEED_VERYLOW(GPIOE_SERVO10) |    \
                                     PIN_OSPEED_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_FLOATING(GPIOE_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN8) |       \
                                     PIN_PUPDR_PULLDOWN(GPIOE_SERVO7) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN10) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOE_SERVO8) |     \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN12) |      \
                                     PIN_PUPDR_PULLDOWN(GPIOE_SERVO9) |     \
                                     PIN_PUPDR_PULLDOWN(GPIOE_SERVO10) |    \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOE_SERVO7) |           \
                                     PIN_ODR_HIGH(GPIOE_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOE_SERVO8) |           \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOE_SERVO9) |           \
                                     PIN_ODR_HIGH(GPIOE_SERVO10) |          \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0U))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_SERVO7, 1U) |        \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_SERVO8, 1U) |        \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_SERVO9, 1U) |        \
                                     PIN_AFIO_AF(GPIOE_SERVO10, 1U) |       \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0U))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input floating).
 * PF1  - PIN1                      (input floating).
 * PF2  - PIN2                      (input floating).
 * PF3  - PIN3                      (input floating).
 * PF4  - PIN4                      (input floating).
 * PF5  - PIN5                      (input floating).
 * PF6  - PIN6                      (input floating).
 * PF7  - PIN7                      (input floating).
 * PF8  - PIN8                      (input floating).
 * PF9  - PIN9                      (input floating).
 * PF10 - PIN10                     (input floating).
 * PF11 - PIN11                     (input floating).
 * PF12 - PIN12                     (input floating).
 * PF13 - PIN13                     (input floating).
 * PF14 - PIN14                     (input floating).
 * PF15 - PIN15                     (input floating).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_HIGH(GPIOF_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOF_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0U))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0U))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input floating).
 * PG1  - PIN1                      (input floating).
 * PG2  - PIN2                      (input floating).
 * PG3  - PIN3                      (input floating).
 * PG4  - PIN4                      (input floating).
 * PG5  - PIN5                      (input floating).
 * PG6  - PIN6                      (input floating).
 * PG7  - PIN7                      (input floating).
 * PG8  - PIN8                      (input floating).
 * PG9  - PIN9                      (input floating).
 * PG10 - PIN10                     (input floating).
 * PG11 - PIN11                     (input floating).
 * PG12 - PIN12                     (input floating).
 * PG13 - PIN13                     (input floating).
 * PG14 - PIN14                     (input floating).
 * PG15 - PIN15                     (input floating).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_HIGH(GPIOG_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOG_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0U))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0U))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input floating).
 * PH3  - PIN3                      (input floating).
 * PH4  - PIN4                      (input floating).
 * PH5  - PIN5                      (input floating).
 * PH6  - PIN6                      (input floating).
 * PH7  - PIN7                      (input floating).
 * PH8  - PIN8                      (input floating).
 * PH9  - PIN9                      (input floating).
 * PH10 - PIN10                     (input floating).
 * PH11 - PIN11                     (input floating).
 * PH12 - PIN12                     (input floating).
 * PH13 - PIN13                     (input floating).
 * PH14 - PIN14                     (input floating).
 * PH15 - PIN15                     (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_HIGH(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_HIGH(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_HIGH(GPIOH_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOH_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0U) |        \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0U) |       \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0U))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0U))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0                      (input floating).
 * PI1  - PIN1                      (input floating).
 * PI2  - PIN2                      (input floating).
 * PI3  - PIN3                      (input floating).
 * PI4  - PIN4                      (input floating).
 * PI5  - PIN5                      (input floating).
 * PI6  - PIN6                      (input floating).
 * PI7  - PIN7                      (input floating).
 * PI8  - PIN8                      (input floating).
 * PI9  - PIN9                      (input floating).
 * PI10 - PIN10                     (input floating).
 * PI11 - PIN11                     (input floating).
 * PI12 - PIN12                     (input floating).
 * PI13 - PIN13                     (input floating).
 * PI14 - PIN14                     (input floating).
 * PI15 - PIN15                     (input floating).
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_HIGH(GPIOI_PIN0) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN1) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN2) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN3) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN4) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN5) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN6) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN7) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN8) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN9) |          \
                                     PIN_OSPEED_HIGH(GPIOI_PIN10) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN11) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN12) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN13) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN14) |         \
                                     PIN_OSPEED_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_FLOATING(GPIOI_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0U))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0U))

/*
 * GPIOJ setup:
 *
 * PJ0  - LCD_R1                    (alternate 14).
 * PJ1  - LCD_R2                    (alternate 14).
 * PJ2  - LCD_R3                    (alternate 14).
 * PJ3  - LCD_R4                    (alternate 14).
 * PJ4  - LCD_R5                    (alternate 14).
 * PJ5  - LCD_R6                    (alternate 14).
 * PJ6  - LCD_R7                    (alternate 14).
 * PJ7  - LCD_G0                    (alternate 14).
 * PJ8  - LCD_G1                    (alternate 14).
 * PJ9  - LCD_G2                    (alternate 14).
 * PJ10 - LCD_G3                    (alternate 14).
 * PJ11 - LCD_G4                    (alternate 14).
 * PJ12 - OTG_FS_VBUS               (input floating).
 * PJ13 - LCD_B1                    (alternate 14).
 * PJ14 - LCD_B2                    (alternate 14).
 * PJ15 - LCD_B3                    (alternate 14).
 */
#define VAL_GPIOJ_MODER             (PIN_MODE_ALTERNATE(GPIOJ_LCD_R1) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_R2) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_R3) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_R4) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_R5) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_R6) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_R7) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_G0) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_G1) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_G2) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_G3) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_G4) |     \
                                     PIN_MODE_INPUT(GPIOJ_OTG_FS_VBUS) |    \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_B1) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_B2) |     \
                                     PIN_MODE_ALTERNATE(GPIOJ_LCD_B3))
#define VAL_GPIOJ_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R5) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R6) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_R7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_G0) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_G1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_G2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_G3) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_G4) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_OTG_FS_VBUS) |\
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_B1) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_B2) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_LCD_B3))
#define VAL_GPIOJ_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOJ_LCD_R1) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_R2) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_R3) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_R4) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_R5) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_R6) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_R7) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_G0) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_G1) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_G2) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_G3) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_G4) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_OTG_FS_VBUS) |\
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_B1) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_B2) |     \
                                     PIN_OSPEED_VERYLOW(GPIOJ_LCD_B3))
#define VAL_GPIOJ_PUPDR             (PIN_PUPDR_FLOATING(GPIOJ_LCD_R1) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_R2) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_R3) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_R4) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_R5) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_R6) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_R7) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_G0) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_G1) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_G2) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_G3) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_G4) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_OTG_FS_VBUS) |\
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_B1) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_B2) |     \
                                     PIN_PUPDR_FLOATING(GPIOJ_LCD_B3))
#define VAL_GPIOJ_ODR               (PIN_ODR_HIGH(GPIOJ_LCD_R1) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_R2) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_R3) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_R4) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_R5) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_R6) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_R7) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_G0) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_G1) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_G2) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_G3) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_G4) |           \
                                     PIN_ODR_HIGH(GPIOJ_OTG_FS_VBUS) |      \
                                     PIN_ODR_HIGH(GPIOJ_LCD_B1) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_B2) |           \
                                     PIN_ODR_HIGH(GPIOJ_LCD_B3))
#define VAL_GPIOJ_AFRL              (PIN_AFIO_AF(GPIOJ_LCD_R1, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_R2, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_R3, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_R4, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_R5, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_R6, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_R7, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_G0, 14U))
#define VAL_GPIOJ_AFRH              (PIN_AFIO_AF(GPIOJ_LCD_G1, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_G2, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_G3, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_G4, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_OTG_FS_VBUS, 0U) |   \
                                     PIN_AFIO_AF(GPIOJ_LCD_B1, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_B2, 14U) |       \
                                     PIN_AFIO_AF(GPIOJ_LCD_B3, 14U))

/*
 * GPIOK setup:
 *
 * PK0  - LCD_G5                    (alternate 14).
 * PK1  - LCD_G6                    (alternate 14).
 * PK2  - LCD_G7                    (alternate 14).
 * PK3  - LCD_BL_CTRL               (output pushpull minimum).
 * PK4  - LCD_B5                    (alternate 14).
 * PK5  - LCD_B6                    (alternate 14).
 * PK6  - LCD_B7                    (alternate 14).
 * PK7  - LCD_DE                    (alternate 14).
 * PK8  - PIN8                      (input floating).
 * PK9  - PIN9                      (input floating).
 * PK10 - PIN10                     (input floating).
 * PK11 - PIN11                     (input floating).
 * PK12 - PIN12                     (input floating).
 * PK13 - PIN13                     (input floating).
 * PK14 - PIN14                     (input floating).
 * PK15 - PIN15                     (input floating).
 */
#define VAL_GPIOK_MODER             (PIN_MODE_ALTERNATE(GPIOK_LCD_G5) |     \
                                     PIN_MODE_ALTERNATE(GPIOK_LCD_G6) |     \
                                     PIN_MODE_ALTERNATE(GPIOK_LCD_G7) |     \
                                     PIN_MODE_OUTPUT(GPIOK_LCD_BL_CTRL) |   \
                                     PIN_MODE_ALTERNATE(GPIOK_LCD_B5) |     \
                                     PIN_MODE_ALTERNATE(GPIOK_LCD_B6) |     \
                                     PIN_MODE_ALTERNATE(GPIOK_LCD_B7) |     \
                                     PIN_MODE_ALTERNATE(GPIOK_LCD_DE) |     \
                                     PIN_MODE_INPUT(GPIOK_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN15))
#define VAL_GPIOK_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOK_LCD_G5) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_G6) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_G7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_BL_CTRL) |\
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_B5) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_B6) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_B7) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_LCD_DE) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN15))
#define VAL_GPIOK_OSPEEDR           (PIN_OSPEED_VERYLOW(GPIOK_LCD_G5) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_G6) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_G7) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_BL_CTRL) |\
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_B5) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_B6) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_B7) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_LCD_DE) |     \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN8) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN9) |       \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN10) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN11) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN12) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN13) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN14) |      \
                                     PIN_OSPEED_VERYLOW(GPIOK_PIN15))
#define VAL_GPIOK_PUPDR             (PIN_PUPDR_FLOATING(GPIOK_LCD_G5) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_G6) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_G7) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_BL_CTRL) |\
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_B5) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_B6) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_B7) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_LCD_DE) |     \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN15))
#define VAL_GPIOK_ODR               (PIN_ODR_HIGH(GPIOK_LCD_G5) |           \
                                     PIN_ODR_HIGH(GPIOK_LCD_G6) |           \
                                     PIN_ODR_HIGH(GPIOK_LCD_G7) |           \
                                     PIN_ODR_LOW(GPIOK_LCD_BL_CTRL) |       \
                                     PIN_ODR_HIGH(GPIOK_LCD_B5) |           \
                                     PIN_ODR_HIGH(GPIOK_LCD_B6) |           \
                                     PIN_ODR_HIGH(GPIOK_LCD_B7) |           \
                                     PIN_ODR_HIGH(GPIOK_LCD_DE) |           \
                                     PIN_ODR_HIGH(GPIOK_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOK_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOK_PIN15))
#define VAL_GPIOK_AFRL              (PIN_AFIO_AF(GPIOK_LCD_G5, 14U) |       \
                                     PIN_AFIO_AF(GPIOK_LCD_G6, 14U) |       \
                                     PIN_AFIO_AF(GPIOK_LCD_G7, 14U) |       \
                                     PIN_AFIO_AF(GPIOK_LCD_BL_CTRL, 0U) |   \
                                     PIN_AFIO_AF(GPIOK_LCD_B5, 14U) |       \
                                     PIN_AFIO_AF(GPIOK_LCD_B6, 14U) |       \
                                     PIN_AFIO_AF(GPIOK_LCD_B7, 14U) |       \
                                     PIN_AFIO_AF(GPIOK_LCD_DE, 14U))
#define VAL_GPIOK_AFRH              (PIN_AFIO_AF(GPIOK_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN15, 0U))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
