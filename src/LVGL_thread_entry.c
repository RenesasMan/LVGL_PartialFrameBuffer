#include <LVGL_thread.h>
//#include "board_init.h"
#include "common_utils.h"
#include "lvgl/demos/lv_demos.h"
#include "LVGL_thread_entry.h"
#include "lvgl/src/drivers/display/renesas_glcdc/lv_renesas_glcdc.h"
#include "lcd_drivers.h"
#include "widgets_modified/lv_demo_widgets_modified.h"
#include "Partial_Frame_Buffer_Benchmark/Partial_Framebuffer_Benchmark.h"
#include "time_counter/time_counter.h"

#define DIRECT_MODE 1u
#define PARTIAL_MODE 2u
#define RENDER_MODE PARTIAL_MODE
//#define RENDER_MODE PARTIAL_MODE
#define PARTIAL_FRAMES_PER_FULL 8 //set the number of partial frames per a full frame

#define BYTES_PER_PIXEL 2
#define USE_FREE_RTOS (BSP_CFG_RTOS == 2)

static uint32_t idle_time_sum;
static uint32_t non_idle_time_sum;
static uint32_t task_switch_timestamp;
static bool idle_task_running;

#if ( (1 == LV_USE_DEMO_WIDGETS_MODIFIED || 1 == LV_USE_PARTIAL_FRAMEBUFFER_BENCHMARK) && 0 == LV_USE_DEMO_BENCHMARK)
extern lv_obj_t * tv;
extern lv_obj_t * t1;
extern lv_obj_t * t2;
extern lv_obj_t * t3;

extern lv_anim_t bars_anim_obj;
extern lv_anim_t vertical_ball0_anim_obj;
extern lv_anim_t horizontal_ball_anim_obj[10];

uint8_t current_tab = 0;
#endif
static bool is_sw1_pressed = false;
static bool is_sw2_pressed = false;

#if (DIRECT_MODE == RENDER_MODE)
uint16_t fb_background[DISPLAY_HSIZE_INPUT0 * DISPLAY_VSIZE_INPUT0] BSP_ALIGN_VARIABLE(64);
#elif (PARTIAL_MODE == RENDER_MODE)
uint16_t fb_background[(DISPLAY_HSIZE_INPUT0 * DISPLAY_VSIZE_INPUT0)/PARTIAL_FRAMES_PER_FULL] BSP_ALIGN_VARIABLE(64);
#endif

static void * rotation_buffer = NULL;
static uint32_t partial_buffer_size = 0;

lv_display_t * disp;

//uint32_t lv_os_get_idle_percent(void);

static void flush_direct(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);
static void flush_partial(lv_display_t * display, const lv_area_t * area, uint8_t * px_map);
static void flush_wait_direct(lv_display_t * display);
static void flush_wait_partial(lv_display_t * display);

void cb_pushbutton1(external_irq_callback_args_t *p_args);
void cb_pushbutton2(external_irq_callback_args_t *p_args);
fsp_err_t ext_irq_init(void);
fsp_err_t qspi_init(void);

void fb_color_bars(void);
void lcd_refresh_565rgb(void);
void lcd_refresh_565rgb_partial(const lv_area_t * area, uint8_t *px);
void lv_freertos_task_switch_in(const char * name)
{
    if(strcmp(name, "IDLE")) idle_task_running = false;
    else idle_task_running = true;

    task_switch_timestamp = lv_tick_get();
}

void lv_freertos_task_switch_out(void)
{
    uint32_t elaps = lv_tick_elaps(task_switch_timestamp);
    if(idle_task_running) idle_time_sum += elaps;
    else non_idle_time_sum += elaps;
}

//uint32_t lv_os_get_idle_percent(void)
//{
//    if(non_idle_time_sum + idle_time_sum == 0) {
//        LV_LOG_WARN("Not enough time elapsed to provide idle percentage");
//        return 0;
//    }
//
//    uint32_t pct = (idle_time_sum * 100) / (idle_time_sum + non_idle_time_sum);
//
//    non_idle_time_sum = 0;
//    idle_time_sum = 0;
//
//    return pct;
//}


