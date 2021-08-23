// SPDX-License-Identifier: GPL-2.0+ OR BSD-3-Clause
/*
 * Copyright (C) 2019, STMicroelectronics - All Rights Reserved
 * Copyright (C) 2021, IoT.bzh - All Rights Reserved
 */
#define LOG_CATEGORY UCLASS_REMOTEPROC

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <fdtdec.h>
#include <log.h>
#include <remoteproc.h>
#include <reset.h>
#include <asm/io.h>
#include <dm/device_compat.h>
#include <linux/err.h>

#define PWRONCR7         0xE618024C

#define CR7BAR           0xE6160070

#define RST_CR7_SRCR2	 0xE61500B0
#define RST_CR7_SRSTCLR2 0xE6150948
#define RST_CR7_VAL      BIT(22)


static void rcar_copro_reset_assert(void)
{
	writel(RST_CR7_VAL, RST_CR7_SRCR2);
};

static void rcar_copro_reset_deassert(void)
{
	writel(RST_CR7_VAL, RST_CR7_SRSTCLR2);
};

/**

 * rcar_copro_probe() - Basic probe
 * @dev:	corresponding RCAR remote processor device
 * @return 0 if all went ok, else corresponding -ve error
 */
static int rcar_copro_probe(struct udevice *dev)
{
	rcar_copro_reset_assert();

	writel(0x40040010, CR7BAR);

	return 0;
}

/**
 * rcar_copro_load() - Loadup the RCAR remote processor
 * @dev:	corresponding RCAR remote processor device
 * @addr:	Address in memory where image is stored
 * @size:	Size in bytes of the image
 * @return 0 if all went ok, else corresponding -ve error
 */
static int rcar_copro_load(struct udevice *dev, ulong addr, ulong size)
{
	rcar_copro_reset_assert();

	return rproc_elf32_load_image(dev, addr, size);
}

/**
 * rcar_copro_start() - Start the RCAR remote processor
 * @dev:	corresponding RCAR remote processor device
 * @return 0 if all went ok, else corresponding -ve error
 */
static int rcar_copro_start(struct udevice *dev)
{
	rcar_copro_reset_deassert();

	return 0;
}

/**
 * rcar_copro_reset() - Reset the RCAR remote processor
 * @dev:	corresponding RCAR remote processor device
 * @return 0 if all went ok, else corresponding -ve error
 */
static int rcar_copro_reset(struct udevice *dev)
{
	rcar_copro_reset_deassert();

	return 0;
}

/**
 * rcar_copro_stop() - Stop the RCAR remote processor
 * @dev:	corresponding RCAR remote processor device
 * @return 0 if all went ok, else corresponding -ve error
 */
static int rcar_copro_stop(struct udevice *dev)
{
	return rcar_copro_reset(dev);
}

/**
 * rcar_copro_is_running() - Is the RCAR remote processor running
 * @dev:	corresponding RCAR remote processor device
 * @return 0 if the remote processor is running, 1 otherwise
 */
static int rcar_copro_is_running(struct udevice *dev)
{
	return (readl(RST_CR7_SRCR2) == RST_CR7_VAL);
}

static const struct dm_rproc_ops rcar_copro_ops = {
	.load = rcar_copro_load,
	.start = rcar_copro_start,
	.stop =  rcar_copro_stop,
	.reset = rcar_copro_reset,
	.is_running = rcar_copro_is_running,
};

static const struct udevice_id rcar_copro_ids[] = {
	{.compatible = "renesas,rcar-cr7"},
	{}
};

U_BOOT_DRIVER(rcar_copro) = {
	.name = "rcar_m4_proc",
	.of_match = rcar_copro_ids,
	.id = UCLASS_REMOTEPROC,
	.ops = &rcar_copro_ops,
	.probe = rcar_copro_probe,
};
