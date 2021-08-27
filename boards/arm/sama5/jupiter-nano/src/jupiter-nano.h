/****************************************************************************
 * boards/arm/sama5/jupiter-nano/src/jupiter-nano.h
 *
 *  Licensed to the Apache Software Foundation (ASF) under one or more
 *  contributor license agreements.  See the NOTICE file distributed with
 *  this work for additional information regarding copyright ownership.  The
 *  ASF licenses this file to you under the Apache License, Version 2.0 (the
 *  "License"); you may not use this file except in compliance with the
 *  License.  You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 *  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 *  License for the specific language governing permissions and limitations
 *  under the License.
 *
 ****************************************************************************/

#ifndef __BOARDS_ARM_SAMA5_JUPITER_NANO_SRC_JUPITER_NANO_H
#define __BOARDS_ARM_SAMA5_JUPITER_NANO_SRC_JUPITER_NANO_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>
#include <nuttx/compiler.h>

#include <stdint.h>
#include <stdbool.h>

#include <arch/irq.h>
#include <nuttx/irq.h>

#include "hardware/sam_pinmap.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* Configuration ************************************************************/

#define HAVE_SDMMC      1
#define HAVE_AT25       1
#define HAVE_NAND       1
#define HAVE_USBHOST    1
#define HAVE_USBDEV     1
#define HAVE_USBMONITOR 1
#define HAVE_NETWORK    1

/* SDMMC */

/* Can't support MMC/SD if the card interface(s) are not enable */

#if !defined(CONFIG_SAMA5_SDMMC) && !defined(CONFIG_SAMA5_SDMMC1)
#  undef HAVE_SDMMC
#endif

/* Can't support MMC/SD features if mountpoints are disabled */

#if defined(HAVE_SDMMC) && defined(CONFIG_DISABLE_MOUNTPOINT)
#  warning Mountpoints disabled.  No MMC/SD support
#  undef HAVE_SDMCC
#endif

/* We need PIO interrupts on PIOD to support card detect interrupts */

#if defined(HAVE_SDMMC) && !defined(CONFIG_SAMA5_PIOA_IRQ)
#  warning PIOA interrupts not enabled.  No MMC/SD support.
#  undef HAVE_SDMMC
#endif

/* No NAND flash */
#undef HAVE_NAND

/* no AT25 Serial EEPROM */
#undef HAVE_AT25

/* MMC/SD minor numbers:  The NSH device minor extended is extended to
 * support two devices.  If CONFIG_NSH_MMCSDMINOR is zero, these will be:
 * /dev/mmcsd0 and /dev/mmcsd1.
 */

#ifndef CONFIG_NSH_MMCSDMINOR
#  define CONFIG_NSH_MMCSDMINOR 0
#endif

#ifdef HAVE_SDMMC

#if ( defined(CONFIG_SAMA5_SDMMC1) )
#    define SDMMC1_SLOTNO 0
#endif

#define SDMMC1_MINOR  CONFIG_NSH_MMCSDMINOR
#else

#endif

/* USB Host / USB Device */

/* Either CONFIG_SAMA5_UHPHS or CONFIG_SAMA5_UDPHS must be defined,
 * or there is no USB of any kind.
 */

#if !defined(CONFIG_SAMA5_UHPHS)
#  undef CONFIG_SAMA5_OHCI
#  undef CONFIG_SAMA5_EHCI
#endif

#if !defined(CONFIG_SAMA5_UDPHS)
#  undef HAVE_USBDEV
#endif

/* CONFIG_USBDEV and CONFIG_USBHOST must also be defined */

#if !defined(CONFIG_USBDEV)
#  undef HAVE_USBDEV
#endif

#if defined(CONFIG_USBHOST)
#  if !defined(CONFIG_SAMA5_OHCI) && !defined(CONFIG_SAMA5_EHCI)
#    warning CONFIG_USBHOST is defined, but neither CONFIG_SAMA5_OHCI nor CONFIG_SAMA5_EHCI are defined
#  endif
#else
#  undef CONFIG_SAMA5_OHCI
#  undef CONFIG_SAMA5_EHCI
#endif

#if !defined(CONFIG_SAMA5_OHCI) && !defined(CONFIG_SAMA5_EHCI)
#  undef HAVE_USBHOST
#endif

/* Check if we should enable the USB monitor before starting NSH */

#ifndef CONFIG_USBMONITOR
#  undef HAVE_USBMONITOR
#endif

#ifndef HAVE_USBDEV
#  undef CONFIG_USBDEV_TRACE
#endif

#ifndef HAVE_USBHOST
#  undef CONFIG_USBHOST_TRACE
#endif

#if !defined(CONFIG_USBDEV_TRACE) && !defined(CONFIG_USBHOST_TRACE)
#  undef HAVE_USBMONITOR
#endif

/* Networking */

#if !defined(CONFIG_NET) || (!defined(CONFIG_SAMA5_EMACA) && !defined(CONFIG_SAMA5_GMAC))
#  undef HAVE_NETWORK
#endif

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define SAMA5_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define SAMA5_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/* LEDs *********************************************************************/

