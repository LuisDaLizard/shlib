//
// Created by Luis on 7/25/2023.
//

#include <shlib/shlib.h>

Vertex3D vertices[4] =
        {
                {{100, 100, -1}, {}, {1, 1}}, // Top Right
                {{100, -100, 0}, {}, {1, 0}},  // Bottom Right
                {{-100, -100, 0}, {}, {0, 0}},   // Bottom Left
                {{-100, 100, -1}, {}, {0, 1}}   // Top Left
        };

unsigned int indices[6] =
        {
                0, 2, 1,
                0, 3, 2
        };

const char *shadow_vert = "#version 400 core\n"
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
const char *shadow_frag = "#version 400 core\n"
                           "\n"
                           "out vec4 oColor;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "}";

const char *texture_vert = "#version 400 core\n"
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
const char *texture_frag = "#version 400 core\n"
                           "\n"
                           "out vec4 FragColor;\n"
                           " \n"
                           "in vec2 fTexCoord;\n"
                           "\n"
                           "uniform sampler2D depthMap;\n"
                           "\n"
                           "void main()\n"
                           "{\n"
                           "    float depthValue = texture(depthMap, fTexCoord).r;\n"
                           "    FragColor = vec4(vec3(depthValue), 1.0);\n"
                           "}";

int main()
{
    window_init(800, 600, "Example 4 - Depth Buffer");

    Framebuffer *shadow_map = framebuffer_create_depth(1024, 1024);
    Mesh *model = mesh_create(vertices, indices, 4, 6);
    Shader *shadow = shader_load(shadow_vert, shadow_frag);
    Shader *texture = shader_load(texture_vert, texture_frag);

    while(!window_should_close())
    {
        window_poll_events();

        framebuffer_bind(shadow_map);
        graphics_clear_screen((Vec4){0.2f, 0.2f, 0.2f, 1});
        Vec2 size = window_get_size();
        Matrix projection = matrix_ortho(-size.x / 2.0f, size.x / 2.0f, size.y / 2.0f, -size.y / 2.0f, 0.0f, 1.0f);
        shader_upload_matrix(shadow, "uProjection", projection);
        shader_use(shadow);
        graphics_draw_mesh(model);
        framebuffer_unbind();

        graphics_clear_screen((Vec4){0.2f, 0.2f, 0.2f, 1});
        shader_upload_matrix(texture, "uProjection", projection);
        shader_use(texture);
        texture_use(shadow_map->texture, 0);
        graphics_draw_mesh(model);

        window_swap_buffers();
    }

    mesh_destroy(model);
    framebuffer_destroy(shadow_map);
    shader_unload(shadow);
    shader_unload(texture);

    window_destroy();
    return 0;
}