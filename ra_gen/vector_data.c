/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [1] = r_icu_isr, /* ICU IRQ10 (External pin interrupt 10) */
            [2] = r_icu_isr, /* ICU IRQ11 (External pin interrupt 11) */
            [3] = gpt_counter_overflow_isr, /* GPT1 COUNTER OVERFLOW (Overflow) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP0), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ10,GROUP1), /* ICU IRQ10 (External pin interrupt 10) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ11,GROUP2), /* ICU IRQ11 (External pin interrupt 11) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP3), /* GPT1 COUNTER OVERFLOW (Overflow) */
        };
        #endif
        #endif
