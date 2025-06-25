/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef ECBIU_DRIVERS_H_
#define ECBIU_DRIVERS_H_

#include "bsp_api.h"
#include "hal_data.h"

#define ECBIU_BUS_WIDTH_16  0
#define ECBIU_BUS_WIDTH_8   2

#define ECBIU_ENDIANNESS_LITTLE 0
#define ECBIU_ENDIANNESS_BIG    1

#define ECBIU_IO_INTERFACE_SEPARATE   0
#define ECBIU_IO_INTERFACE_MULTIPLEX  1

#define ECBIU_WRITE_ACCESS_STROBE_MODE_BYTE  0
#define ECBIU_WRITE_ACCESS_STROBE_MODE_SINGLE_WRITE  1

#define RECOVERY_CYCLES_0   0
#define RECOVERY_CYCLES_1   1
#define RECOVERY_CYCLES_2   2
#define RECOVERY_CYCLES_3   3
#define RECOVERY_CYCLES_4   4
#define RECOVERY_CYCLES_5   5
#define RECOVERY_CYCLES_6   6
#define RECOVERY_CYCLES_7   7
#define RECOVERY_CYCLES_8   8

void ecbiu_lcd_config(void);

#endif /* ECBIU_DRIVERS_H_ */
