#include "bubbles.hpp"
#include <assets/bubbles.frag.h>
#include <assets/bubbles.vert.h>

#include <glm/gtx/transform.hpp>


namespace {

GLfloat screen_vertex_data[] = {
    /*   X      Y     S     T    */
    -2.0f, -2.0f, -0.5f, -0.5f,
    -2.0f, 2.0f, -0.5f, 1.5f,
    2.0f, 2.0f, 1.5f, 1.5f,

    -2.0f, -2.0f, -0.5f, -0.5f,
    2.0f, 2.0f, 1.5f, 1.5f,
    2.0f, -2.0f, 1.5f, -0.5f};

constexpr int ATTRIBUTE_POSITION = 0;
constexpr int ATTRIBUTE_TEXTURE_COORD = 1;

}

Bubbles::Bubbles(GLsizei width, GLsizei height)
    : screen_width(width), screen_height(height) {
    bubbles.reserve(1000);
    shader.attach(engine::opengl::Shader(GL_VERTEX_SHADER, reinterpret_cast<const char *>(bubbles_vert), sizeof(bubbles_vert)));
    shader.attach(engine::opengl::Shader(GL_FRAGMENT_SHADER, reinterpret_cast<const char *>(bubbles_frag), sizeof(bubbles_frag)));
    shader.bind(ATTRIBUTE_POSITION, "position");
    shader.bind(ATTRIBUTE_TEXTURE_COORD, "texture_coord");
    shader.link();
    shader.uniform_block_binding("bubbles", 0);
    {
        const auto binding = vao.bind();
        binding.enable_attribute(ATTRIBUTE_POSITION);
        binding.enable_attribute(ATTRIBUTE_TEXTURE_COORD);

        auto buffer_binding = screen_vertices.bind(GL_ARRAY_BUFFER);
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_POSITION, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(0 * sizeof(float)));
        buffer_binding.vertex_attrib_pointer(binding, ATTRIBUTE_TEXTURE_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, reinterpret_cast<void *>(2 * sizeof(float)));
        buffer_binding.data(sizeof(screen_vertex_data), screen_vertex_data, GL_STATIC_DRAW);
    }
    {
        auto binding = bubbles_buffer.bind(GL_UNIFORM_BUFFER);
        binding.data(4 * sizeof(GLint) + sizeof(BubbleEntry) * 1000, nullptr, GL_DYNAMIC_DRAW);
        const GLint size = bubbles.size();
        binding.subdata(0, sizeof(GLint), &size);
    }
    bubbles_buffer.bind_buffer_base(GL_UNIFORM_BUFFER, 0);
    {
        auto binding = destination.bind(GL_TEXTURE0, GL_TEXTURE_2D);
        binding.image_2d(0, GL_RGB16F, screen_width, screen_height, 0, GL_RGBA, GL_FLOAT, nullptr);
        binding.set_parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        binding.set_parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        binding.set_parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        binding.set_parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    {
        auto binding = depth_stencil.bind(GL_RENDERBUFFER);
        binding.storage(GL_DEPTH24_STENCIL8, screen_width, screen_height);
    }
    {
        auto binding = framebuffer.bind(GL_FRAMEBUFFER);
        binding.attach(GL_COLOR_ATTACHMENT0, destination, 0);
        binding.attach(GL_DEPTH_STENCIL_ATTACHMENT, depth_stencil);
        if (binding.get_status() != GL_FRAMEBUFFER_COMPLETE) {
            throw std::runtime_error("Destination framebuffer not complete");
        }
    }
    for (int i = 0; i < 10; ++i) {
        add_bubble(static_cast<GLfloat>(rand()) / RAND_MAX, static_cast<GLfloat>(rand()) / RAND_MAX, 0.1 * static_cast<GLfloat>(rand()) / RAND_MAX);
    }
    /*bubbles.push_back(BubbleEntry{0.5f});
    bubbles.push_back(BubbleEntry{0.5f});
    bubbles.push_back(BubbleEntry{0.07f});

    bubbles.push_back(BubbleEntry{0.75f});
    bubbles.push_back(BubbleEntry{0.75f});
    bubbles.push_back(BubbleEntry{0.08f});

    bubbles.push_back(BubbleEntry{0.25f});
    bubbles.push_back(BubbleEntry{0.25f});
    bubbles.push_back(BubbleEntry{0.02f});*/
}

engine::opengl::Framebuffer::Binding Bubbles::bind_as_target() const {
    glViewport(0, 0, screen_width, screen_height);
    return framebuffer.bind(GL_FRAMEBUFFER);
}

void Bubbles::draw(float time) const {
    auto usage = shader.use();
    glm::mat4x4 projection{translate(glm::vec3(0.0f, 0.0f, 0.0f))};
    usage.set_uniform("projection", projection);
    usage.set_uniform("gamma", 2.2f);
    usage.set_uniform("exposure", 0.5f);
    usage.set_uniform("time", time);
    auto binding = vao.bind();
    auto bubbles_binding = bubbles_buffer.bind(GL_UNIFORM_BUFFER);
    const GLint size = bubbles.size();
    bubbles_binding.subdata(0, sizeof(GLint), &size);
    const void *const data = bubbles.data();
    bubbles_binding.subdata(4 * sizeof(GLint), size * sizeof(BubbleEntry), data);
    auto destination_texture_binding = destination.bind(GL_TEXTURE0, GL_TEXTURE_2D);
    glDrawArrays(GL_TRIANGLES, 0, sizeof(screen_vertex_data) / 4);
}

void Bubbles::add_bubble(GLfloat x, GLfloat y, GLfloat r) {
    bubbles.push_back(BubbleEntry{x});
    bubbles.push_back(BubbleEntry{y});
    bubbles.push_back(BubbleEntry{r});
}
