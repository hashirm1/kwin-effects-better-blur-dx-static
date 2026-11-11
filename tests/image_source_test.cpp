#include "image_source.h"
#include <QCoreApplication>
#include <QTemporaryDir>
#include <QFile>
#include <iostream>
int main(int argc,char **argv) {
 QCoreApplication app(argc,argv); int failures=0;
 auto check=[&](bool ok,const char *name){if(!ok){std::cerr<<name<<'\n';++failures;}};
 QTemporaryDir dir;
 QImage image(6,2,QImage::Format_ARGB32); image.fill(Qt::red);
 for(int y=0;y<2;++y)for(int x=2;x<4;++x)image.setPixelColor(x,y,Qt::green);
 image.save(dir.filePath("crop.png"));
 auto decoded=SBDX::loadStaticImage(dir.filePath("crop.png"));
 auto cropped=SBDX::fillStaticImage(decoded,{2,2});
 check(cropped.size()==QSize(2,2)&&cropped.pixelColor(0,0)==QColor(Qt::green)&&cropped.pixelColor(1,1)==QColor(Qt::green),"centered crop preserves aspect ratio");
 check(decoded.colorSpace()==QColorSpace(QColorSpace::SRgb),"untagged image normalized to sRGB");
 image.fill(Qt::transparent);image.save(dir.filePath("alpha.png"));
 auto alpha=SBDX::loadStaticImage(dir.filePath("alpha.png"));
 check(alpha.pixelColor(0,0)==QColor(Qt::black),"transparent pixels become opaque black");
 check(SBDX::loadStaticImage(dir.filePath("missing")).isNull(),"missing file refused");
 QFile bad(dir.filePath("bad.png"));bad.open(QIODevice::WriteOnly);bad.write("not an image");bad.close();
 check(SBDX::loadStaticImage(bad.fileName()).isNull(),"corrupt file refused");
 check(SBDX::fillStaticImage(decoded,{}).isNull(),"empty output refused");
 image.fill(QColor(80,120,160));image.setColorSpace(QColorSpace::DisplayP3);image.save(dir.filePath("profile.png"));
 check(SBDX::loadStaticImage(dir.filePath("profile.png")).colorSpace()==QColorSpace(QColorSpace::SRgb),"embedded profile converted");
 return failures?1:0;
}
