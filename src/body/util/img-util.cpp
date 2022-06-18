#include "img-util.h"

#include <QBuffer>


namespace myapp::ImgUtil {

QByteArray toByteArray(const QPixmap &img, const char *format)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, format);
    return bytes;
}

scc::buff toSccBuff(const QImage &img, const char *format, std::pmr::polymorphic_allocator<std::byte> allocator)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, format);

    scc::buff buff(allocator);
    buff.resize(bytes.size());
    std::memcpy(buff.data(), bytes.constData(), buff.size());

    return buff;
}

}
