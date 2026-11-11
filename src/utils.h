#pragma once
#include "kwin_compat.hpp"
#include <core/rect.h>
#include <core/region.h>
#include <QString>
namespace SBDX {
inline constexpr auto LOG_PREFIX = "[Static Blur DX]";
inline QSize getTextureSize(const KWin::Rect &r, size_t i) { return {std::max(1,r.width()/int(1<<i)),std::max(1,r.height()/int(1<<i))}; }
QString shaderFilePath(const QString &path);
void setGLScissor(const KWin::Region &, const KWin::Rect &);
void clearGLScissor();
inline KWin::Rect rectRoundedIn(const KWin::RectF &r) { return r.roundedIn(); }
inline KWin::Rect rectRoundedOut(const KWin::RectF &r) { return r.roundedOut(); }
inline KWin::RegionF regionTranslatedF(const KWin::RegionF &r, const QPointF &p) { return r.translated(p); }
}
