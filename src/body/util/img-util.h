#pragma once

#include <QPixmap>
#include <QByteArray>


namespace myapp::ImgUtil {

QByteArray toByteArray(const QPixmap &img, const char *format);

}
