//
// Created by Luis on 7/11/2023.
//

#include <shlib/shlib.h>

Vertex vertices[4] =
        {
                {{100, 100, 0}}, // Top Right
                {{100, -100, 0}},  // Bottom Right
                {{-100, -100, 0}},   // Bottom Left
                {{-100, 100, 0}}   // Top Left
        };

unsigned int indices[6] =
        {
            0, 1, 3,
            1, 2, 3
        };

const char *vertex_src = "#version 400 core\n"
                         "\n"
                         "layout (location = 0) in vec3 aPosition;\n"
                         "layout (location = 1) in vec3 aNormal;\n"
                         "layout (location = 2) in vec2 aTexCoord;\n"
                         "\n"
                         "uniform mat4 uProjection;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "    gl_Position = uProjection * vec4(aPosition, 1);\n"
                         "}";
const char *fragment_src = "#version 400 core\n"
                           "\n"
                           "out vec4 oColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    oColor = vec4(1, 1, 1, 1);\n"
                           "}";

int main()
{
    window_init(800, 600, "Example 3 - 3D Cube");

    Model *quad = model_load_from_mesh(mesh_create(vertices, indices, 4, 6));
    Shader *shader = shader_load_from_memory(vertex_src, fragment_src);
    int projection_loc = shader_get_location(shader, "uProjection");

    while(!window_should_close())
    {
        graphics_begin_drawing();
        graphics_clear_screen((Vec4){0.1f, 0.1f, 0.1f, 1});

        Vec2 size = window_get_size();
        Matrix projection = matrix_ortho(-size.x / 2.0f, size.x / 2.0f, size.y / 2.0f, -size.y / 2.0f, 0.0f, 1.0f);
        shader_set_uniform_matrix(shader, projection_loc, projection);

        shader_use(shader);
        model_draw(quad);

        graphics_end_drawing();
    }

    shader_unload(shader);
    model_unload(quad);

    window_destroy();

    return 0;
}