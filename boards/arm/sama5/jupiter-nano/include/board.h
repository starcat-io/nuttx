/****************************************************************************
 * boards/arm/sama5/jupiter-nano/include/board.h
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_ARM_SAMA5_JUPITER_NANO_INCLUDE_BOARD_H
#define __BOARDS_ARM_SAMA5_JUPITER_NANO_INCLUDE_BOARD_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#ifndef __ASSEMBLY__
#  include <stdbool.h>
#  include <nuttx/irq.h>
#endif

/* Clocking *****************************************************************/

/* On-board crystal frequencies */

#define BOARD_MAINOSC_FREQUENCY    (12000000)  /* MAINOSC: 12MHz crystal on-board */
#define BOARD_SLOWCLK_FREQUENCY    (32768)     /* Slow Clock: 32.768KHz */

/* After power-on reset, the SAMA5 device is running on a 12MHz internal RC.
 * These definitions will configure operational clocking.
 */

#if defined(CONFIG_SAMA5_BOOT_SDRAM)
/* When booting from SDRAM, NuttX is loaded in SDRAM by an intermediate
 * bootloader.
 * That bootloader had to have already configured the PLL and SDRAM for
 * proper operation.
 *
 * In this case, we don not reconfigure the clocking.
 * Rather, we need to query the register settings to determine the clock
 * frequencies.
 * We can only assume that the Main clock source is the on-board 12MHz
 * crystal.
 */

#  include <arch/board/board_sdram.h>

#elif defined(CONFIG_JUPITER_NANO_498MHZ)

/* This is the configuration results in a CPU clock of 498MHz.
 *
 * In this configuration, UPLL is the source of the UHPHS clock (if enabled).
 */

#  include <arch/board/board_498mhz.h>

#endif

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* LED definitions **********************************************************/

 *
 *   ------------------------------ ------------------- ---------------------
 *   SAMA5D2 PIO                    SIGNAL              USAGE
 *   ------------------------------ ------------------- ---------------------
 *   PA6                            PA6                 Blue Status LED
 *   ------------------------------ ------------------- ---------------------
 */

#ifndef CONFIG_ARCH_LEDS

/* LED index values for use with board_userled() */

#define BOARD_BLUE        0
#define BOARD_NLEDS       1

#else

/* LED index values for use with board_userled() */

#define BOARD_BLUE        0
#define BOARD_NLEDS       1

/* LED bits for use with board_userled_all() */

#define BOARD_BLUE_BIT   (1 << BOARD_BLUE)
#endif

/* These LEDs are not used by the board port unless CONFIG_ARCH_LEDS is
 * defined.  In that case, the usage by the board port is defined in
 * include/board.h and src/sam_leds.c. The LEDs are used to encode OS-related
 * events as follows.  Note that only the GREEN LED is used in this case
 *
 *      SYMBOL            Val    Meaning                   Blue LED
 *      ----------------- ---   -----------------------  -----------
 */

#define LED_STARTED       0  /* NuttX has been started     OFF       */
#define LED_HEAPALLOCATE  0  /* Heap has been allocated    OFF       */
#define LED_IRQSENABLED   0  /* Interrupts enabled         OFF       */
#define LED_STACKCREATED  1  /* Idle stack created         ON        */
#define LED_INIRQ         2  /* In an interrupt            N/C       */
#define LED_SIGNAL        2  /* In a signal handler        N/C       */
#define LED_ASSERTION     2  /* An assertion failed        N/C       */
#define LED_PANIC         3  /* The system has crashed     Flash     */
#undef  LED_IDLE             /* MCU is is sleep mode       Not used  */

/* Thus if the Blue Status LED is statically on, NuttX has successfully  booted
 * and is, apparently, running normally.
 * If LED is flashing at approximately 2Hz, then a fatal error has been
 * detected and the system has halted.
 */

/* Pin disambiguation *******************************************************/

/* Alternative pin selections are provided with a numeric suffix like _1, _2,
 * etc. Drivers, however, will use the pin selection without the numeric
 * suffix.
 * Additional definitions are required in this board.h file.
 * For example, if we wanted the PCK0on PB26, then the following definition
 * should appear in the board.h header file for that board:
 *
 *   #define PIO_PMC_PCK0 PIO_PMC_PCK0_1
 *
 * The PCK logic will then automatically configure PB26 as the PCK0 pin.
 */

/* USB Console. The UART1 serial console is connected the MCP2200
 * USB-UART converter connected to the Micro USB connector labeled CONSOLE.  
 *  
 *   ------------------------ -------------
 *   SCHEMATIC                   SAMA5D2
 *   NAME(s)                  PIO  FUNCTION
 *   ------------------------ -------------
 *   USB_U_P                  PD3  UTXD1
 *   USB_U_N                  PD2  URXD1
 *   ------------------------ -------------
 */

#define PIO_UART1_RXD     PIO_UART1_RXD_1
#define PIO_UART1_TXD     PIO_UART1_TXD_1

/* Other USARTs are available:
 *
 *   ---- ------- -------------
 *   J22  BOARD      SAMA5D2
 *   PIN  NAME    PIO  FUNCTION
 *   ---- ------- -------------
 *    3   F0_TXD  PB28 FLEXCOM0
 *    4   F0_RXD  PB29 FLEXCOM0
 *    5   F3_TXD  PB23 FLEXCOM2
 *    6   F3_RXD  PB22 FLEXCOM2
 *    5   F3_TXD  PB23 FLEXCOM3
 *    6   F3_RXD  PB22 FLEXCOM3
 *    5   F3_TXD  PB23 FLEXCOM4
 *    6   F3_RXD  PB22 FLEXCOM4
 *   ---- ------- -------------
 */

#define PIO_FLEXCOM3_IO0  PIO_FLEXCOM3_IO0_2
#define PIO_FLEXCOM3_IO1  PIO_FLEXCOM3_IO1_2

/* SDIO - Used for both Port 0 & 1 ******************************************/

/* 386 KHz for initial inquiry stuff */

#define BOARD_SDMMC_IDMODE_PRESCALER    SDMMC_SYSCTL_SDCLKFS_DIV256
#define BOARD_SDMMC_IDMODE_DIVISOR      SDMMC_SYSCTL_DVS_DIV(2)

/* 24.8MHz for other modes */

#define BOARD_SDMMC_MMCMODE_PRESCALER   SDMMC_SYSCTL_SDCLKFS_DIV8
#define BOARD_SDMMC_MMCMODE_DIVISOR     SDMMC_SYSCTL_DVS_DIV(1)

#define BOARD_SDMMC_SD1MODE_PRESCALER   SDMMC_SYSCTL_SDCLKFS_DIV8
#define BOARD_SDMMC_SD1MODE_DIVISOR     SDMMC_SYSCTL_DVS_DIV(1)

#define BOARD_SDMMC_SD4MODE_PRESCALER   SDMMC_SYSCTL_SDCLKFS_DIV8
#define BOARD_SDMMC_SD4MODE_DIVISOR     SDMMC_SYSCTL_DVS_DIV(1)

/****************************************************************************
 * Assembly Language Macros
 ****************************************************************************/

#ifdef __ASSEMBLY__
  .macro config_sdram
  .endm
#endif /* __ASSEMBLY__ */

#endif /* __BOARDS_ARM_SAMA5_JUPITER_NANO_INCLUDE_BOARD_H */
