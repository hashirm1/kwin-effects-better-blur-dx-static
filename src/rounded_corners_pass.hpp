#pragma once
#include <opengl/glshader.h>
#include <opengl/glvertexbuffer.h>
#include <core/renderviewport.h>
#include <memory>
namespace SBDX {
class WindowManager; class BlurCache; class BlurCacheEntry; struct BlurRenderData;
class RoundedCornersPass {
 std::unique_ptr<KWin::GLShader> m_shader;
 int m_mvpMatrixLocation, m_modulationLocation, m_boxLocation, m_cornerRadiusLocation;
public:
 static std::unique_ptr<RoundedCornersPass> create();
 bool drawRounded(const WindowManager *, const BlurCache *, BlurCacheEntry *, KWin::GLVertexBuffer *, int, float) const;
};
}