void timer_tick_callback(timer_callback_args_t *p_args)
{
    FSP_PARAMETER_NOT_USED(p_args);
    lv_tick_inc(1);
}

void vApplicationMallocFailedHook( void )
{
    __BKPT(0);
}

/* New Thread entry function */
/* pvParameters contains TaskHandle_t */
void LVGL_thread_entry(void *pvParameters)
{
    FSP_PARAMETER_NOT_USED (pvParameters);
    fsp_err_t err;

    fsp_pack_version_t version    = {RESET_VALUE};

    /* version get API for FLEX pack information */
    R_FSP_VersionGet(&version);

//    APP_PRINT(BANNER_INFO, EP_VERSION, version.version_id_b.major, version.version_id_b.minor, version.version_id_b.patch);
//    APP_PRINT(EP_INFO);

    TimeCounter_Init();

    err = ext_irq_init();
    if (FSP_SUCCESS != err)
    {
        __BKPT();
    }

    err = qspi_init();
    if (FSP_SUCCESS != err)
    {
        __BKPT();
    }

    /* Setup the external bus transfers. */
    ecbiu_lcd_config();

    /* Initialize LCD, fill screen with color bars, and turn on the display. */
    LCD_Init_ST7789Vi();
//    fb_color_bars();
    LCD_Display_On();

    lv_init();

#if (DIRECT_MODE == RENDER_MODE)

    //Based on the GLCDC implementation, has 2 buffers
    lv_memzero(fb_background, sizeof(fb_background));

    disp = lv_display_create(DISPLAY_HSIZE_INPUT0, DISPLAY_VSIZE_INPUT0);

    // Direct render
    lv_display_set_flush_cb(disp, flush_direct);
    lv_display_set_flush_wait_cb(disp, flush_wait_direct);

    lv_display_set_buffers(disp, &fb_background[0], NULL, sizeof(fb_background), LV_DISPLAY_RENDER_MODE_DIRECT);

#else if(PARTIAL_MODE == RENDER_MODE)

    //Based on the GLCDC implementation, has 2 buffers
    lv_memzero(fb_background, sizeof(fb_background));

    disp = lv_display_create(DISPLAY_HSIZE_INPUT0, DISPLAY_VSIZE_INPUT0);

    // Partial render
    lv_display_set_flush_cb(disp, flush_partial);
    lv_display_set_flush_wait_cb(disp, flush_wait_partial);

//@@    lv_display_set_buffers(disp, &fb_background[0], NULL, sizeof(fb_background), LV_DISPLAY_RENDER_MODE_DIRECT);
    lv_display_set_buffers(disp, &fb_background[0], NULL, sizeof(fb_background), LV_DISPLAY_RENDER_MODE_PARTIAL);
#endif

    lv_display_set_default(disp);


#if (1 == LV_USE_DEMO_BENCHMARK)
    lv_demo_benchmark();
#endif

#if (1 == LV_USE_DEMO_MUSIC)
    lv_demo_music();
#endif

#if (1 == LV_USE_DEMO_KEYPAD_AND_ENCODER)
    lv_demo_keypad_encoder();

#endif

#if (1 == LV_USE_DEMO_STRESS)
    lv_demo_stress();
#endif

#if (1 == LV_USE_DEMO_WIDGETS && 0 == LV_USE_DEMO_BENCHMARK)
    lv_demo_widgets();
#endif


#if (1 == LV_USE_DEMO_WIDGETS_MODIFIED && 0 == LV_USE_DEMO_BENCHMARK)
    lv_demo_widgets_modified();
#endif

#if (1 == LV_USE_HELLO_WORLD)
//    lv_example_get_started_1();
    lv_example_get_started_2();
#endif

#if (1 == LV_USE_PARTIAL_FRAMEBUFFER_BENCHMARK)
    lv_demo_partial_framebuffer_benchmark();
#endif

    err = R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    err = R_GPT_Start(&g_timer0_ctrl);
    if (FSP_SUCCESS != err)
    {
        __BKPT(0);
    }

    start_tick();
    vTaskDelay (1);
    stop_tick();

    start_tick();
    vTaskDelay (10);
    stop_tick();

    start_tick();
    vTaskDelay (1000);
    stop_tick();

    lv_timer_handler();

    uint8_t old_tab;

    /* TODO: add your own code here */
    while (1)
    {
        lv_timer_handler();
        vTaskDelay (1);

#if ( (1 == LV_USE_DEMO_WIDGETS_MODIFIED || 1 == LV_USE_PARTIAL_FRAMEBUFFER_BENCHMARK) && 0 == LV_USE_DEMO_BENCHMARK)
        if( true == is_sw1_pressed )
        {
            is_sw1_pressed = false;
            is_sw2_pressed = false; // just in case both switches were pressed, ignore sw2 by resetting
            old_tab = current_tab;

            if(0U == current_tab)
            {
                APP_PRINT("\r\nBar Graph Animation Start:\r\n");
                lv_anim_start(&bars_anim_obj);
            }
            else if( 1U == current_tab)
            {
                APP_PRINT("\r\nVertical Ball Animation Start:\r\n");
                lv_anim_start(&vertical_ball0_anim_obj);
            }
            else if(2U == current_tab)
            {
                APP_PRINT("\r\nHorizontal Ball Array Animation Start:\r\n");
                lv_anim_start(&horizontal_ball_anim_obj[0]);
                lv_anim_start(&horizontal_ball_anim_obj[1]);
                lv_anim_start(&horizontal_ball_anim_obj[2]);
                lv_anim_start(&horizontal_ball_anim_obj[3]);
                lv_anim_start(&horizontal_ball_anim_obj[4]);
                lv_anim_start(&horizontal_ball_anim_obj[5]);
                lv_anim_start(&horizontal_ball_anim_obj[6]);
                lv_anim_start(&horizontal_ball_anim_obj[7]);
                lv_anim_start(&horizontal_ball_anim_obj[8]);
                lv_anim_start(&horizontal_ball_anim_obj[9]);
            }

        }
        if (true == is_sw2_pressed )
        {
            is_sw2_pressed = false;

            if(2U > current_tab )
            {
                current_tab++;
            }
            else
            {
                current_tab = 0U;
            }
            APP_PRINT("\r\nTab Transition (%u to %u) Animation Start:\r\n", old_tab, current_tab);
            lv_tabview_set_active(tv, current_tab, LV_ANIM_ON);
        }

#endif

    }
}


