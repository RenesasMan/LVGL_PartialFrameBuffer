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


#define EP_INFO  "\r\nThis Example Project demonstrates CRC operation for transmission\r\n"\
                  "and reception in normal mode. On any key press through RTT Viewer\r\n"\
                  "CRC value in normal mode is calculated for 4 bytes of data.\r\n"\
                  "The calculated CRC value along with the data is trans-received on\r\n"\
                  "sci_uart through loop-back.\r\n"\
                  "\r\nIf the CRC value of data received is zero and transmit and receive\r\n"\
                  "buffer are equal then On-board LED blinks as sign of successful\r\n"\
                  "CRC operation. On a data mismatch, LED stays ON. Failure messages\r\n"\
                  "and status is displayed on RTTViewer.\r\n" //TODO, change this


#endif /* LVGL_THREAD_ENTRY_H_ */
