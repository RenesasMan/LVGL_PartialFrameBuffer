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
#include "r_gpt.h"
#include "r_timer_api.h"
#include "r_qspi.h"
#include "r_spi_flash_api.h"
FSP_HEADER
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
