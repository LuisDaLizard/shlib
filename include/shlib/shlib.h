//
// Created by Luis Tadeo Sanchez on 7/1/23.
//

#ifndef SHLIB_SHLIB_H
#define SHLIB_SHLIB_H

#include <stdbool.h>

// ------------ Shlib Core Window API ------------ //
extern void window_init(int width, int height, const char *title);
extern void window_destroy(void);
extern bool window_should_close(void);
extern void window_poll_events(void);
extern void window_swap_buffers(void);
extern void window_toggle_fullscreen(void);

// ------------ Shlib Core Graphics API ------------ //
extern void graphics_clear_screen(float color[4]);
extern void graphics_begin_drawing(void);
extern void graphics_end_drawing(void);

#endif //SHLIB_SHLIB_H
