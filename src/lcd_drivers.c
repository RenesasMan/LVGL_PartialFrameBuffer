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
#include "LVGL_thread.h"
#include <stdint.h>

extern bool g_transfer_complete;

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


// For the display in SPI mode

/* LCD Driver Functions */
void LCD_Init_ST7789Vi_spi(void)
{
    volatile fsp_err_t err;
    uint8_t lcd_data[20];
    volatile uint8_t lcd_cmd[10]; //read display id.

    /* Reset the LCD, using GPIO to set the reset signal low for at least 100 ms */
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_SPI_RES, BSP_IO_LEVEL_HIGH );
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_SPI_RES, BSP_IO_LEVEL_LOW );
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_SPI_RES, BSP_IO_LEVEL_HIGH );
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);


    lcd_cmd[0] = NHD_LCD_CMD_MADCTL;
    lcd_data[0] = 0x00;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);


    /* Exit Sleep */
    lcd_cmd[0] = NHD_LCD_CMD_EXIT_SLEEP;
    command_spi(&lcd_cmd, 1);
    R_BSP_SoftwareDelay(100,BSP_DELAY_UNITS_MILLISECONDS);

    /* MADCTL: Memory Data Access Control */
    lcd_cmd[0] = NHD_LCD_CMD_MADCTL;
    lcd_data[0] = 0x00;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);


    /* COLMOD: Interface Pixel format *** 16 bit per pixel, 65k RGB */
    lcd_cmd[0] = NHD_LCD_CMD_COLMOD;
    lcd_data[0] = 0x05;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

//    /* INVON: Display Inversion ON (setting for IPS) */
//    lcd_cmd[0] = NHD_LCD_CMD_INVON;
//    command_spi(&lcd_cmd, 1);

    /* PORCTRK: Porch setting */
    lcd_cmd[0] = NHD_LCD_CMD_PORCTRK;
    lcd_data[0] = 0x0B;
    lcd_data[1] = 0x0B;
    lcd_data[2] = 0x00;
    lcd_data[3] = 0x33;
    lcd_data[4] = 0x35;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 5);

    /* GCTRL: Gate Control */
    lcd_cmd[0] = NHD_LCD_CMD_GCTRL;
    lcd_data[0] = 0x11;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* VCOMS: VCOM setting */
    lcd_cmd[0] = NHD_LCD_CMD_VCOMS;
    lcd_data[0] = 0x35;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* LCMCTRL: LCM Control */
    lcd_cmd[0] = NHD_LCD_CMD_LCMCTRL;
    lcd_data[0] = 0x2C;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* VDVVRHEN: VDV and VRH Command Enable */
    lcd_cmd[0] = NHD_LCD_CMD_VDVVRHEN;
    lcd_data[0] = 0x01;
//    lcd_data[1] = 0xFF;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* VRHS: VRH Set */
    lcd_cmd[0] = NHD_LCD_CMD_VRHS;
    lcd_data[0] = 0x0D;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* VDVS: VDV Set */
    lcd_cmd[0] = NHD_LCD_CMD_VDVS;
    lcd_data[0] = 0x20;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* FRCTRL2: Frame Rate control in normal mode */
    lcd_cmd[0] = NHD_LCD_CMD_FRCTRL2;
    lcd_data[0] = 0x13;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 1);

    /* PWCTRL1: Power Control 1 */
    lcd_cmd[0] = NHD_LCD_CMD_PWCTRL1;
    lcd_data[0] = 0xA4;
    lcd_data[1] = 0xA1;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 2);

    /* PVGAMCTRL: Positive Voltage Gamma control */
    lcd_cmd[0] = NHD_LCD_CMD_PVGAMCTRL;
    lcd_data[0] = 0xF0;
    lcd_data[1] = 0x06;
    lcd_data[2] = 0x0B;
    lcd_data[3] = 0x0A;
    lcd_data[4] = 0x09;
    lcd_data[5] = 0x26;
    lcd_data[6] = 0x29;
    lcd_data[7] = 0x33;
    lcd_data[8] = 0x41;
    lcd_data[9] = 0x18;
    lcd_data[10] = 0x16;
    lcd_data[11] = 0x15;
    lcd_data[12] = 0x29;
    lcd_data[13] = 0x2D;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 14);

    /* NVGAMCTRL: Negative Voltage Gamma control */
    lcd_cmd[0] = NHD_LCD_CMD_NVGAMCTRL;
    lcd_data[0] = 0xF0;
    lcd_data[1] = 0x04;
    lcd_data[2] = 0x08;
    lcd_data[3] = 0x08;
    lcd_data[4] = 0x07;
    lcd_data[5] = 0x03;
    lcd_data[6] = 0x28;
    lcd_data[7] = 0x32;
    lcd_data[8] = 0x40;
    lcd_data[9] = 0x3B;
    lcd_data[10] = 0x19;
    lcd_data[11] = 0x18;
    lcd_data[12] = 0x2A;
    lcd_data[13] = 0x2E;
    command_spi(&lcd_cmd, 1);
    data_spi(&lcd_data, 14);

