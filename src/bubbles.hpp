#pragma once

#include "opengl/buffer.hpp"
#include "opengl/framebuffer.hpp"
#include "opengl/vertexarray.hpp"

#include <opengl/program.hpp>
#include <opengl/renderbuffer.hpp>
#include <opengl/texture.hpp>
#include <vector>

class Bubbles {
public:
    Bubbles(GLsizei width, GLsizei height);

    engine::opengl::Framebuffer::Binding bind_as_target() const;
    void draw(float time) const;

    void add_bubble(GLfloat x, GLfloat y, GLfloat r);
private:
    union BubbleEntry {
        GLfloat value;
        GLfloat padding[4];
    };
    engine::opengl::Program shader;
    GLsizei screen_width;
    GLsizei screen_height;
    engine::opengl::Framebuffer framebuffer;
    engine::opengl::Texture destination;
    engine::opengl::Renderbuffer depth_stencil;
    engine::opengl::VertexArray vao;
    engine::opengl::Buffer screen_vertices;
    engine::opengl::Buffer bubbles_buffer;
    std::vector<BubbleEntry> bubbles;
};
