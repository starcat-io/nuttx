/****************************************************************************
 * boards/arm/sama5/sama5d2-xult/src/sama5d2-xult.h
 *
 *   Copyright (C) 2015 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#ifndef __BOARDS_ARM_SAMA5_SAMA5D2_XULT_SRC_SAMA5D2_XULT_H
#define __BOARDS_ARM_SAMA5_SAMA5D2_XULT_SRC_SAMA5D2_XULT_H

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

#define HAVE_HSMCI      1
#define HAVE_AT25       1
#define HAVE_NAND       1
#define HAVE_USBHOST    1
#define HAVE_USBDEV     1
#define HAVE_USBMONITOR 1
#define HAVE_NETWORK    1

/* procfs File System */

#ifdef CONFIG_FS_PROCFS
#  ifdef CONFIG_NSH_PROC_MOUNTPOINT
#    define SAMA5_PROCFS_MOUNTPOINT CONFIG_NSH_PROC_MOUNTPOINT
#  else
#    define SAMA5_PROCFS_MOUNTPOINT "/proc"
#  endif
#endif

/* LEDs *********************************************************************/

/* There is an RGB LED on board the SAMA5D2-XULT.
 * The RED component is driven by the SDHC_CD pin (PA13) and so will not
 * be used.
 * The LEDs are provided VDD_LED and so bringing the LED low will illuminate
 * the LED.
 *
 *   ------------------------------ ------------------- ---------------------
 *   SAMA5D2 PIO                    SIGNAL              USAGE
 *   ------------------------------ ------------------- ---------------------
 *   PA13                           SDHC_CD_PA13        Red LED
 *   PB5                            LED_GREEN_PB5       Green LED
 *   PB0                            LED_BLUE_PB0        Blue LED
 *   ------------------------------ ------------------- ---------------------
 */

#define PIO_LED_GREEN (PIO_OUTPUT | PIO_CFG_DEFAULT | PIO_OUTPUT_SET | \
                       PIO_PORT_PIOB | PIO_PIN5)
#define PIO_LED_BLUE  (PIO_OUTPUT | PIO_CFG_DEFAULT | PIO_OUTPUT_SET | \
                       PIO_PORT_PIOB | PIO_PIN0)

/* Buttons ******************************************************************/

/* A single button, PB_USER (PB6), is available on the SAMA5D2-XULT
 *
 *  ------------------------------ ------------------- ----------------------
 *  SAMA5D2 PIO                    SIGNAL              USAGE
 *  ------------------------------ ------------------- ----------------------
 *  PB6                            USER_PB_PB6         PB_USER push button
 *  ------------------------------ ------------------- ----------------------
 *
 *  Closing PB_USER will bring PB6 to ground so 1) PB6 should have a weak
 *  pull-up, and 2) when PB_USER is pressed, a low value will be senses.
 */

#define PIO_BTN_USER (PIO_INPUT | PIO_CFG_PULLUP | PIO_CFG_DEGLITCH | \
                      PIO_INT_BOTHEDGES | PIO_PORT_PIOB | PIO_PIN6)
#define IRQ_BTN_USER  SAM_IRQ_PB6

/* USB Ports ****************************************************************/

/* The SAMA5D2-XULT features two USB communication ports:
 *
 *   1. Port A Host High Speed (EHCI) and Full Speed (OHCI) multiplexed with
 *      USB Device High Speed Micro AB connector, J23
 *
 *   2. Port B Host High Speed (EHCI) and Full Speed (OHCI) standard type A
 *      connector, J13
 *
 * The USB host port (only) is equipped with 500-mA high-side power
 * switch for self-powered and bus-powered applications.
 *
 * The USB device port A (J6) features a VBUS insert detection function.
 *
 *
 * Port A
 *
 *   PIO  Signal Name Function
 *   ---- ----------- -------------------------------------------------------
 *   PE9  VBUS_SENSE VBus detection
 *
 *     Note: No VBus power switch enable on port A.  I think that this limits
 *     this port to a device port or as a host port for self-powered devices
 *     only.
 */

#define PIO_USBA_VBUS_SENSE \
                     (PIO_INPUT | PIO_CFG_PULLUP | PIO_CFG_DEGLITCH | \
                      PIO_INT_BOTHEDGES | PIO_PORT_PIOA | PIO_PIN31)
#define IRQ_USBA_VBUS_SENSE \
                     SAM_IRQ_PA31

/* Port B
 *
 *   PIO  Signal Name Function
 *   ---- ----------- -------------------------------------------------------
 *   PE3  EN5V_USBB   VBus power enable via MN3 SP2526A-2E dual power
 *                    switch.  PE3 (EN5V_USBB)connects to ENB pin of MN3.
 *                    MN3 OUTB (5V_USBB) is provided to pin 1 of J13 USB
 *                    A connector
 *
 *                    Active high for SP2526A-1; active low for SP2526A-2
 */

#define PIO_USBB_VBUS_ENABLE \
                     (PIO_OUTPUT | PIO_CFG_DEFAULT | PIO_OUTPUT_SET | \
                      PIO_PORT_PIOB | PIO_PIN10)

/*  Ports B
 *
 *   PIO  Signal Name Function
 *   ---- ----------- -------------------------------------------------------
 *   PE5  OVCUR_USB   Over-current indication from B
 */

#define PIO_USBB_VBUS_OVERCURRENT \
                     (PIO_INPUT | PIO_CFG_PULLUP | PIO_CFG_DEGLITCH | \
                      PIO_INT_BOTHEDGES | PIO_PORT_PIOA | PIO_PIN29)
#define IRQ_USBB_VBUS_OVERCURRENT \
                     SAM_IRQ_PA29

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public data
 ****************************************************************************/

#ifndef __ASSEMBLY__

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: sam_bringup
 *
 * Description:
 *   Bring up board features
 *
 ****************************************************************************/

int sam_bringup(void);

#endif /* __ASSEMBLY__ */
#endif /* __BOARDS_ARM__SAMA5SAMA5D2_XULT_SRC_SAMA5D2_XULT_H */
