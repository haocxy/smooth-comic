#pragma once

#include <QPixmap>
#include <QByteArray>

#include "core/basetype.h"


namespace myapp::ImgUtil {

QByteArray toByteArray(const QPixmap &img, const char *format);

Buff toBuff(const QImage &img, const char *format);

}