/* There is a blue LED on board the Jupiter Nano 
 * The LEDs are provided VDD_LED and so bringing the LED low will illuminate
 * the LED.
 *
 *   ------------------------------ ------------------- ---------------------
 *   SAMA5D2 PIO                    SIGNAL              USAGE
 *   ------------------------------ ------------------- ---------------------
 *   PA6                            STATUS_LED_PA6      Blue LED
 *   ------------------------------ ------------------- ---------------------
 */

#define PIO_LED_BLUE  (PIO_OUTPUT | PIO_CFG_DEFAULT | PIO_OUTPUT_SET | \
                       PIO_PORT_PIOA | PIO_PIN6)

/* SDMMC clocking
 *
 * Multimedia Card Interface clock (MCCK or MCI_CK) is Master Clock (MCK)
 * divided by (2*(CLKDIV+1)).
 *
 *   MCI_SPEED = MCK / (2*(CLKDIV+1))
 *   CLKDIV = MCI / MCI_SPEED / 2 - 1
 *
 * Where CLKDIV has a range of 0-255.
 */

/* MCK = 96MHz, CLKDIV = 119, MCI_SPEED = 96MHz / 2 * (119+1) = 400 KHz */

#define SDMMC_INIT_CLKDIV          (119 << SDMMC_MR_CLKDIV_SHIFT)

/* MCK = 96MHz, CLKDIV = 3, MCI_SPEED = 96MHz / 2 * (3+1) = 12 MHz */

#define SDMMC_MMCXFR_CLKDIV        (3 << SDMMC_MR_CLKDIV_SHIFT)

/* MCK = 96MHz, CLKDIV = 1, MCI_SPEED = 96MHz / 2 * (1+1) = 24 MHz */

#define SDMMC_SDXFR_CLKDIV         (1 << SDMMC_MR_CLKDIV_SHIFT)
#define SDMMC_SDWIDEXFR_CLKDIV     SDMMC_SDXFR_CLKDIV

/* SDMMC Card Slots *********************************************************/

/* The JUPITER-NANO provides a SD memory card slot:
 *  a full size SD card slot (J19)
 *
 * The full size SD card slot connects via SDMMC1.  The card detect discrete
 * is available on PA30 (pulled high).  The write protect discrete is tied to
 * ground and not available to software.  The slot only supports 4-bit
 * wide transfer mode, and the NuttX driver currently uses only the 4-bit
 * wide transfer mode.
 *
 *   PA30 SDMMC1_CD
 *   PA18 SDMMC1_DAT0
 *   PA19 SDMMC1_DAT1
 *   PA20 SDMMC1_DAT2
 *   PA21 SDMMC1_DAT3
 *   PA22 SDMMC1_CK
 *   PA28 SDMMC1_CDA
 */

#define IRQ_SDMMC1_CD   SAM_IRQ_PA30

/* USB Ports ****************************************************************/

/* The JUPITER-NANO features two USB communication ports:
 *
 *   1. Port A Host High Speed (EHCI) and Full Speed (OHCI) multiplexed with
 *      USB Device High Speed Micro AB connector, J2
 *
 *   2. Port B Host High Speed (EHCI) and Full Speed (OHCI) standard type A
 *      connector, J7
 *
 */

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

/****************************************************************************
 * Name: sam_bringup
 *
 * Description:
 *   Bring up board features
 *
 ****************************************************************************/

int sam_bringup(void);

/****************************************************************************
 * Name: sam_sdmmc_initialize
 *
 * Description:
 *   Initialize and configure one SDMMC slot
 *
 ****************************************************************************/

#ifdef HAVE_SDMMC
int sam_sdmmc_initialize(int slotno, int minor);
#endif

/****************************************************************************
 * Name: sam_cardinserted
 *
 * Description:
 *   Check if a card is inserted into the selected SDMMC slot
 *
 ****************************************************************************/

#ifdef HAVE_SDMMC
bool sam_cardinserted(int slotno);
#endif

/****************************************************************************
 * Name: sam_writeprotected
 *
 * Description:
 *   Check if the card in the MMCSD slot is write protected
 *
 ****************************************************************************/

#ifdef HAVE_HSMCI
bool sam_writeprotected(int slotno);
#endif

/****************************************************************************
 * Name: sam_usbinitialize
 *
 * Description:
 *   Called from sam_usbinitialize very early in initialization to setup
 *   USB-related PIO pins for the JUPITER-NANO board.
 *
 ****************************************************************************/

#if defined(CONFIG_SAMA5_UHPHS) || defined(CONFIG_SAMA5_UDPHS)
void weak_function sam_usbinitialize(void);
#endif

/****************************************************************************
 * Name: stm32_usbhost_initialize
 *
 * Description:
 *   Called at application startup time to initialize the USB host
 *   functionality.
 *   This function will start a thread that will monitor for device
 *   connection/disconnection events.
 *
 ****************************************************************************/

#ifdef HAVE_USBHOST
int sam_usbhost_initialize(void);
#endif

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_ARM__SAMA5_JUPITER_NANO_SRC_JUPITER_NANO_H */
