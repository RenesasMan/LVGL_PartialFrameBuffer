/*
* Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
/***********************************************************************************************************************
 * File Name    : dwt.c
 * Description  : Contains function implementations for using the DWT.c
 **********************************************************************************************************************/

#include "hal_data.h"
#include "time_counter.h"
#include "../LVGL_thread_entry.h"

// ############### USER SETTING ###############
#define TIMER_TYPE (1) // 0: CPU DWT, 1: Peripheral timer
// ############ END OF USER SETTING ###########

#if (TIMER_TYPE == 1)
static volatile uint32_t timer_count = 0;
volatile gpt_instance_ctrl_t * p_instance_ctrl;
#endif

volatile uint32_t timer_count_before, timer_count_reg_before, timer_count_after, timer_count_reg_after;


/*********************************************************************************************************************
 *  Enable the time counter
 *  @param[IN]   None
 *  @retval      None
***********************************************************************************************************************/
void TimeCounter_Init(void)
{
#if (TIMER_TYPE == 0)
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk |CoreDebug_DEMCR_MON_EN_Msk;
    DWT->CTRL |= (DWT_CTRL_CYCCNTENA_Msk << DWT_CTRL_CYCCNTENA_Pos);
#elif (TIMER_TYPE == 1)
    g_time_counter.p_api->open(g_time_counter.p_ctrl, g_time_counter.p_cfg);
    g_time_counter.p_api->start(g_time_counter.p_ctrl);
    timer_count = 0;

    p_instance_ctrl = (gpt_instance_ctrl_t *) g_time_counter.p_ctrl;
#endif
}

/*********************************************************************************************************************
 *  @brief       Stop the counter timer
 *  @param[IN]   None
 *  @retval      None
***********************************************************************************************************************/
void TimeCounter_Stop(void)
{
#if (TIMER_TYPE == 0)
    CoreDebug->DEMCR &= ~(CoreDebug_DEMCR_TRCENA_Msk |CoreDebug_DEMCR_MON_EN_Msk);
#elif (TIMER_TYPE == 1)
    g_time_counter.p_api->stop(g_time_counter.p_ctrl);
//    g_time_counter.p_api->close(g_time_counter.p_ctrl);
#endif
}

/*********************************************************************************************************************
 *  @brief       Start the counte timer
 *  @param[IN]   None
 *  @retval      None
***********************************************************************************************************************/
void TimeCounter_Start(void)
{
#if (TIMER_TYPE == 0)
    CoreDebug->DEMCR &= ~(CoreDebug_DEMCR_TRCENA_Msk |CoreDebug_DEMCR_MON_EN_Msk);
#elif (TIMER_TYPE == 1)
    g_time_counter.p_api->start(g_time_counter.p_ctrl);
//    g_time_counter.p_api->close(g_time_counter.p_ctrl);
#endif
}

/*********************************************************************************************************************
 *  @brief       Disable the counter timer
 *  @param[IN]   None
 *  @retval      None
***********************************************************************************************************************/
void TimeCounter_Disable(void)
{
#if (TIMER_TYPE == 0)
    CoreDebug->DEMCR &= ~(CoreDebug_DEMCR_TRCENA_Msk |CoreDebug_DEMCR_MON_EN_Msk);
#elif (TIMER_TYPE == 1)
    g_time_counter.p_api->stop(g_time_counter.p_ctrl);
    g_time_counter.p_api->close(g_time_counter.p_ctrl);
#endif
}

/*********************************************************************************************************************
 *  Reset the time counter
 *  @param[IN]   None
 *  @retval      None
***********************************************************************************************************************/
void TimeCounter_CountReset(void)
{
#if (TIMER_TYPE == 0)
    DWT->CYCCNT = 0;
#elif (TIMER_TYPE == 1)
    g_time_counter.p_api->stop(g_time_counter.p_ctrl);

    timer_count = 0;

    g_time_counter.p_api->reset(g_time_counter.p_ctrl);
    g_time_counter.p_api->start(g_time_counter.p_ctrl);
#endif
}

/*********************************************************************************************************************
 *  @brief       Get current cycle count
 *  @param[IN]   None
 *  @retval      None
***********************************************************************************************************************/
uint32_t TimeCounter_CurrentCountGet(void)
{
#if (TIMER_TYPE == 0)
    return DWT->CYCCNT;
#elif (TIMER_TYPE == 1)
    return timer_count;
#endif
}




/*********************************************************************************************************************
 *  @brief       Capture counter value from GCTNT for precise timing
 *  @param[IN]   None
 *  @retval      Count value from GCTNT
***********************************************************************************************************************/
uint32_t TimeCounter_CurrentCountRegGet(void)
{
    volatile uint32_t counter_reg_value = p_instance_ctrl->p_reg->GTCNT;

    return counter_reg_value;
}

/*********************************************************************************************************************
 *  @brief       Convert count value to ms
 *  @param[IN]   None
 *  @retval      Count value in milliseconds
***********************************************************************************************************************/
uint32_t TimeCounter_CountValueConvertToMs(uint32_t t1, uint32_t t2)
{
#if (TIMER_TYPE == 0)
#if BSP_FEATURE_CGC_HAS_CPUCLK
    return (t2 - t1) * 1000 / R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_CPUCLK);
#else
    return (t2 - t1) * 1000 / R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_ICLK);
#endif
#elif (TIMER_TYPE == 1)
    return (t2 - t1) / 10;
#endif
}

/*********************************************************************************************************************
 *  @brief       Convert count value in ms to fps
 *  @param[IN]   None
 *  @retval      Count value in fps
***********************************************************************************************************************/
uint32_t TimeCounter_ConvertFromMsToFps(uint32_t ms)
{
    return (uint32_t)(1000 / ms);
}

#if (TIMER_TYPE == 1)
void time_counter_callback(timer_callback_args_t *p_args)
{
    if(TIMER_EVENT_CYCLE_END == p_args->event)
    {
        timer_count++;
    }
}
#endif

void start_tick(void)
{
    //Prepare timer to measure the duration of rendering
    TimeCounter_Stop();
    TimeCounter_CountReset();
    timer_count_before = TimeCounter_CurrentCountGet();
    timer_count_reg_before = TimeCounter_CurrentCountRegGet();
}

void stop_tick(void)
{
    volatile uint32_t timer_ms_decimal;
    volatile uint32_t timer_ms;
    TimeCounter_Stop();
    timer_count_after = TimeCounter_CurrentCountGet();
    timer_count_reg_after = TimeCounter_CurrentCountRegGet();

    //combine 100ms inverval interrupt and counter register to get precise timing
    timer_ms_decimal = ((timer_count_after - timer_count_before)%10)*100 + ((timer_count_reg_after - timer_count_reg_before)*100)/g_time_counter.p_cfg->period_counts;
    timer_ms = (timer_count_after - timer_count_before)/10;

    APP_PRINT("Frame Buffer Render Time: %u.%u ms\tFPS:%u\r\n", timer_ms, timer_ms_decimal, TimeCounter_ConvertFromMsToFps(timer_ms) );
}
