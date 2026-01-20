/* generated thread header file - do not edit */
#ifndef LVGL_THREAD_H_
#define LVGL_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void LVGL_thread_entry(void * pvParameters);
                #else
extern void LVGL_thread_entry(void *pvParameters);
#endif
#include "r_spi.h"
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_qspi.h"
#include "r_spi_flash_api.h"
FSP_HEADER
/** SPI on SPI Instance. */
extern const spi_instance_t g_spi0;

/** Access the SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern spi_instance_ctrl_t g_spi0_ctrl;
extern const spi_cfg_t g_spi0_cfg;

/** Callback used by SPI Instance. */
#ifndef spi_callback
void spi_callback(spi_callback_args_t *p_args);
#endif

#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#define g_spi0_P_TRANSFER_TX (NULL)
#else
    #define g_spi0_P_TRANSFER_TX (&RA_NOT_DEFINED)
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
#define g_spi0_P_TRANSFER_RX (NULL)
#else
    #define g_spi0_P_TRANSFER_RX (&RA_NOT_DEFINED)
#endif
#undef RA_NOT_DEFINED
/** Timer on GPT Instance. */
extern const timer_instance_t g_time_counter;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_time_counter_ctrl;
extern const timer_cfg_t g_time_counter_cfg;

#ifndef time_counter_callback
void time_counter_callback(timer_callback_args_t *p_args);
#endif
extern const spi_flash_instance_t g_qspi;
extern qspi_instance_ctrl_t g_qspi_ctrl;
extern const spi_flash_cfg_t g_qspi_cfg;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer0;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer0_ctrl;
extern const timer_cfg_t g_timer0_cfg;

#ifndef timer_tick_callback
void timer_tick_callback(timer_callback_args_t *p_args);
#endif
FSP_FOOTER
#endif /* LVGL_THREAD_H_ */