static void flush_direct(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    FSP_PARAMETER_NOT_USED(area);

    /*Display the frame buffer pointed by px_map*/
    lcd_refresh_565rgb();
    if(!lv_display_flush_is_last(display)) return;

    //Added for EcoWater
    lv_display_flush_ready(display);

#if defined(RENESAS_CORTEX_M85) && (BSP_CFG_DCACHE_ENABLED)
    /* Invalidate cache - so the HW can access any data written by the CPU */
    SCB_CleanInvalidateDCache_by_Addr(px_map, sizeof(fb_background[0]));
#endif

#ifdef _RENESAS_RA_
//    R_GLCDC_BufferChange(&g_display0_ctrl,
//                         (uint8_t *) px_map,
//                         (display_frame_layer_t) 0);
#else /* RX */
    glcdc_err_t err;

    g_layer_change.input.p_base = (uint32_t *)px_map;

    do {
        err = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_2, &g_layer_change);
    } while(GLCDC_ERR_INVALID_UPDATE_TIMING == err);
#endif /*_RENESAS_RA_*/
}

static void flush_wait_direct(lv_display_t * display)
{
    if(!lv_display_flush_is_last(display)) return;

#if USE_FREE_RTOS
    /*If Vsync semaphore has already been set, clear it then wait to avoid tearing*/
    if(uxSemaphoreGetCount(_SemaphoreVsync)) {
        xSemaphoreTake(_SemaphoreVsync, 10);
    }

    xSemaphoreTake(_SemaphoreVsync, portMAX_DELAY);
#endif /*USE_FREE_RTOS*/

}

