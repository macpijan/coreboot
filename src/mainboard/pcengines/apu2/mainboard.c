/*
 * This file is part of the coreboot project.
 *
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <console/console.h>
#include <device/device.h>
#include <device/pci.h>
#include <arch/io.h>
#include <device/pci_def.h>
#include <arch/acpi.h>
#include <northbridge/amd/pi/BiosCallOuts.h>
#include <cpu/amd/pi/s3_resume.h>
#include <northbridge/amd/pi/agesawrapper.h>
#include <cpu/x86/msr.h>
#include <cpu/amd/mtrr.h>
#if CONFIG_USE_OPTION_TABLE
#include <pc80/mc146818rtc.h>
#endif //CONFIG_USE_OPTION_TABLE
#if CONFIG_HAVE_OPTION_TABLE
#include "option_table.h"
#endif //CONFIG_HAVE_OPTION_TABLE
#include <hudson.h>
#include <timestamp.h>
#include "apu2.h"
#include <superio/nuvoton/nct5104d/nct5104d.h>
#include <spd_cache.h>

#define SPD_SIZE  128
#define PM_RTC_CONTROL	    0x56
#define PM_S_STATE_CONTROL  0xBA

/* Wrapper to enable GPIO/UART devices under menuconfig. Revisit
 * once configuration file format for SPI flash storage is complete.
 */
#define SIO_PORT 0x2e

static void config_gpio_mux(void)
{
	struct device *uart, *gpio;

	uart = dev_find_slot_pnp(SIO_PORT, NCT5104D_SP3);
	gpio = dev_find_slot_pnp(SIO_PORT, NCT5104D_GPIO0);
	if (uart)
		uart->enabled = CONFIG_PINMUX_UART_C;
	if (gpio)
		gpio->enabled = CONFIG_PINMUX_GPIO0;

	uart = dev_find_slot_pnp(SIO_PORT, NCT5104D_SP4);
	gpio = dev_find_slot_pnp(SIO_PORT, NCT5104D_GPIO1);
	if (uart)
		uart->enabled = CONFIG_PINMUX_UART_D;
	if (gpio)
		gpio->enabled = CONFIG_PINMUX_GPIO1;
}

/**********************************************
 * enable the dedicated function in mainboard.
 **********************************************/

static void mainboard_enable(device_t dev)
{
	printk(BIOS_INFO, "Mainboard " CONFIG_MAINBOARD_PART_NUMBER " Enable.\n");

	config_gpio_mux();

	//
	// Enable the RTC output
	//
	pm_write16 ( PM_RTC_CONTROL, pm_read16( PM_RTC_CONTROL ) | (1 << 11));

	//
	// Enable power on from WAKE#
	//
	pm_write16 ( PM_S_STATE_CONTROL, pm_read16( PM_S_STATE_CONTROL ) | (1 << 14));

	if (acpi_is_wakeup_s3())
		agesawrapper_fchs3earlyrestore();
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
};


