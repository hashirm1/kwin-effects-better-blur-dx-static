#include "utils.h"
#include <epoxy/gl.h>
namespace SBDX {
QString shaderFilePath(const QString &path) { auto p=path; auto dot=p.lastIndexOf('.'); p.insert(dot,QStringLiteral("_core")); return p; }
void clearGLScissor() { glDisable(GL_SCISSOR_TEST); }
void setGLScissor(const KWin::Region &region, const KWin::Rect &background) {
 auto rect=region.boundingRect().translated(-background.topLeft());
 glEnable(GL_SCISSOR_TEST);
 glScissor(rect.x(),background.height()-rect.bottom(),rect.width(),rect.height());
}
}
