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
#include <southbridge/amd/pi/hudson/hudson.h>
#include <northbridge/amd/pi/agesawrapper.h>
#include <cpu/x86/msr.h>
#include <cpu/amd/mtrr.h>
#if CONFIG_USE_OPTION_TABLE
#include <pc80/mc146818rtc.h>
#endif //CONFIG_USE_OPTION_TABLE
#if CONFIG_HAVE_OPTION_TABLE
#include "option_table.h"
#endif //CONFIG_HAVE_OPTION_TABLE
#include <timestamp.h>
#include <fchgpio.h>
#include "apu2.h"
#include <superio/nuvoton/nct5104d/nct5104d.h>
#include <cbfs.h>
#include <spd_cache.h>

/**********************************************
 * enable the dedicated function in mainboard.
 **********************************************/

static void mainboard_enable(device_t dev)
{
	struct device *sio_dev;

	setup_bsp_ramtop();
	u32 TOM1 = bsp_topmem() / (1024 *1024);	// Tom1 in Mbyte
	u32 TOM2 = ( bsp_topmem2() / (1024 *1024)) - 4 * 1024;	// Tom2 in Mbyte
	printk(BIOS_ERR, "%d MB", TOM1+TOM2);

	u8 spd_buffer[128];
	int	index = 0;

	/* One SPD file contains all 4 options, determine which index to read here, then call into the standard routines*/

	if ( ReadFchGpio(APU2_SPD_STRAP0_GPIO) ) index |= BIT0;
	if ( ReadFchGpio(APU2_SPD_STRAP1_GPIO) ) index |= BIT1;

	printk(BIOS_SPEW, "Reading SPD index %d to get ECC info \n", index);
	if (read_spd_from_cbfs(spd_buffer, index) < 0)
		spd_buffer[3]=3;	// Indicate no ECC

	if ( spd_buffer[3] == 8 ) 	printk(BIOS_ERR, " ECC");
	printk(BIOS_ERR, " DRAM\n\n");

	//
	// Enable the RTC output
	//
	pm_write16 ( 0x56, pm_read16( 0x56 ) | (1 << 11));

	//
	// Enable power on from WAKE#
	//
	pm_write16 ( 0xBA, pm_read16( 0xBA ) | (1 << 14));

	if (acpi_is_wakeup_s3())
		agesawrapper_fchs3earlyrestore();

//
// SIO CONFIG, enable and disable UARTC and UARTD depending on the selection
//
#if CONFIG_SUPERIO_NUVOTON_NCT5104D_UARTC_ENABLE
	printk(BIOS_INFO, "UARTC enabled\n");

	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_SP3);
	if ( sio_dev ) sio_dev->enabled = 1;
	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_GPIO0);
	if ( sio_dev ) sio_dev->enabled = 0;

#else //CONFIG_SUPERIO_NUVOTON_NCT5104D_UARTC_ENABLE
	printk(BIOS_INFO, "UARTC disabled\n");

	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_SP3);
	if ( sio_dev ) sio_dev->enabled = 0;
	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_GPIO0);
	if ( sio_dev ) sio_dev->enabled = 1;
#endif //CONFIG_SUPERIO_NUVOTON_NCT5104D_UARTC_ENABLE


#if CONFIG_SUPERIO_NUVOTON_NCT5104D_UARTD_ENABLE
	printk(BIOS_INFO, "UARTD enabled\n");

	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_SP4);
	if ( sio_dev ) sio_dev->enabled = 1;
	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_GPIO1);
	if ( sio_dev ) sio_dev->enabled = 0;

#else //CONFIG_SUPERIO_NUVOTON_NCT5104D_UARTD_ENABLE
	printk(BIOS_INFO, "UARTD disabled\n");

	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_SP4);
	if ( sio_dev ) sio_dev->enabled = 0;
	sio_dev = dev_find_slot_pnp(0x2E, NCT5104D_GPIO1);
	if ( sio_dev ) sio_dev->enabled = 1;

#endif //CONFIG_SUPERIO_NUVOTON_NCT5104D_UARTD_ENABLE
}

static void mainboard_final(void *chip_info) {

	printk(BIOS_INFO, "Mainboard " CONFIG_MAINBOARD_PART_NUMBER "final\n");

/* Disabling LPCCLK0 which is unused according to the schematic doesn't work. The system is stuck if we do this
 * So we don't do this.

 */

#if CONFIG_DUMP_GPIO_CONFIGURATION
	//DumpGpioConfiguration( );
#endif //CONFIG_DUMP_GPIO_CONFIGURATION

#if CONFIG_DUMP_CLOCK_CONFIGURATION
	//DumpClockConfiguration( );
#endif //CONFIG_DUMP_GPIO_CONFIGURATION

#if CONFIG_DUMP_LINK_CONFIGURATION
	//DumpLinkConfiguration( );
#endif //CONFIG_DUMP_LINK_CONFIGURATION

	//
	// Turn off LED 2 and 3
	//
	printk(BIOS_INFO, "Turn off LED 2 and 3\n");
	WriteFchGpio( APU2_LED2_L_GPIO, 1);
	WriteFchGpio( APU2_LED3_L_GPIO, 1);

	printk(BIOS_INFO, "USB PORT ROUTING = 0x%08x\n", *((u8 *)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF )));
	if ( *((u8 *)(ACPI_MMIO_BASE + PMIO_BASE + FCH_PMIOA_REGEF )) & (1<<7) ) {

		printk(BIOS_INFO, "USB PORT ROUTING = XHCI PORTS ENABLED\n");
	} else {

		printk(BIOS_INFO, "USB PORT ROUTING = EHCI PORTS ENABLED\n");
	}
}

struct chip_operations mainboard_ops = {
	.enable_dev = mainboard_enable,
	.final = mainboard_final,
};
