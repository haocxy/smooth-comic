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

}
