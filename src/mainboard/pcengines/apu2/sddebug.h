void print_sd_gpio(unsigned int index);
void pull_down_sd_gpio(void);
void config_gpio_as_sd(void);

void print_sd_gpio(unsigned int index) {

	u32 data, *memptr;
    u8 datab, *memptrb;

	printk(BIOS_INFO, "index %d \n", index);

    memptrb = (u8 *)(ACPI_MMIO_BASE + PMIO_BASE + 0xE8);
    datab = *memptrb;
    printk(BIOS_ERR, "SDFlashCntrl = 0x%02x \n", datab);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x174);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x174 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x178);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x178 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x17C);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x17C 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x180);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x180 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x184);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x184 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x188);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x188 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x18C);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x18C 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x190);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x190 0x%08x \n", data);

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x194);
	data = *memptr;
	printk(BIOS_INFO, "GPIO_BANK0_BASE + 0x194 0x%08x \n", data);
}

void pull_down_sd_gpio(void) {

	u32 data, *memptr;
    u8 datab, *memptrb;

    print_sd_gpio(101);

    // configure SD pins as GPIO
    printk(BIOS_ERR, "configure SD pins as GPIO\n");
    memptrb = (u8 *)(ACPI_MMIO_BASE + PMIO_BASE + 0xE8);
    datab = *memptrb;
    datab &= ~(0x01);
    *memptrb = datab;

    print_sd_gpio(102);

    // pull down all sd GPIOs (except for LED)
    printk(BIOS_ERR, "pull down all SD pins\n");
	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x178);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x17C);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x180);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x184);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x188);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x18C);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x190);
	data = 0x00200800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x194);
	data = 0x00200800;
	*memptr = data;

    print_sd_gpio(103);
}

void config_gpio_as_sd(void) {

	u32 data, *memptr;
    u8 datab, *memptrb;

    print_sd_gpio(201);

    // pull up all SD GPIOs (except for LED)
    printk(BIOS_ERR, "pull up all SD BUS GPIOS\n");
	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x178);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x17C);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x180);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x184);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x188);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x18C);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x190);
	data = 0x00100800;
	*memptr = data;

	memptr = (u32 *)(ACPI_MMIO_BASE + GPIO_BANK0_BASE + 0x194);
	data = 0x00100800;
	*memptr = data;

    print_sd_gpio(202);

    // configure SD GPIO as SD BUS
    printk(BIOS_ERR, "configure SD pins as SD BUS\n");
    memptrb = (u8 *)(ACPI_MMIO_BASE + PMIO_BASE + 0xE8);
    datab = *memptrb;
    datab |= (0x01);
    *memptrb = datab;

    print_sd_gpio(203);
}

