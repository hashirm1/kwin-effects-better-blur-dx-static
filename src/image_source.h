#pragma once
#include <QImage>
#include <QImageReader>
#include <QColorSpace>
#include <QPainter>
namespace SBDX {
inline QImage loadStaticImage(const QString &path) {
 QImageReader reader(path); reader.setAutoTransform(true);
 QImage image=reader.read(); if(image.isNull()) return {};
 if(!image.colorSpace().isValid()) image.setColorSpace(QColorSpace::SRgb);
 image.convertToColorSpace(QColorSpace::SRgb);
 QImage opaque(image.size(),QImage::Format_RGB32); opaque.fill(Qt::black);
 QPainter painter(&opaque); painter.drawImage(0,0,image); painter.end();
 opaque.setColorSpace(QColorSpace::SRgb); return opaque;
}
inline QImage fillStaticImage(const QImage &image, QSize size) {
 if(image.isNull() || size.isEmpty()) return {};
 const auto scaled=image.scaled(size,Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation);
 return scaled.copy((scaled.width()-size.width())/2,(scaled.height()-size.height())/2,size.width(),size.height());
}
}
