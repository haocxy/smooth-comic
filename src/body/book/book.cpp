#include "book.h"

#include <cstring>

#include "core/fs.h"
#include "core/logger.h"


using namespace logger::global_loggers;


namespace myapp {

Book::Book(Engine &engine, QObject *parent)
    : QObjectActor(parent)
    , engine_(engine)
    , loader_(std::make_unique<BookLoader>(engine)) {

    initSignalsAndSlots();



    loader_->startLoadFromLocalFile("D:/tmp/a.zip");
}

Book::~Book()
{
    loader_->stop();

    disconnect();

    engine_.asyncDeleter().post(std::make_unique<AsyncDeleter::AsyncDeleteMsg>(std::move(loader_)));
}

void Book::initSignalsAndSlots()
{
    connect(loader_.get(), &BookLoader::onPageLoaded, this, &Book::handleOnPageLoaded);
}

static std::u8string toU8String(const QString &qs)
{
    const std::string s = qs.toUtf8().toStdString();
    std::u8string u8s;
    u8s.resize(s.size());
    std::memcpy(u8s.data(), s.data(), s.size());
    return u8s;
}

void Book::handleOnPageLoaded(PageNum pageNum, const QImage &img)
{
    pageKeyToImg_[pageNum] = img;

    const fs::path outfile = QString("D:/tmp/save-by-class-Book/%1.bmp").arg(pageNum).toStdU32String();

    fs::create_directories(outfile.parent_path());

    img.save(QString::fromStdU32String(outfile.generic_u32string()), "BMP");

    sendTo(engine_.testActor(), std::make_unique<TestActor::SumRequest>(pageNum, 1), [pageNum](TestActor::SumRequest::Response &rsp) {
        logInfo << "test actor by sum, " << pageNum << " + 1 = " << rsp.result;
    });
}

}
