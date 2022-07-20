#include "prop-repo.h"

#include <sstream>


namespace myapp
{

static std::string mkCreateTableSql(const std::string &repo)
{
    std::ostringstream ss;
    ss << "create table if not exists " << repo << " ";
    ss << "(key text primary key, val text not null);";
    return ss.str();
}

static std::string mkSelectSql(const std::string &repo)
{
    std::ostringstream ss;
    ss << "select val from " << repo << " ";
    ss << "where key = ?;";
    return ss.str();
}

static std::string mkInsertOrReplaceSql(const std::string &repo)
{
    std::ostringstream ss;
    ss << "insert or replace into " << repo << " ";
    ss << "values (?,?);";
    return ss.str();
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

void PropRepo::close()
{
    stmtInsertOrReplace_.close();
    stmtSelect_.close();
}

bool PropRepo::getValue(const u8view &key, u8str &val) const
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

void PropRepo::setValue(const u8view &key, const u8str &val)
{
    stmtInsertOrReplace_.reset();
    stmtInsertOrReplace_.arg(key).arg(val);
    stmtInsertOrReplace_.execute();
}

void PropRepo::set(const u8view &key, const u8str &val)
{
    setValue(key, val);
}

void PropRepo::set(const u8view &key, bool val)
{
    setValue(key, val ? 1 : 0);
}

void PropRepo::set(const u8view &key, int val)
{
    setValue(key, val);
}

void PropRepo::set(const u8view &key, long val)
{
    setValue(key, val);
}

void PropRepo::set(const u8view &key, long long val)
{
    setValue(key, val);
}

bool PropRepo::has(const u8view &key) const
{
    stmtSelect_.reset();
    stmtSelect_.arg(key);
    return stmtSelect_.nextRow();
}

bool PropRepo::get(const u8view &key, bool &to) const
{
    int n = 0;
    if (getValue(key, n)) {
        to = (n != 0);
        return true;
    } else {
        return false;
    }
}

bool PropRepo::get(const u8view &key, int &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8view &key, long &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8view &key, long long &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8view &key, u8str &to) const
{
    return getValue(key, to);
}

bool PropRepo::get(const u8view &key, u32str &to) const
{
    return getValue(key, to);
}



}
