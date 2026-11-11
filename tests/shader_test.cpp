#include <QGuiApplication>
#include <opengl/egldisplay.h>
#include <opengl/eglcontext.h>
#include <opengl/glshadermanager.h>
#include <opengl/glshader.h>
#include <iostream>
#include "image_texture.h"
int main(int argc,char **argv){
 QGuiApplication app(argc,argv);Q_INIT_RESOURCE(blur);
 auto getDisplay=reinterpret_cast<PFNEGLGETPLATFORMDISPLAYEXTPROC>(eglGetProcAddress("eglGetPlatformDisplayEXT"));
 if(!getDisplay) return 77;
 auto raw=getDisplay(EGL_PLATFORM_SURFACELESS_MESA,EGL_DEFAULT_DISPLAY,nullptr);
 auto display=KWin::EglDisplay::create(raw,nullptr);
 if(!display){std::cerr<<"EGL unavailable\n";return 77;}
 eglBindAPI(EGL_OPENGL_API);
 auto ctx=KWin::EglContext::create(display.get(),nullptr,nullptr);
 if(!ctx||!ctx->makeCurrent()){std::cerr<<"Context unavailable\n";return 77;}
 int failures=0;
 for(auto name:{"downsample","upsample","noise","onscreen","refraction","texture","rounded_corners"}){
 QString vertex=QStringLiteral(":/effects/static_blur_dx/shaders/")+(QString(name)=="rounded_corners"?"rounded_corners_core.vert":"vertex_core.vert");
 QString frag=QStringLiteral(":/effects/static_blur_dx/shaders/")+name+"_core.frag";
 auto shader=KWin::ShaderManager::instance()->generateShaderFromFile(KWin::ShaderTrait::MapTexture,vertex,frag);
 if(!shader){std::cerr<<"Shader failed: "<<name<<'\n';++failures;}
 }
 auto conversion=KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture|KWin::ShaderTrait::TransformColorspace);
 if(!conversion) ++failures;
 QImage image(8,8,QImage::Format_RGB32); image.fill(Qt::red);
 for(int y=4;y<8;++y)for(int x=0;x<8;++x)image.setPixelColor(x,y,Qt::blue);
 glEnable(GL_BLEND); glEnable(GL_SCISSOR_TEST); glScissor(0,0,0,0);
 auto converted=SBDX::convertStaticImage(image,GL_RGBA8,KWin::ColorDescription::sRGB);
 if(!glIsEnabled(GL_BLEND) || !glIsEnabled(GL_SCISSOR_TEST)) ++failures;
 glDisable(GL_BLEND); glDisable(GL_SCISSOR_TEST);
 if(!converted){std::cerr<<"Conversion failed\n";++failures;}
 else {
   auto result=converted->toImage();
   if(result.pixelColor(2,1).red()<240 || result.pixelColor(2,6).blue()<240){std::cerr<<"Image orientation/color mismatch\n";++failures;}
 }
 return failures?1:0;
}
