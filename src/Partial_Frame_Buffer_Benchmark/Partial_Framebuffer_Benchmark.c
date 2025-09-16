/*
 * Partial_Framebuffer_Benchmark.c
 *
 *  Created on: Aug 5, 2025
 *      Author: a5138119
 */

#include "Partial_Framebuffer_Benchmark.h"
#include "../lvgl/lvgl_private.h"
#include "common_utils.h"


#if LV_USE_STDLIB_MALLOC == LV_STDLIB_BUILTIN && LV_MEM_SIZE < (38ul * 1024ul)
    #error Insufficient memory for lv_demo_widgets. Please set LV_MEM_SIZE to at least 38KB (38ul * 1024ul).  48KB is recommended.
#endif

/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
} disp_size_t;

lv_obj_t * tv;
lv_obj_t * t1;
lv_obj_t * t2;
lv_obj_t * t3;

lv_anim_t bars_anim_obj;
lv_anim_t vertical_ball0_anim_obj;
lv_anim_t horizontal_ball_anim_obj[10];


static lv_obj_t * calendar;
static lv_style_t style_text_muted;
static lv_style_t style_title;
static lv_style_t style_icon;
static lv_style_t style_bullet;

static disp_size_t disp_size;

static const lv_font_t * font_large;
static const lv_font_t * font_normal;


static void bars_create(lv_obj_t * parent);
static void ball_create(lv_obj_t * parent);
static void popup_create(lv_obj_t * parent);

static void tabview_delete_event_cb(lv_event_t * e);



void lv_demo_partial_framebuffer_benchmark(void)
{

    font_large = LV_FONT_DEFAULT;
    font_normal = LV_FONT_DEFAULT;

    int32_t tab_h;
    if(disp_size == DISP_LARGE) {
        tab_h = 70;
#if LV_FONT_MONTSERRAT_24
        font_large     = &lv_font_montserrat_24;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_24 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_16
        font_normal    = &lv_font_montserrat_16;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_16 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
    else if(disp_size == DISP_MEDIUM) {
        tab_h = 45;
#if LV_FONT_MONTSERRAT_20
        font_large     = &lv_font_montserrat_20;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_20 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_14
        font_normal    = &lv_font_montserrat_14;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_14 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }
    else {   /* disp_size == DISP_SMALL */
        tab_h = 45;
#if LV_FONT_MONTSERRAT_18
        font_large     = &lv_font_montserrat_18;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_18 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
#if LV_FONT_MONTSERRAT_12
        font_normal    = &lv_font_montserrat_12;
#else
        LV_LOG_WARN("LV_FONT_MONTSERRAT_12 is not enabled for the widgets demo. Using LV_FONT_DEFAULT instead.");
#endif
    }

#if LV_USE_THEME_DEFAULT
    lv_theme_default_init(NULL, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), LV_THEME_DEFAULT_DARK,
                          font_normal);
#endif

    lv_style_init(&style_text_muted);
    lv_style_set_text_opa(&style_text_muted, LV_OPA_50);

    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);

    lv_style_init(&style_icon);
    lv_style_set_text_color(&style_icon, lv_theme_get_color_primary(NULL));
    lv_style_set_text_font(&style_icon, font_large);

    lv_style_init(&style_bullet);
    lv_style_set_border_width(&style_bullet, 0);
    lv_style_set_radius(&style_bullet, LV_RADIUS_CIRCLE);

    tv = lv_tabview_create(lv_screen_active());
    lv_tabview_set_tab_bar_size(tv, tab_h);
    lv_obj_add_event_cb(tv, tabview_delete_event_cb, LV_EVENT_DELETE, NULL);

    lv_obj_set_style_text_font(lv_screen_active(), font_normal, 0);

    // Create tabs, each tab has a different test
        //test 1 is different sized bars
        //test 2 is a bouncing ball
        //test 3 is a rapid swipe

    t1 = lv_tabview_add_tab(tv, "Bars");
    t2 = lv_tabview_add_tab(tv, "Ball");
    t3 = lv_tabview_add_tab(tv, "Popups");

    bars_create(t1);
    ball_create(t2);
    popup_create(t3);

    return;
}

static void anim_x_cb(void * var, int32_t v)
{
    lv_obj_set_x(var, v);
}

static void anim_y_cb(void * var, int32_t v)
{
    lv_obj_set_y(var, v);
}

static void set_temp(void * bar, int32_t temp)
{
    lv_bar_set_value(bar, temp, LV_ANIM_ON);
}

static void anim_report_cb(void * var, int32_t v)
{
    APP_PRINT("\r\nAnimation Complete\r\n");
}


