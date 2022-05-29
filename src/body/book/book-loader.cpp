#include "book-loader.h"

#include <string>
#include <sstream>

#include "engine/engine.h"

#include "img-archive.h"


namespace myapp {



BookLoader::BookLoader(Engine &engine, QObject *parent)
    : QObject(parent)
    , mEngine(engine)
{

    mLoadThread.setObjectName("BookLoader");

    moveToThread(&mLoadThread);

    initSignalsAndSlots();

    mLoadThread.start();
}

BookLoader::~BookLoader()
{
    disconnect();

    stopped_ = true;
}

void BookLoader::initSignalsAndSlots()
{
    using Class = BookLoader;

    connect(this, &Class::startLoadFromLocalFile, this, &Class::doStartLoadFromLocalFile);

}

static std::u32string getNameFromPagePath(const QString &filePath)
{
    const std::u32string s = filePath.toStdU32String();
    for (int32_t i = static_cast<int32_t>(s.size()) - 1; i >= 0; --i) {
        const char32_t ch = s[i];
        if (ch == '/' || ch == '\\') {
            return s.substr(i + 1);
        }
    }
    return s;
}

static PageNum getPageNumFromPagePath(const QString &filePath)
{
    const std::u32string fileName = getNameFromPagePath(filePath);
    std::string buff;
    for (char32_t c : fileName) {
        if (std::isdigit(c)) {
            buff.push_back(c);
        } else {
            if (!buff.empty()) {
                break;
            }
        }
    }

    PageNum n = 0;

    std::istringstream ss(buff);
    ss >> n;

    return n;
}

void BookLoader::doStartLoadFromLocalFile(const QString &filePath)
{
    ImgArchive archive;
    archive.load(filePath.toStdU32String(), [this](const QString &pagePath, const QImage &img) {
        const PageNum pageNum = getPageNumFromPagePath(pagePath);
        emit onPageLoaded(pageNum, img);
        return !stopped_;
    });
}

}
