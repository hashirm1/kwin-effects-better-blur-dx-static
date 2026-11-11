#include "blurconfig.h"
#include <KConfigGroup>
#include <QCoreApplication>
#include <QTemporaryDir>
int main(int argc,char **argv){
 QCoreApplication app(argc,argv); QTemporaryDir dir;
 auto file=KSharedConfig::openConfig(dir.filePath("kwinrc"),KConfig::SimpleConfig);
 KConfigGroup other(file,"Effect-better-blur-dx");other.writeEntry("BlitMode",1);other.sync();
 SBDX::BlurConfig::instance(file);
 if(SBDX::BlurConfig::blitMode()!=SBDX::BlitMode::RENDER_TARGET) return 1;
 SBDX::BlurConfig::setBlitMode(SBDX::BlitMode::CUSTOM_IMAGE);
 SBDX::BlurConfig::setStaticImagePath("/tmp/a path.png");SBDX::BlurConfig::self()->save();
 KConfigGroup own(file,"Effect-static-blur-dx");
 if(own.readEntry("BlitMode",QString())!="CUSTOM_IMAGE" || own.readEntry("StaticImagePath",QString())!="/tmp/a path.png" || other.readEntry("BlitMode",0)!=1)return 2;
 SBDX::BlurConfig::self()->read();
 return SBDX::BlurConfig::blitMode()==SBDX::BlitMode::CUSTOM_IMAGE?0:3;
}
