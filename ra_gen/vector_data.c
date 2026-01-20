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
            [4] = spi_rxi_isr, /* SPI0 RXI (Receive buffer full) */
            [5] = spi_txi_isr, /* SPI0 TXI (Transmit buffer empty) */
            [6] = spi_tei_isr, /* SPI0 TEI (Transmission complete event) */
            [7] = spi_eri_isr, /* SPI0 ERI (Error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP0), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ10,GROUP1), /* ICU IRQ10 (External pin interrupt 10) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ11,GROUP2), /* ICU IRQ11 (External pin interrupt 11) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP3), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_SPI0_RXI,GROUP4), /* SPI0 RXI (Receive buffer full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TXI,GROUP5), /* SPI0 TXI (Transmit buffer empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SPI0_TEI,GROUP6), /* SPI0 TEI (Transmission complete event) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SPI0_ERI,GROUP7), /* SPI0 ERI (Error) */
        };
        #endif
        #endif
