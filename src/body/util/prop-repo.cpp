#include "prop-repo.h"

#include <format>


namespace myapp
{

static std::string mkCreateTableSql(const std::string &repo)
{
    return std::format("create table if not exists {} (key text primary key, val text not null);", repo);
}

static std::string mkSelectSql(const std::string &repo)
{
    return std::format("select val from {} where key = ?;", repo);
}

static std::string mkInsertOrReplaceSql(const std::string &repo)
{
    return std::format("insert or replace into {} values (?,?);", repo);
}

PropRepo::PropRepo()
{
}

PropRepo::PropRepo(sqlite::Database &db, const std::string &repo)
{
    open(db, repo);
}

PropRepo::~PropRepo()
{
}

void PropRepo::open(sqlite::Database &db, const std::string &repo)
{
    repo_ = repo;

    db.exec(mkCreateTableSql(repo));

    stmtSelect_.open(db, mkSelectSql(repo));

    stmtInsertOrReplace_.open(db, mkInsertOrReplaceSql(repo));
}

bool PropRepo::getValue(const u8str &key, u8str &val) const
{
    stmtSelect_.reset();
    stmtSelect_.arg(key);
    if (stmtSelect_.nextRow()) {
        stmtSelect_.getValue(0, val);
        return true;
    } else {
        return false;
    }
}

void PropRepo::setValue(const u8str &key, const u8str &val)
{
    stmtInsertOrReplace_.reset();
    stmtInsertOrReplace_.arg(key).arg(val);
    stmtInsertOrReplace_.execute();
}

void PropRepo::set(const u8str &key, const u8str &val)
{
    setValue(key, val);
}

void PropRepo::set(const u8str &key, bool val)
{
    setValue(key, val ? 1 : 0);
}

void PropRepo::set(const u8str &key, int val)
{
    setValue(key, val);
}

void PropRepo::set(const u8str &key, long val)
{
    setValue(key, val);
}

void PropRepo::set(const u8str &key, long long val)
{
    setValue(key, val);
}

bool PropRepo::get(const u8str &key, bool &to) const
{
    int n = 0;
    if (getValue(key, n)) {
        to = (n != 0);
        return true;
    } else {
        return false;
    }
}

bool PropRepo::get(const u8str &key, int &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8str &key, long &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8str &key, long long &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8str &key, u8str &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8str &key, u32str &to) const
{
    return getValue(key, to);
}



}
