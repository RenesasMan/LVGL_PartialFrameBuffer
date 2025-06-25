/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
 * File Name    : ecbiu_drivers.c
 * Description  : Contains drivers for the ECBIU.
 **********************************************************************************************************************/

#include <ecbiu_drivers.h>

/* Configure the external bus for the Hello World Application. */
void ecbiu_lcd_config(void)
{
    R_BUS->CSb[0].CR_b.BSIZE = ECBIU_BUS_WIDTH_16;
    R_BUS->CSb[0].CR_b.EMODE = ECBIU_ENDIANNESS_LITTLE;
    R_BUS->CSb[0].CR_b.MPXEN = ECBIU_IO_INTERFACE_SEPARATE;
    R_BUS->CSa[0].MOD_b.WRMOD = ECBIU_WRITE_ACCESS_STROBE_MODE_SINGLE_WRITE;

    R_BUS->CSb[0].REC_b.RRCV = RECOVERY_CYCLES_3;       // Read recovery cycles: 3
    R_BUS->CSb[0].REC_b.WRCV = RECOVERY_CYCLES_3;       // Write recovery cycles: 3

    R_BUS->CSRECEN_b.RCVEN0 = RECOVERY_CYCLES_1;        // Recovery cycles specified by RRCV are inserted between cycles: read to read, in same area
    R_BUS->CSRECEN_b.RCVEN2 = RECOVERY_CYCLES_1;        // Recovery cycles specified by RRCV are inserted between cycles: read to write, in same area
    R_BUS->CSRECEN_b.RCVEN4 = RECOVERY_CYCLES_1;        // Recovery cycles specified by WRCV are inserted between cycles: write to read, in same area
    R_BUS->CSRECEN_b.RCVEN6 = RECOVERY_CYCLES_1;        // Recovery cycles specified by WRCV are inserted between cycles: write to write, in same area

    R_BUS->CSa[0].WCR1_b.CSWWAIT = RECOVERY_CYCLES_1;   // See timing diagram for value explanation
    R_BUS->CSa[0].WCR1_b.CSRWAIT = RECOVERY_CYCLES_5;
    R_BUS->CSa[0].WCR2_b.CSROFF = RECOVERY_CYCLES_7;
    R_BUS->CSa[0].WCR2_b.CSWOFF = RECOVERY_CYCLES_2;
    R_BUS->CSa[0].WCR2_b.WDOFF = RECOVERY_CYCLES_1;
}

