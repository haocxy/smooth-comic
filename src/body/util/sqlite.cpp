#include "sqlite.h"

#include <cstring>

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include "sqlite-3380500/sqlite3.h"

#include "core/logger.h"

namespace
{

using logger::gLogger;

class Sqlite3ErrorMsgDeleter {
public:
    Sqlite3ErrorMsgDeleter(char *&errmsg)
        : errmsg_(errmsg)
    {
        errmsg_ = nullptr;
    }

    ~Sqlite3ErrorMsgDeleter() {
        if (errmsg_) {
            sqlite3_free(errmsg_);
            errmsg_ = nullptr;
        }
    }

private:
    char *&errmsg_;
};

}


namespace sqlite
{

Database::Database()
{
}

Database::Database(const fs::path &path)
{
    open(path);
}

Database::~Database()
{
    try {
        if (isOpened()) {
            close();
        }
    }
    catch (const std::exception &e) {
        gLogger.e << "exception in sqlite3ns::Database::~Database(): " << e.what();
    }
    catch (...) {
        gLogger.e << "unknown exception in sqlite3ns::Database::~Database()";
    }
}

bool Database::isOpened() const
{
    return db_;
}

void Database::open(const fs::path &path)
{
    if (isOpened()) {
        close();
    }

    if (!fs::exists(path.parent_path())) {
        try {
            fs::create_directories(path.parent_path());
        }
        catch (const fs::filesystem_error &e) {
            std::ostringstream ss;
            ss << "cannot create directories for [" << path.parent_path().generic_string() << "] ";
            ss << "because [" << e.what() << "]";
            throw std::logic_error(ss.str());
        }
    }
    {
        // 确保数据库文件存在
        std::ofstream ofs(path, std::ios::binary | std::ios::app);
    }
    sqlite3 *db = nullptr;
    int n = sqlite3_open(reinterpret_cast<const char *>(path.generic_u8string().c_str()), &db);
    if (n != SQLITE_OK) {
        std::ostringstream ss;
        ss << "cannot open SQLite3 database by file path: [" << path.generic_string() << "] ";
        ss << "because [" << sqlite3_errstr(n) << "]";
        throw std::logic_error(ss.str());
    }

    path_ = path;
    db_ = db;
}

void Database::close()
{
    assertOpened();

    int n = sqlite3_close(db_);
    if (n != SQLITE_OK) {
        std::ostringstream ss;
        ss << "cannot close SQLite3 database on file [" << path_.generic_string() << "] ";
        ss << "because [" << sqlite3_errstr(n) << "]";
        throw std::logic_error(ss.str());
    }

    path_.clear();
    db_ = nullptr;
}

void Database::exec(const std::string_view &sql)
{
    assertOpened();

    char *errmsg = nullptr;
    Sqlite3ErrorMsgDeleter errmsgDeleter(errmsg);

    sqlite3_exec(db_, sql.data(), nullptr, nullptr, &errmsg);

    if (errmsg) {
        std::ostringstream ss;
        ss << "sqlite3ns::Database::exec() error: [" << errmsg << "]";
        throw std::logic_error(ss.str());
    }
}

void Database::assertOpened() const
{
    if (!isOpened()) {
        throw std::logic_error("sqlite3ns::Database::assertOpened(): db not opened");
    }
}


Statement::Statement()
{
}

Statement::~Statement()
{
    try {
        if (isOpened()) {
            close();
        }
    }
    catch (const std::exception &e) {
        gLogger.e << "sqlite::Statement::~Statement() " << "error: [" << e.what() << "]";
    }
    catch (...) {
        gLogger.e << "sqlite::Statement::~Statement() " << "meet unknown exception";
    }
}

void Statement::open(Database &db, const std::string_view &sql)
{
    if (sql.empty()) {
        throw std::logic_error("sqlite::Statement::open() failed because SQL in argument is empty");
    }

    if (isOpened()) {
        close();
    }

    db.assertOpened();

    sqlite3_stmt *stmt = nullptr;

    const int n = sqlite3_prepare_v2(db.db_, sql.data(), static_cast<int>(sql.size()), &stmt, nullptr);
    if (n != SQLITE_OK) {
        throwError("open", n);
    }

    stmt_ = stmt;
}

void Statement::reset()
{
    argBindIndex_ = 1;

    const int n = sqlite3_reset(stmt_);
    if (n != SQLITE_OK) {
        throwError("reset", n);
    }
}

void Statement::bindNull(int pos)
{
    if (pos > 0) {
        const int n = sqlite3_bind_null(stmt_, pos);
        if (n != SQLITE_OK) {
            throwError("bindNull", n);
        }
    }
}

void Statement::bind(int pos, const char *cstr)
{
    if (pos > 0 && cstr) {
        const int n = sqlite3_bind_text(stmt_, pos, cstr, static_cast<int>(std::strlen(cstr)), SQLITE_TRANSIENT);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, const std::string_view &utf8str)
{
    if (pos > 0) {
        const int n = sqlite3_bind_text(stmt_, pos, utf8str.data(), static_cast<int>(utf8str.size()), SQLITE_TRANSIENT);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, const u8view &s)
{
    if (pos > 0) {
        const int n = sqlite3_bind_text(stmt_, pos, reinterpret_cast<const char *>(s.data()), static_cast<int>(s.size()), SQLITE_TRANSIENT);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, const u8str &s)
{
    bind(pos, u8view(s));
}

void Statement::bind(int pos, const u32str &s)
{
    bind(pos, std::u32string_view(s));
}

void Statement::bind(int pos, const std::u32string_view &utf32str)
{
    bind(pos, u8str(utf32str));
}

void Statement::bind(int pos, const void *data, size_t len)
{
    if (pos > 0 && data != nullptr && len > 0) {
        const int n = sqlite3_bind_blob(stmt_, pos, data, static_cast<int>(len), SQLITE_TRANSIENT);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, int value)
{
    if (pos > 0) {
        const int n = sqlite3_bind_int(stmt_, pos, value);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, long value)
{
    if (pos > 0) {
        const int n = sqlite3_bind_int64(stmt_, pos, value);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, long long value)
{
    if (pos > 0) {
        const int n = sqlite3_bind_int64(stmt_, pos, value);
        if (n != SQLITE_OK) {
            throwError("bind", n);
        }
    }
}

void Statement::bind(int pos, const fs::path &path)
{
    bind(pos, u8str(path.generic_u8string()));
}

void Statement::step()
{
    argBindIndex_ = 1;

    const int n = sqlite3_step(stmt_);
    // TODO step 返回值不是简单地非对即错，需要详细处理
}

bool Statement::nextRow()
{
    const int n = sqlite3_step(stmt_);

    if (n == SQLITE_ROW) {
        return true;
    } else if (n == SQLITE_DONE) {
        return false;
    } else {
        throwError("nextRow", n);
        return false;
    }
}

void Statement::getValue(int col, int &to)
{
    to = sqlite3_column_int(stmt_, col);
}

void Statement::getValue(int col, long &to)
{
    to = static_cast<long>(sqlite3_column_int64(stmt_, col));
}

void Statement::getValue(int col, long long &to)
{
    to = sqlite3_column_int64(stmt_, col);
}

void Statement::getValue(int col, MemBuff &to)
{
    to.resize(sqlite3_column_bytes(stmt_, col));
    to.write(0, sqlite3_column_blob(stmt_, col), to.size());
}

void Statement::getValue(int col, Buff &to)
{
    to.resize(sqlite3_column_bytes(stmt_, col));
    std::memcpy(to.data(), sqlite3_column_blob(stmt_, col), to.size());
}

void Statement::getValue(int col, std::string &utf8)
{
    size_t nbytes = sqlite3_column_bytes(stmt_, col);
    utf8.resize(nbytes);
    std::memcpy(utf8.data(), sqlite3_column_text(stmt_, col), nbytes);
}

void Statement::getValue(int col, u8str &to)
{
    size_t nbytes = sqlite3_column_bytes(stmt_, col);
    to.resize(nbytes);
    std::memcpy(to.data(), sqlite3_column_text(stmt_, col), nbytes);
}

void Statement::getValue(int col, std::u32string &to)
{
    u8str u8str;
    getValue(col, u8str);
    to = u8str;
}

void Statement::getValue(int col, fs::path &to)
{
    u8str str;
    getValue(col, str);
    const u32str u32s = u32str(str);
    to = static_cast<const std::u32string &>(u32s);
}

int64_t Statement::lastInsertRowId() const
{
    return sqlite3_last_insert_rowid(sqlite3_db_handle(stmt_));
}

void Statement::close()
{
    const int n = sqlite3_finalize(stmt_);
    if (n != SQLITE_OK) {
        std::ostringstream ss;
        ss << "sqlite::Statement::close() failed because: [" << sqlite3_errstr(n) << "]";
        throw std::logic_error(ss.str());
    }

    stmt_ = nullptr;
}

bool Statement::isOpened() const
{
    return stmt_;
}

void Statement::throwError(const char *func, int err)
{
    std::ostringstream ss;
    ss << "sqlite::Statement::" << func << "() failed because: [" << sqlite3_errstr(err) << "]";
    throw std::logic_error(ss.str());
}

}
