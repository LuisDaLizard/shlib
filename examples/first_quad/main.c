//
// Created by Luis on 7/11/2023.
//

#include <shlib/shlib.h>

Vertex vertices[4] =
        {
                {{100, 100, 0}, {}, {1, 1}}, // Top Right
                {{100, -100, 0}, {}, {1, 0}},  // Bottom Right
                {{-100, -100, 0}, {}, {0, 0}},   // Bottom Left
                {{-100, 100, 0}, {}, {0, 1}}   // Top Left
        };

unsigned int indices[6] =
        {
            0, 2, 1,
            0, 3, 2
        };

unsigned char image_data[16] =
        {
                0x00, 0x00, 0x00, 0xff, // Top Right
                0x00, 0x00, 0xff, 0xff, // Top Left
                0x00, 0xff, 0x00, 0xff, // Bottom Right
                0xff, 0x00, 0x00, 0xff // Bottom Left
        };

const char *vertex_src = "#version 400 core\n"
                         "\n"
                         "layout (location = 0) in vec3 aPosition;\n"
                         "layout (location = 1) in vec3 aNormal;\n"
                         "layout (location = 2) in vec2 aTexCoord;\n"
                         "\n"
                         "uniform mat4 uProjection;\n"
                         "\n"
                         "out vec2 fTexCoord;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "    fTexCoord = aTexCoord;\n"
                         "    gl_Position = uProjection * vec4(aPosition, 1);\n"
                         "}";
const char *fragment_src = "#version 400 core\n"
                           "\n"
                           "in vec2 fTexCoord;\n"
                           "\n"
                           "uniform sampler2D texture0;\n"
                           "\n"
                           "out vec4 oColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    oColor = texture(texture0, fTexCoord);\n"
                           "}";

int main()
{
    window_init(800, 600, "Example 1 - First Quad");

    Model *quad = model_load_from_mesh(mesh_create(vertices, indices, 4, 6));
    Shader *shader = shader_load(vertex_src, fragment_src);
    Texture *texture = texture_load(image_data, 2, 2, 4);
    int projection_loc = shader_get_location(shader, "uProjection");

    while(!window_should_close())
    {
        graphics_begin_drawing();
        graphics_clear_screen((Vec4){0.1f, 0.1f, 0.1f, 1});

        Vec2 size = window_get_size();
        Matrix projection = matrix_ortho(-size.x / 2.0f, size.x / 2.0f, size.y / 2.0f, -size.y / 2.0f, 0.0f, 1.0f);
        shader_set_uniform_matrix(shader, projection_loc, projection);

        shader_use(shader);
        texture_use(texture, 0);
        model_draw(quad);

        graphics_end_drawing();
    }

    shader_unload(shader);
    model_unload(quad);
    texture_unload(texture);

    window_destroy();

    return 0;
}