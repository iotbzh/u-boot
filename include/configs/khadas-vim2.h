/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Configuration for Khadas VIM2
 *
 * Copyright (C) 2017 Baylibre, SAS
 * Author: Neil Armstrong <narmstrong@baylibre.com>
 */

#ifndef __CONFIG_H
#define __CONFIG_H

#define CONFIG_MISC_INIT_R

#define MESON_FDTFILE_SETTING "fdtfile=amlogic/meson-gxm-khadas-vim2.dtb\0"

#include <configs/meson-gx-common.h>

#endif /* __CONFIG_H */