static void flush_partial(lv_display_t * display, const lv_area_t * area, uint8_t * px_map)
{
    //FSP_PARAMETER_NOT_USED(area);
    stop_tick();
    uint32_t draw_area_size = (area->x2 - area->x1 )*(area->y2 - area->y1);
    APP_PRINT("\r\nRegion %u\t x1=%u\tx2=%u\ty1=%u\ty2=%u\tsize=%u\r\n", (area->y1)/(DISPLAY_VSIZE_INPUT0/PARTIAL_FRAMES_PER_FULL), area->x1, area->x2, area->y1, area->y2, draw_area_size);

    /*Display the frame buffer pointed by px_map*/
    lcd_refresh_565rgb_partial(area,px_map);
    if(!lv_display_flush_is_last(display)) return;

    //Added for EcoWater
    lv_display_flush_ready(display);

#if defined(RENESAS_CORTEX_M85) && (BSP_CFG_DCACHE_ENABLED)
    /* Invalidate cache - so the HW can access any data written by the CPU */
    SCB_CleanInvalidateDCache_by_Addr(px_map, sizeof(fb_background[0]));
#endif

#ifdef _RENESAS_RA_
//    R_GLCDC_BufferChange(&g_display0_ctrl,
//                         (uint8_t *) px_map,
//                         (display_frame_layer_t) 0);
#else /* RX */
    glcdc_err_t err;

    g_layer_change.input.p_base = (uint32_t *)px_map;

    do {
        err = R_GLCDC_LayerChange(GLCDC_FRAME_LAYER_2, &g_layer_change);
    } while(GLCDC_ERR_INVALID_UPDATE_TIMING == err);
#endif /*_RENESAS_RA_*/
    start_tick();
}

static void flush_wait_partial(lv_display_t * display)
{
    if(!lv_display_flush_is_last(display)) return;

#if USE_FREE_RTOS
    /*If Vsync semaphore has already been set, clear it then wait to avoid tearing*/
    if(uxSemaphoreGetCount(_SemaphoreVsync)) {
        xSemaphoreTake(_SemaphoreVsync, 10);
    }

    xSemaphoreTake(_SemaphoreVsync, portMAX_DELAY);
#endif /*USE_FREE_RTOS*/
}


/* Function fills the LCD with colored bars. */
void fb_color_bars(void)
{
    uint16_t color_bar[] = { BLACK_565RGB, RED_565RGB , GREEN_565RGB, BLUE_565RGB,
                          CYAN_565RGB, MAGENTA_565RGB, YELLOW_565RGB, WHITE_565RGB};
    int num_cb = 8;
    int bar_len = (DISPLAY_XRES * DISPLAY_YRES)/num_cb;

    LCD_CMD = NHD_LCD_CMD_MEM_WRITE;

    /* Send the color bar pixel data to fill the LCD */
    for(int i=0; i < num_cb; ++i)
    {
        for(int j=0; j < bar_len; ++j)
        {
            LCD_DATA = color_bar[i];
        }
    }

    LCD_CMD = NHD_LCD_CMD_END_TRANSFER;

}


