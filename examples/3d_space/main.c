//
// Created by Luis on 7/11/2023.
//

#include <shlib/shlib.h>

Vertex vertices[4] =
        {
                {{0.5f, 0.5f, 0}}, // Top Right
                {{0.5f, -0.5f, 0}},  // Bottom Right
                {{-0.5f, -0.5f, 0}},   // Bottom Left
                {{-0.5f, 0.5f, 0}}   // Top Left
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
                         "uniform mat4 uModel;\n"
                         "uniform mat4 uView;\n"
                         "\n"
                         "void main()\n"
                         "{\n"
                         "    gl_Position = uProjection * uView * uModel * vec4(aPosition, 1);\n"
                         "}";
const char *fragment_src = "#version 400 core\n"
                           "\n"
                           "out vec4 oColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    oColor = vec4(1, 1, 1, 1);\n"
                           "}";

Vec3 position = (Vec3){0, 0, 0};
Vec3 scale = (Vec3){2, 2, 2};
Vec3 rotation_dir = (Vec3){0, 1, 0};
float rotation = 15.0f;

int main()
{
    window_init(800, 600, "Example 3 - 3D Space");

    Model *cube = model_load_from_mesh(mesh_create(vertices, indices, 4, 6));
    Shader *shader = shader_load_from_memory(vertex_src, fragment_src);
    int projection_loc = shader_get_location(shader, "uProjection");
    int view_loc = shader_get_location(shader, "uView");
    int model_loc = shader_get_location(shader, "uModel");

    while(!window_should_close())
    {
        graphics_begin_drawing();
        graphics_clear_screen((Vec4){0.1f, 0.1f, 0.1f, 1});

        rotation += 0.5f;

        Vec2 window_size = window_get_size();
        Matrix projection = matrix_perspective(window_size.x / window_size.y, 90.0f, 0.01f, 1000.0f);
        shader_set_uniform_matrix(shader, projection_loc, projection);

        Matrix view = matrix_look_at((Vec3){0, 5, 10}, (Vec3){0, 0, 0}, (Vec3){0, 1, 0});
        shader_set_uniform_matrix(shader, view_loc, view);

        Matrix model = matrix_identity();
        model = matrix_scale(model, scale);
        model = matrix_rotate(model, rotation_dir, rotation);
        model = matrix_translate(model, position);
        shader_set_uniform_matrix(shader, model_loc, model);

        shader_use(shader);
        model_draw(cube);

        graphics_end_drawing();
    }

    shader_unload(shader);
    model_unload(cube);

    window_destroy();

    return 0;
}