static void bars_create(lv_obj_t * parent)
{
    static lv_style_t style_indic, style_main;

    lv_style_init(&style_indic);
    lv_style_set_bg_opa(&style_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&style_indic, lv_palette_main(LV_PALETTE_BLUE));

    lv_obj_t * bar = lv_bar_create(parent);
    lv_obj_add_style(bar, &style_indic, LV_PART_INDICATOR);
    lv_obj_set_size(bar, 20, 250);
    lv_obj_center(bar);
    lv_bar_set_range(bar, -20, 40);

//    lv_anim_t bars_anim_obj;
    lv_anim_init(&bars_anim_obj);
    lv_anim_set_exec_cb(&bars_anim_obj, set_temp);
    lv_anim_set_duration(&bars_anim_obj, 3000);
    lv_anim_set_reverse_duration(&bars_anim_obj, 3000);
    lv_anim_set_var(&bars_anim_obj, bar);
    lv_anim_set_values(&bars_anim_obj, -20, 40);
    lv_anim_set_repeat_count(&bars_anim_obj, ANIMATION_COUNT);
    lv_anim_set_completed_cb(&bars_anim_obj, anim_report_cb);
//    lv_anim_start(&bars_anim_obj);


    return;
}


static void ball_animate(lv_obj_t * obj, lv_anim_t * anim)
{
    lv_anim_init(anim);
    lv_anim_set_var(anim, obj);
    lv_anim_set_duration(anim, 1000);
    lv_anim_set_reverse_delay(anim, 100);
    lv_anim_set_reverse_duration(anim, 300);
    lv_anim_set_repeat_delay(anim, 500);
    lv_anim_set_repeat_count(anim, ANIMATION_COUNT);
    lv_anim_set_exec_cb(anim, anim_y_cb);
    lv_anim_set_completed_cb(anim, anim_report_cb);
    lv_anim_set_values(anim, -100, 100);
//    lv_anim_start(anim);
}


static void ball_create(lv_obj_t * parent)
{
    lv_obj_t * ball0 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball0, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(ball0, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball0, 20, 20);
    lv_obj_center(ball0);

//    lv_anim_t vertical_ball0_anim_obj;
    lv_anim_init(&vertical_ball0_anim_obj);
    lv_anim_set_var(&vertical_ball0_anim_obj, ball0);
    lv_anim_set_duration(&vertical_ball0_anim_obj, 1000);
    lv_anim_set_reverse_delay(&vertical_ball0_anim_obj, 100);
    lv_anim_set_reverse_duration(&vertical_ball0_anim_obj, 300);
    lv_anim_set_repeat_delay(&vertical_ball0_anim_obj, 500);
    lv_anim_set_repeat_count(&vertical_ball0_anim_obj, ANIMATION_COUNT);
    lv_anim_set_exec_cb(&vertical_ball0_anim_obj, anim_y_cb);
    lv_anim_set_completed_cb(&vertical_ball0_anim_obj, anim_report_cb);
    lv_anim_set_values(&vertical_ball0_anim_obj, -100, 100);
//    lv_anim_start(&vertical_ball0_anim_obj);

    return;
}

