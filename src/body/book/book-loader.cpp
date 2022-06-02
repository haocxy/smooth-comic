#include "book-loader.h"

#include <string>
#include <sstream>

#include "core/logger.h"
#include "engine/engine.h"

#include "img-archive.h"


using namespace logger::global_loggers;


namespace myapp {



BookLoader::BookLoader()
{
    setActorName("BookLoader");
}

BookLoader::~BookLoader()
{
    logInfo << "BookLoader destructing";
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

void BookLoader::onMessage(actor::Message &msg)
{
    if (StartLoadMsg *m = msg.tryAs<StartLoadMsg>()) {
        doStartLoadFromLocalFile(m->archive);
    }
}

void BookLoader::doStartLoadFromLocalFile(const fs::path &archiveFile)
{
    ImgArchive archive;
    archive.load(archiveFile, [this](const QString &pagePath, const QPixmap &img) {
        const PageNum pageNum = getPageNumFromPagePath(pagePath);
        notify(std::make_unique<PageLoaded>(pageNum, img));
        return !stopped_;
    });
}

}
