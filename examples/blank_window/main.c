//
// Created by luist on 7/3/2023.
//

#include <shlib/shlib.h>
#include <stdio.h>
#include <math.h>

float rg = 0.0f;

int main()
{
    window_init(800, 600, "Example 1 - Blank Window");

    while(!window_should_close())
    {
        if (input_is_key_down(KEY_SPACE))
            rg += 0.02f;

        window_poll_events();

        graphics_clear_screen((Vec4){cosf(rg), sinf(rg), 0});

        window_swap_buffers();
    }

    window_destroy();
}