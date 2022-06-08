#include "page-loader.h"

#include <string>
#include <sstream>

#include "core/logger.h"
#include "engine/engine.h"

#include "img-archive.h"


namespace myapp {

using logger::gLogger;

PageLoader::PageLoader(const fs::path &archiveFile)
    : archiveFile_(archiveFile)
{
    setActorName("PageLoader");
}

PageLoader::~PageLoader()
{
    gLogger.i << "PageLoader destructing";
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

void PageLoader::onMessage(actor::Message &msg)
{
    if (LoadFromArchieMsg *m = msg) {
        handleLoadFromArchieMsg(msg.sender());
    }
}

void PageLoader::handleLoadFromArchieMsg(actor::WeakHandle peer)
{
    ImgArchive archive;
    archive.load(archiveFile_, [this, peer](const u8str &entryPath, const QPixmap &img) {
        sendTo(peer, std::make_unique<PageLoadedMsg>(entryPath, img));
        return !stopped_;
    });
}

}