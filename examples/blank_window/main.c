//
// Created by luist on 7/3/2023.
//

#include <shlib/shlib.h>

int main()
{
    window_init(800, 600, "Example 1 - Blank Window");

    while(!window_should_close())
    {
        graphics_begin_drawing();
        graphics_clear_screen((Vec4){0.5f, 0, 0, 1});
        graphics_end_drawing();
    }

    window_destroy();
}