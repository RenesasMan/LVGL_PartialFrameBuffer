/* generated common source file - do not edit */
#include "common_data.h"
icu_instance_ctrl_t g_external_irq1_ctrl;
const external_irq_cfg_t g_external_irq1_cfg =
{ .channel = 11, .trigger = EXTERNAL_IRQ_TRIG_RISING, .filter_enable = true, .clock_source_div =
          EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
  .p_callback = cb_pushbutton2,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ11)
    .irq                 = VECTOR_NUMBER_ICU_IRQ11,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq1 =
{ .p_ctrl = &g_external_irq1_ctrl, .p_cfg = &g_external_irq1_cfg, .p_api = &g_external_irq_on_icu };
icu_instance_ctrl_t g_external_irq0_ctrl;
const external_irq_cfg_t g_external_irq0_cfg =
{ .channel = 10, .trigger = EXTERNAL_IRQ_TRIG_RISING, .filter_enable = true, .clock_source_div =
          EXTERNAL_IRQ_CLOCK_SOURCE_DIV_64,
  .p_callback = cb_pushbutton1,
  /** If NULL then do not add & */
#if defined(NULL)
    .p_context           = NULL,
#else
  .p_context = &NULL,
#endif
  .p_extend = NULL,
  .ipl = (12),
#if defined(VECTOR_NUMBER_ICU_IRQ10)
    .irq                 = VECTOR_NUMBER_ICU_IRQ10,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
        };
/* Instance structure to use this module. */
const external_irq_instance_t g_external_irq0 =
{ .p_ctrl = &g_external_irq0_ctrl, .p_cfg = &g_external_irq0_cfg, .p_api = &g_external_irq_on_icu };
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };
SemaphoreHandle_t _SemaphoreVsync;
#if 1
StaticSemaphore_t _SemaphoreVsync_memory;
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
void g_common_init(void)
{
    _SemaphoreVsync =
#if 1
            xSemaphoreCreateBinaryStatic (&_SemaphoreVsync_memory);
#else
                xSemaphoreCreateBinary();
                #endif
    if (NULL == _SemaphoreVsync)
    {
        rtos_startup_err_callback (_SemaphoreVsync, 0);
    }
}
