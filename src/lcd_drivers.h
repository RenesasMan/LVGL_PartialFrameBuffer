/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef LCD_DRIVERS_H_
#define LCD_DRIVERS_H_

#include "ecbiu_drivers.h"

//reset is P303
//DC is P611
//BL is P304

#define LCD_SPI_RES (BSP_IO_PORT_03_PIN_03)
#define LCD_SPI_CMD (BSP_IO_PORT_06_PIN_11)
#define LCD_SPI_BL (BSP_IO_PORT_03_PIN_04)

#define LCD_CMD (*( volatile uint8_t *) 0x80000000)
#define LCD_DATA (*( volatile uint16_t *) 0x80000004)

#ifndef LCD_RES
#define LCD_RES     BSP_IO_PORT_01_PIN_13
#endif
#ifndef LCD_IM0
#define LCD_IM0     BSP_IO_PORT_01_PIN_15   //set imo = 0 for 16bit
#endif

#define DISPLAY_XRES    240
#define DISPLAY_YRES    320
#define FRAME_BUFFER_SIZE   (DISPLAY_XRES * DISPLAY_YRES * 2)

#define RED_565RGB      0xf800
#define GREEN_565RGB    0x07e0
#define BLUE_565RGB     0x001f
#define YELLOW_565RGB   0xffe0
#define CYAN_565RGB     0x07ff
#define MAGENTA_565RGB  0xf81f
#define BLACK_565RGB    0x0000
#define WHITE_565RGB    0xffff

#define NHD_LCD_CMD_EXIT_SLEEP  0x11
#define NHD_LCD_CMD_MADCTL      0x36
#define NHD_LCD_CMD_COLMOD      0x3A
#define NHD_LCD_CMD_INVON       0x21
#define NHD_LCD_CMD_PORCTRK     0xB2
#define NHD_LCD_CMD_GCTRL       0xB7
#define NHD_LCD_CMD_VCOMS       0xBB
#define NHD_LCD_CMD_LCMCTRL     0xC0
#define NHD_LCD_CMD_VDVVRHEN    0xC2
#define NHD_LCD_CMD_VRHS        0xC3
#define NHD_LCD_CMD_VDVS        0xC4
#define NHD_LCD_CMD_FRCTRL2     0xC6
#define NHD_LCD_CMD_PWCTRL1     0xD0
#define NHD_LCD_CMD_PVGAMCTRL   0xE0
#define NHD_LCD_CMD_NVGAMCTRL   0xE1
#define NHD_LCD_CMD_X_ADDRESS   0x2A
#define NHD_LCD_CMD_Y_ADDRESS   0x2B

#define NHD_LCD_CMD_MEM_WRITE       0x2C
#define NHD_LCD_CMD_END_TRANSFER    0x00
#define NHD_LCD_CMD_DISPLAY_ON      0x29

void LCD_Init_ST7789Vi(void);
void LCD_Fill(uint16_t pixel_color);
void LCD_Display_On(void);

void LCD_Init_ST7789Vi_spi(void);
void LCD_Fill_spi(uint16_t pixel_color);
void LCD_Display_On_spi(void);
fsp_err_t command_spi(uint8_t * lcd_cmd, uint16_t data_length );
fsp_err_t data_spi(uint8_t * lcd_data, uint16_t data_length );

#endif /* LCD_DRIVERS_H_ */