//    /*X address set. 0-239 */
//    lcd_cmd[0] = NHD_LCD_CMD_X_ADDRESS;
//    lcd_data[0] = 0x00;
//    lcd_data[1] = 0x00;
//    lcd_data[2] = 0x00;
//    lcd_data[3] = 0xEF;
//    command_spi(&lcd_cmd, 1);
//    data_spi(&lcd_data, 1);

//    /* Y address set. 0-319 */
//    lcd_cmd[0] = NHD_LCD_CMD_Y_ADDRESS;
//    lcd_data[0] = 0x00;
//    lcd_data[1] = 0x00;
//    lcd_data[2] = 0x01;
//    lcd_data[3] = 0x3F;
//    command_spi(&lcd_cmd, 1);
//    data_spi(&lcd_data, 1);

        lcd_cmd[0] = 0xE4;
        lcd_data[0] = 0x25;
        lcd_data[1] = 0x00;
        lcd_data[2] = 0x00;
        command_spi(&lcd_cmd, 1);
        data_spi(&lcd_data, 3);

        lcd_cmd[0] = NHD_LCD_CMD_INVON;
        command_spi(&lcd_cmd, 1);

        /* Exit Sleep */
        lcd_cmd[0] = NHD_LCD_CMD_EXIT_SLEEP;
        command_spi(&lcd_cmd, 1);
        R_BSP_SoftwareDelay(120,BSP_DELAY_UNITS_MILLISECONDS);

        /* Display ON */
        lcd_cmd[0] = NHD_LCD_CMD_DISPLAY_ON;
        command_spi(&lcd_cmd, 1);

        LCD_Fill_spi(0xa5e6);

}

void LCD_Fill_spi(uint16_t pixel_color)
{
    volatile uint8_t lcd_cmd[10]; //read display id.
    uint8_t lcd_data[10];

    /* Memory Write */
    lcd_cmd[0] = NHD_LCD_CMD_MEM_WRITE;
    command_spi(&lcd_cmd, 1);

    for(int i=0; i< (DISPLAY_XRES * DISPLAY_YRES) ; ++i)
    {
        lcd_data[0] = pixel_color>>8;
        lcd_data[1] = pixel_color & 0xff;
        data_spi(&lcd_data, 2);
    }
    /* End Transfer */
    lcd_cmd[0] = NHD_LCD_CMD_END_TRANSFER;
    command_spi(&lcd_cmd, 1);
}

void LCD_Display_On_spi(void)
{
    /* Display On */
    LCD_CMD = NHD_LCD_CMD_DISPLAY_ON;
}

fsp_err_t command_spi(uint8_t * lcd_cmd, uint16_t data_length )
{
    fsp_err_t err;
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_SPI_CMD, BSP_IO_LEVEL_LOW );
    g_transfer_complete = false;
    err = R_SPI_Write(&g_spi0_ctrl, lcd_cmd, data_length,  SPI_BIT_WIDTH_8_BITS  );
    /* Wait for SPI_EVENT_TRANSFER_COMPLETE callback event. */
    while (false == g_transfer_complete)
    {
        __NOP();
    }
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_SPI_CMD, BSP_IO_LEVEL_LOW );
    return err;
}


fsp_err_t data_spi(uint8_t * lcd_data, uint16_t data_length )
{
    fsp_err_t err;
    R_IOPORT_PinWrite(&g_ioport_ctrl, LCD_SPI_CMD, BSP_IO_LEVEL_HIGH );
    g_transfer_complete = false;
    err = R_SPI_Write(&g_spi0_ctrl, lcd_data, data_length,  SPI_BIT_WIDTH_8_BITS  );
    /* Wait for SPI_EVENT_TRANSFER_COMPLETE callback event. */
    while (false == g_transfer_complete)
    {
        __NOP();
    }
    return err;
}
