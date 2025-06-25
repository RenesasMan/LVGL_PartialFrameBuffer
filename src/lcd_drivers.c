/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
 * File Name    : lcd_drivers.c
 * Description  : Contains drivers for the NHD LCD.
 **********************************************************************************************************************/

#include <lcd_drivers.h>

/* LCD Driver Functions */
void LCD_Init_ST7789Vi(void)
{
    /* Reset the LCD, using GPIO to set the reset signal low for at least 100 ms */
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_RES, BSP_IO_LEVEL_HIGH );
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_RES, BSP_IO_LEVEL_LOW );
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_RES, BSP_IO_LEVEL_HIGH );
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);


    /* You can use this code to verify the LCD turned on properly for LCD troubleshooting */
    volatile uint16_t data, id1, id2, id3, dummy = 0;
    LCD_CMD = 0x04;     //RDDID: Read Display ID
    dummy = LCD_DATA;
    id1 = LCD_DATA;
    id2 = LCD_DATA;
    id3 = LCD_DATA;

    if ((id1 != 0x85) || (id2 != 0x85) || (id3 != 0x52)) //default S/W Reset and Power On Sequence values
            __BKPT();



    /* Exit Sleep */
    LCD_CMD = NHD_LCD_CMD_EXIT_SLEEP;
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);

    /* MADCTL: Memory Data Access Control */
    LCD_CMD = NHD_LCD_CMD_MADCTL;
    LCD_DATA = 0x00;

    /* COLMOD: Interface Pixel format *** 16 bit per pixel, 65k RGB */
    LCD_CMD = NHD_LCD_CMD_COLMOD;
    LCD_DATA = 0x55;

    /* INVON: Display Inversion ON (setting for IPS) */
    LCD_CMD = NHD_LCD_CMD_INVON;

    /* PORCTRK: Porch setting */
    LCD_CMD = NHD_LCD_CMD_PORCTRK;
    LCD_DATA = 0x0C;
    LCD_DATA = 0x0C;
    LCD_DATA = 0x00;
    LCD_DATA = 0x33;
    LCD_DATA = 0x33;

    /* GCTRL: Gate Control */
    LCD_CMD = NHD_LCD_CMD_GCTRL;
    LCD_DATA = 0x35;

    /* VCOMS: VCOM setting */
    LCD_CMD = NHD_LCD_CMD_VCOMS;
    LCD_DATA = 0x2B;

    /* LCMCTRL: LCM Control */
    LCD_CMD = NHD_LCD_CMD_LCMCTRL;
    LCD_DATA = 0x2C;

    /* VDVVRHEN: VDV and VRH Command Enable */
    LCD_CMD = NHD_LCD_CMD_VDVVRHEN;
    LCD_DATA = 0x01;
    LCD_DATA = 0xFF;

    /* VRHS: VRH Set */
    LCD_CMD = NHD_LCD_CMD_VRHS;
    LCD_DATA = 0x11;

    /* VDVS: VDV Set */
    LCD_CMD = NHD_LCD_CMD_VDVS;
    LCD_DATA = 0x20;

    /* FRCTRL2: Frame Rate control in normal mode */
    LCD_CMD = NHD_LCD_CMD_FRCTRL2;
    LCD_DATA = 0x0F;

    /* PWCTRL1: Power Control 1 */
    LCD_CMD = NHD_LCD_CMD_PWCTRL1;
    LCD_DATA = 0xA4;
    LCD_DATA = 0xA1;

    /* PVGAMCTRL: Positive Voltage Gamma control */
    LCD_CMD = NHD_LCD_CMD_PVGAMCTRL;
    LCD_DATA = 0xD0;
    LCD_DATA = 0x00;
    LCD_DATA = 0x05;
    LCD_DATA = 0x0E;
    LCD_DATA = 0x15;
    LCD_DATA = 0x0D;
    LCD_DATA = 0x37;
    LCD_DATA = 0x43;
    LCD_DATA = 0x47;
    LCD_DATA = 0x09;
    LCD_DATA = 0x15;
    LCD_DATA = 0x12;
    LCD_DATA = 0x16;
    LCD_DATA = 0x19;

    /* NVGAMCTRL: Negative Voltage Gamma control */
    LCD_CMD = NHD_LCD_CMD_NVGAMCTRL;
    LCD_DATA = 0xD0;
    LCD_DATA = 0x00;
    LCD_DATA = 0x05;
    LCD_DATA = 0x0D;
    LCD_DATA = 0x0C;
    LCD_DATA = 0x06;
    LCD_DATA = 0x2D;
    LCD_DATA = 0x44;
    LCD_DATA = 0x40;
    LCD_DATA = 0x0E;
    LCD_DATA = 0x1C;
    LCD_DATA = 0x18;
    LCD_DATA = 0x16;
    LCD_DATA = 0x19;

    /*X address set. 0-239 */
    LCD_CMD = NHD_LCD_CMD_X_ADDRESS;
    LCD_DATA = 0x00;
    LCD_DATA = 0x00;
    LCD_DATA = 0x00;
    LCD_DATA = 0xEF;

    /* Y address set. 0-319 */
    LCD_CMD = NHD_LCD_CMD_Y_ADDRESS;
    LCD_DATA = 0x00;
    LCD_DATA = 0x00;
    LCD_DATA = 0x01;
    LCD_DATA = 0x3F;

}

void LCD_Fill(uint16_t pixel_color)
{
    /* Memory Write */
    LCD_CMD = NHD_LCD_CMD_MEM_WRITE;
    for(int i=0; i< (DISPLAY_XRES * DISPLAY_YRES) ; ++i)
    {
        LCD_DATA = pixel_color;

    }
    /* End Transfer */
    LCD_CMD = NHD_LCD_CMD_END_TRANSFER;
}

void LCD_Display_On(void)
{
    /* Display On */
    LCD_CMD = NHD_LCD_CMD_DISPLAY_ON;
}
