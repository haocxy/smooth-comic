#pragma once

#include <cstddef>
#include <cstdint>

#include "core/basetype.h"
#include "core/membuff.h"
#include "core/fs.h"


struct sqlite3;
struct sqlite3_stmt;

namespace sqlite
{

class Database {
public:
	Database();

	Database(const fs::path &path);

	~Database();

	Database(const Database &) = delete;

	Database(Database &&) = delete;

	Database &operator=(const Database &) = delete;

	Database &operator=(Database &&) = delete;

    bool isOpened() const;

    const fs::path &path() const {
        return path_;
    }

	void open(const fs::path &path);

	void close();

    void exec(const std::string_view &sql);

private:
    void assertOpened() const;

private:
	fs::path path_;
	sqlite3 *db_ = nullptr;

    friend class Statement;
};


class Statement {
public:
    Statement();

    Statement(Database &db, const std::string_view &sql) {
        open(db, sql);
    }

    Statement(const Statement &) = delete;

    Statement(Statement &&) = delete;

    Statement &operator=(const Statement &) = delete;

    Statement &operator=(Statement &&) = delete;

    ~Statement();

    void open(Database &db, const std::string_view &sql);

    void reset();

    void bindNull(int pos);

    Statement &arg() {
        bindNull(argBindIndex_++);
        return *this;
    }

    void bind(int pos, const char *cstr);

    void bind(int pos, const std::string_view &utf8str);

    void bind(int pos, const u8view &s);

    void bind(int pos, const u8str &s);

    void bind(int pos, const std::u32string_view &utf32str);

    void bind(int pos, const void *data, size_t len);

    void bind(int pos, const MemBuff &data) {
        bind(pos, data.data(), data.size());
    }

    void bind(int pos, int value);

    void bind(int pos, long value);

    void bind(int pos, long long value);

    void bind(int pos, const fs::path &path);

    template <typename... Args>
    Statement &arg(Args&&... args) {
        bind(argBindIndex_++, std::forward<Args>(args)...);
        return *this;
    }

    void step();

    void execute() {
        step();
    }

    bool nextRow();

    void getValue(int col, int &to);

    void getValue(int col, long &to);

    void getValue(int col, long long &to);

    void getValue(int col, MemBuff &to);

    void getValue(int col, std::string &utf8);

    void getValue(int col, u8str &to);

    void getValue(int col, std::u32string &to);

    void getValue(int col, fs::path &to);

    int64_t lastInsertRowId() const;

    void close();

private:
    bool isOpened() const;

    static void throwError(const char *func, int err);

private:
    sqlite3_stmt *stmt_ = nullptr;
    int argBindIndex_ = 1;
};

}