/* Function toggles screen by writing the framebuffer pixels on the LCD. */
void lcd_refresh_565rgb()
{

    LCD_CMD = NHD_LCD_CMD_MEM_WRITE;

    /* Send all pixels in the FrameBuffer */
    for(volatile int i=0; i< (DISPLAY_XRES * DISPLAY_YRES) ; i++)
    {
        LCD_DATA = (uint16_t) fb_background[i];
    }

    LCD_CMD = NHD_LCD_CMD_END_TRANSFER;

    lv_display_flush_ready(disp);
}
volatile lv_area_t *pony;
void lcd_refresh_565rgb_partial(const lv_area_t * area, uint8_t *px)
{
    volatile int row,col;
    pony = area;
    { //@@ LCD set area
        LCD_CMD = NHD_LCD_CMD_X_ADDRESS;
        LCD_DATA = (area->x1 & 0x0000FF00) >> 8;
        LCD_DATA = (area->x1 & 0x000000FF);
        LCD_DATA = (area->x2 & 0x0000FF00) >> 8;
        LCD_DATA = (area->x2 & 0x000000FF);
        LCD_CMD = NHD_LCD_CMD_Y_ADDRESS;
        LCD_DATA = (area->y1 & 0x0000FF00) >> 8;
        LCD_DATA = (area->y1 & 0x000000FF);
        LCD_DATA = (area->y2 & 0x0000FF00) >> 8;
        LCD_DATA = (area->y2 & 0x000000FF);

    }
    LCD_CMD = NHD_LCD_CMD_MEM_WRITE;
    /* Send partial pixels in the FrameBuffer */
#if 0 //@@ This for loop indexes into a full-screen buffer
    for(row=area->y1;row<=area->y2;row++)
        for(col=area->x1;col<=area->x2;col++)
        {
//            LCD_DATA = (uint16_t) fb_background[(row*(DISPLAY_XRES))+col];
            LCD_DATA = (uint16_t) ((uint16_t*)px)[(row*(DISPLAY_XRES))+col];
        }
#else //@@ this one bases at 0 0
    for(row=0;row<=(area->y2-area->y1);row++)
        for(col=0;col<=(area->x2-area->x1);col++)
            LCD_DATA = (uint16_t) ((uint16_t*)px)[(row*(area->x2-area->x1+1))+col];
#endif
    LCD_CMD = NHD_LCD_CMD_END_TRANSFER;
    lv_display_flush_ready(disp);
}
void cb_pushbutton1(external_irq_callback_args_t *p_args)
{

    is_sw1_pressed = true;

}


void cb_pushbutton2(external_irq_callback_args_t *p_args)
{

    is_sw2_pressed = true;

}

fsp_err_t ext_irq_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    err = R_ICU_ExternalIrqOpen(&g_external_irq0_ctrl, &g_external_irq0_cfg);
      if(FSP_SUCCESS != err)
      {
          __BKPT();

      }

      err = R_ICU_ExternalIrqEnable(&g_external_irq0_ctrl);
      if(FSP_SUCCESS != err)
      {
          __BKPT();

      }

      err = R_ICU_ExternalIrqOpen(&g_external_irq1_ctrl, &g_external_irq1_cfg);
      if(FSP_SUCCESS != err)
      {
          __BKPT();

      }

      err = R_ICU_ExternalIrqEnable(&g_external_irq1_ctrl);
      if(FSP_SUCCESS != err)
      {
          __BKPT();

      }

    return err;
}

fsp_err_t qspi_init(void)
{
    fsp_err_t err = FSP_SUCCESS;

    /* open QSPI in extended SPI mode */
        err = R_QSPI_Open(&g_qspi_ctrl, &g_qspi_cfg);
        if (FSP_SUCCESS != err)
        {
            __BKPT();
        }

        /* write enable for further operations */
        err = R_QSPI_DirectWrite(&g_qspi_ctrl, &(g_qspi_cfg.write_enable_command), 0x01, false);
        if (FSP_SUCCESS != err)
        {
    //        APP_ERR_PRINT("R_QSPI_DirectWrite Failed\r\n");
            /* close QSPI module which is currently in extended SPI mode only */
    //        deinit_qspi(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
            __BKPT();
        }
        else
        {
    //        err = get_flash_status();
            if (FSP_SUCCESS != err)
            {
    //            APP_ERR_PRINT("Failed to get status for QSPI operation\r\n");
                /* close QSPI module which is currently in extended SPI mode only */
    //            deinit_qspi(SPI_FLASH_PROTOCOL_EXTENDED_SPI);
                __BKPT();
            }
        }

        /*
             * write QSPI flash status register
             * This is required to make sure the device is ready for general
             * read write operation,
             * This performs settings such as physical reset,WP hardware pin disable,
             * block protection lock bits clearing.
             * for more details please refer Mx25L data sheet.
             */
        uint8_t   data_sreg[0x03]                 = {0x01,0x40,0x00};
        err = R_QSPI_DirectWrite(&g_qspi_ctrl, data_sreg, 0x03, false);
        if (FSP_SUCCESS != err)
        {
            __BKPT();
        }
        else
        {
            if (FSP_SUCCESS != err)
            {
                __BKPT();
            }
        }

    return err;
}
