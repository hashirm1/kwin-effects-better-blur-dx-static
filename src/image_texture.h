#pragma once
#include <opengl/gltexture.h>
#include <opengl/glframebuffer.h>
#include <opengl/glshadermanager.h>
#include <opengl/glshader.h>
#include <core/colorspace.h>
namespace SBDX {
inline std::unique_ptr<KWin::GLTexture> convertStaticImage(const QImage &image, GLenum format, const std::shared_ptr<KWin::ColorDescription> &description) {
    const auto size=image.size();
    auto upload = KWin::GLTexture::upload(image);
    if (!upload) return {};
    upload->setFilter(GL_LINEAR);
    upload->setWrapMode(GL_CLAMP_TO_EDGE);
    auto texture = KWin::GLTexture::allocate(format, size);
    if (!texture) return {};
    auto framebuffer = std::make_unique<KWin::GLFramebuffer>(texture.get());
    if (!framebuffer->valid()) return {};
    auto m_imageShader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture | KWin::ShaderTrait::TransformColorspace);
    if (!m_imageShader) return {};
    KWin::ShaderManager::instance()->pushShader(m_imageShader.get());
    m_imageShader->setColorspaceUniforms(KWin::ColorDescription::sRGB, description, KWin::RenderingIntent::RelativeColorimetricWithBPC);
    QMatrix4x4 projection;
    projection.scale(1,-1);
    projection.ortho(QRect(QPoint(), size));
    m_imageShader->setUniform(KWin::GLShader::Mat4Uniform::ModelViewProjectionMatrix, projection);
    KWin::GLFramebuffer::pushFramebuffer(framebuffer.get());
    const bool blendEnabled = glIsEnabled(GL_BLEND);
    const bool scissorEnabled = glIsEnabled(GL_SCISSOR_TEST);
    GLfloat clearColor[4]; glGetFloatv(GL_COLOR_CLEAR_VALUE, clearColor);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glClearColor(0,0,0,1); glClear(GL_COLOR_BUFFER_BIT);
    upload->render(QSizeF(size));
    glClearColor(clearColor[0],clearColor[1],clearColor[2],clearColor[3]);
    if (blendEnabled) glEnable(GL_BLEND);
    if (scissorEnabled) glEnable(GL_SCISSOR_TEST);
    KWin::GLFramebuffer::popFramebuffer();
    KWin::ShaderManager::instance()->popShader();
    return texture;
}
}
