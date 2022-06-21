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

Buff toBuff(const QImage &img, const char *format)
{
    QByteArray bytes;
    QBuffer buffer(&bytes);
    buffer.open(QIODevice::WriteOnly);
    img.save(&buffer, format);

    Buff buff;
    buff.resize(bytes.size());
    std::memcpy(buff.data(), bytes.constData(), buff.size());

    return buff;
}

}