static void popup_create(lv_obj_t * parent)
{
    lv_obj_t * ball0, * ball2, * ball3, * ball4, * ball5, * ball6, * ball7, * ball8, *ball9;
    lv_obj_t * ball1;
//    lv_anim_t a_ball0, a_ball1, a_ball2, a_ball3, horizontal_ball_anim_obj[4], horizontal_ball_anim_obj[5], horizontal_ball_anim_obj[6], horizontal_ball_anim_obj[7], horizontal_ball_anim_obj[8], horizontal_ball_anim_obj[9];

    // ball 0
    ball0 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball0, lv_palette_main(LV_PALETTE_RED), 0);
    lv_obj_set_style_radius(ball0, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball0, 20, 20);
    lv_obj_center(ball0);
    lv_obj_set_y(ball0, -80);

    lv_anim_init(&horizontal_ball_anim_obj[0]);
    lv_anim_set_var(&horizontal_ball_anim_obj[0], ball0);
    lv_anim_set_duration(&horizontal_ball_anim_obj[0], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[0], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[0], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[0], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[0], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[0], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[0], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[0]);

    // ball 1
    ball1 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball1, lv_palette_main(LV_PALETTE_PURPLE), 0);
    lv_obj_set_style_radius(ball1, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball1, 20, 20);
    lv_obj_center(ball1);
    lv_obj_set_y(ball1, -60);

    lv_anim_init(&horizontal_ball_anim_obj[1]);
    lv_anim_set_var(&horizontal_ball_anim_obj[1], ball1);
    lv_anim_set_duration(&horizontal_ball_anim_obj[1], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[1], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[1], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[1], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[1], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[1], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[1], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[1]);

    // ball 2
    ball2 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball2, lv_palette_main(LV_PALETTE_BLUE), 0);
    lv_obj_set_style_radius(ball2, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball2, 20, 20);
    lv_obj_center(ball2);
    lv_obj_set_y(ball2, -40);

    lv_anim_init(&horizontal_ball_anim_obj[2]);
    lv_anim_set_var(&horizontal_ball_anim_obj[2], ball2);
    lv_anim_set_duration(&horizontal_ball_anim_obj[2], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[2], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[2], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[2], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[2], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[2], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[2], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[2]);

    // ball 3
    ball3 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball3, lv_palette_main(LV_PALETTE_CYAN), 0);
    lv_obj_set_style_radius(ball3, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball3, 20, 20);
    lv_obj_center(ball3);
    lv_obj_set_y(ball3, -20);

    lv_anim_init(&horizontal_ball_anim_obj[3]);
    lv_anim_set_var(&horizontal_ball_anim_obj[3], ball3);
    lv_anim_set_duration(&horizontal_ball_anim_obj[3], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[3], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[3], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[3], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[3], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[3], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[3], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[3]);

    // ball 4
    ball4 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball4, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_set_style_radius(ball4, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball4, 20, 20);
    lv_obj_center(ball4);
    lv_obj_set_y(ball4, 0);

    lv_anim_init(&horizontal_ball_anim_obj[4]);
    lv_anim_set_var(&horizontal_ball_anim_obj[4], ball4);
    lv_anim_set_duration(&horizontal_ball_anim_obj[4], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[4], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[4], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[4], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[4], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[4], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[4], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[4]);

    // ball 5
    ball5 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball5, lv_palette_main(LV_PALETTE_LIME), 0);
    lv_obj_set_style_radius(ball5, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball5, 20, 20);
    lv_obj_center(ball5);
    lv_obj_set_y(ball5, 20);

    lv_anim_init(&horizontal_ball_anim_obj[5]);
    lv_anim_set_var(&horizontal_ball_anim_obj[5], ball5);
    lv_anim_set_duration(&horizontal_ball_anim_obj[5], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[5], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[5], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[5], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[5], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[5], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[5], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[5]);

    // ball 6
    ball6 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball6, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_set_style_radius(ball6, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball6, 20, 20);
    lv_obj_center(ball6);
    lv_obj_set_y(ball6, 40);

    lv_anim_init(&horizontal_ball_anim_obj[6]);
    lv_anim_set_var(&horizontal_ball_anim_obj[6], ball6);
    lv_anim_set_duration(&horizontal_ball_anim_obj[6], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[6], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[6], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[6], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[6], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[6], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[6], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[6]);

    // ball 7
    ball7 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball7, lv_palette_main(LV_PALETTE_ORANGE), 0);
    lv_obj_set_style_radius(ball7, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball7, 20, 20);
    lv_obj_center(ball7);
    lv_obj_set_y(ball7, 60);

    lv_anim_init(&horizontal_ball_anim_obj[7]);
    lv_anim_set_var(&horizontal_ball_anim_obj[7], ball7);
    lv_anim_set_duration(&horizontal_ball_anim_obj[7], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[7], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[7], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[7], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[7], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[7], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[7], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[7]);

    // ball 8
    ball8 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball8, lv_palette_main(LV_PALETTE_AMBER), 0);
    lv_obj_set_style_radius(ball8, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball8, 20, 20);
    lv_obj_center(ball8);
    lv_obj_set_y(ball8, 80);

    lv_anim_init(&horizontal_ball_anim_obj[8]);
    lv_anim_set_var(&horizontal_ball_anim_obj[8], ball8);
    lv_anim_set_duration(&horizontal_ball_anim_obj[8], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[8], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[8], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[8], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[8], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[8], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[8], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[8]);

    // ball 9
    ball9 = lv_obj_create(parent);
    lv_obj_set_style_bg_color(ball9, lv_palette_main(LV_PALETTE_TEAL), 0);
    lv_obj_set_style_radius(ball9, LV_RADIUS_CIRCLE, 0);
    lv_obj_set_size(ball9, 20, 20);
    lv_obj_center(ball9);
    lv_obj_set_y(ball9, 100);

    lv_anim_init(&horizontal_ball_anim_obj[9]);
    lv_anim_set_var(&horizontal_ball_anim_obj[9], ball9);
    lv_anim_set_duration(&horizontal_ball_anim_obj[9], 1000);
    lv_anim_set_reverse_delay(&horizontal_ball_anim_obj[9], 100);
    lv_anim_set_reverse_duration(&horizontal_ball_anim_obj[9], 300);
    lv_anim_set_repeat_delay(&horizontal_ball_anim_obj[9], 500);
    lv_anim_set_repeat_count(&horizontal_ball_anim_obj[9], ANIMATION_COUNT);
    lv_anim_set_exec_cb(&horizontal_ball_anim_obj[9], anim_x_cb);
    lv_anim_set_values(&horizontal_ball_anim_obj[9], -100, 100);
//    lv_anim_start(&horizontal_ball_anim_obj[9]);

//    ball_animate(&ball0, &a_ball0);




    return;
}


static void tabview_delete_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        lv_style_reset(&style_text_muted);
        lv_style_reset(&style_title);
        lv_style_reset(&style_icon);
        lv_style_reset(&style_bullet);
    }
}
