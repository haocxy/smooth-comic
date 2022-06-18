#pragma once

#include <QPixmap>
#include <QByteArray>

#include "core/scoped_container.h"


namespace myapp::ImgUtil {

QByteArray toByteArray(const QPixmap &img, const char *format);

scc::buff toSccBuff(const QImage &img, const char *format, std::pmr::polymorphic_allocator<std::byte> allocator);

}
