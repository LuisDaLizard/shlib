//
// Created by luist on 7/3/2023.
//

#include <shlib/shlib.h>
#include <stdio.h>

#define MAX_QUADS 100

const char *quad_vert_src = "#version 400 core\n"
                         "\n"
                         "layout (location = 0) in vec3 aPosition;\n"
                         "layout (location = 1) in vec4 aColor;\n"
                         "layout (location = 2) in vec2 aTexCoord;\n"
                         "layout (location = 3) in float aTexId;\n"
                         "\n"
                         "uniform mat4 uProjection;\n"
                         "\n"
                         "out vec4 fColor;\n"
                         "out vec2 fTexCoord;\n"
                         "out float fTexId;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "    fColor = aColor;\n"
                         "    fTexCoord = aTexCoord;\n"
                         "    fTexId = aTexId;\n"
                         "    gl_Position = uProjection * vec4(aPosition, 1);\n"
                         "}";
const char *quad_frag_src = "#version 400 core\n"
                           "\n"
                           "in vec4 fColor;\n"
                           "in vec2 fTexCoord;\n"
                           "in float fTexId;\n"
                           "\n"
                           "uniform sampler2D uTextures[16];\n"
                           "\n"
                           "out vec4 oColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    float a = texture(uTextures[int(fTexId)], fTexCoord).r;\n"
                           "    oColor = vec4(a);\n"
                           "}";

int main(int argc, char **argv)
{
    window_init(800, 600, "Example 5 - Batch Rendering");
    Font *font = font_load_from_file("./retro.ttf", 36);
    Batch *batch = batch_create(MAX_QUADS);
    Shader *shader = shader_load(quad_vert_src, quad_frag_src);
    int samplers[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    shader_upload_int_array(shader, "uTextures", 16, samplers);
    Matrix projection = matrix_ortho(-400, 400, 300, -300, -1.0f, 1.0f);

    while(!window_should_close())
    {
        window_poll_events();
        graphics_clear_screen((Vec4){0.1f, 0.1f, 0.1f});

        batch_add_text(batch, (Vec2){0, 0}, font, "Hello World!");

        shader_upload_matrix(shader, "uProjection", projection);
        shader_use(shader);
        graphics_draw_batch_quads(batch);
        window_swap_buffers();
    }
    batch_destroy(batch);
    window_destroy();
}