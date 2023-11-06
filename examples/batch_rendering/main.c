//
// Created by luist on 7/3/2023.
//

#include <shlib/shlib.h>
#include <stdio.h>

#define MAX_QUADS 25

const char *vertex_src = "#version 400 core\n"
                         "\n"
                         "layout (location = 0) in vec3 aPosition;\n"
                         "layout (location = 1) in vec4 aColor;\n"
                         "layout (location = 2) in vec3 aNormal;\n"
                         "layout (location = 3) in vec2 aTexCoord;\n"
                         "\n"
                         "uniform mat4 uProjection;\n"
                         "\n"
                         "out vec4 fColor;\n"
                         "out vec2 fTexCoord;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "    fColor = aColor;\n"
                         "    fTexCoord = aTexCoord;\n"
                         "    gl_Position = uProjection * vec4(aPosition, 1);\n"
                         "}";
const char *fragment_src = "#version 400 core\n"
                           "\n"
                           "in vec4 fColor;\n"
                           "in vec2 fTexCoord;\n"
                           "\n"
                           "uniform sampler2D texture0;\n"
                           "\n"
                           "out vec4 oColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    oColor = fColor * texture(texture0, fTexCoord);\n"
                           "}";

int main()
{
    window_init(800, 600, "Example 5 - Batch Rendering");
    Batch *batch = batch_create(MAX_QUADS);
    Shader *shader = shader_load(vertex_src, fragment_src);
    Matrix projection = matrix_ortho(0, 800, 600, 0, -1.0f, 1.0f);

    while(!window_should_close())
    {
        window_poll_events();
        graphics_clear_screen((Vec4){0.1f, 0.1f, 0.1f});

        int i;
        for (i = 0; i < MAX_QUADS; i++)
        {
            batch_add_quad(batch, (Vec2){100 + 20 * i, 300}, (Vec2){18, 18}, (Vec4){1, 1, 1});
        }

        batch_add_quad(batch, (Vec2){400, 300}, (Vec2){100, 100}, (Vec4){1, 1, 0});

        shader_upload_matrix(shader, "uProjection", projection);
        shader_use(shader);
        graphics_draw_batch(batch);
        window_swap_buffers();
    }
    batch_destroy(batch);
    window_destroy();
}