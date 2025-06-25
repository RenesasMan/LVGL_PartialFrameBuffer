/*
 * LVGL_thread_entry.h
 *
 *  Created on: Jan 17, 2025
 *      Author: a5138119
 */

#ifndef LVGL_THREAD_ENTRY_H_
#define LVGL_THREAD_ENTRY_H_


//#define DISPLAY_HSIZE_INPUT0 320
//#define DISPLAY_VSIZE_INPUT0 240

#define DISPLAY_HSIZE_INPUT0 240
#define DISPLAY_VSIZE_INPUT0 320

/** Display event codes */
typedef enum e_display_event
{
    DISPLAY_EVENT_GR1_UNDERFLOW  = 1,  ///< Graphics frame1 underflow occurs
    DISPLAY_EVENT_GR2_UNDERFLOW  = 2,  ///< Graphics frame2 underflow occurs
    DISPLAY_EVENT_LINE_DETECTION = 3,  ///< Designated line is processed
    DISPLAY_EVENT_FRAME_END      = 4,  ///< Frame end is processed
} display_event_t;

#endif /* LVGL_THREAD_ENTRY_H_ */
