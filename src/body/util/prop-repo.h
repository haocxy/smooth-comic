#pragma once

#include "core/fs.h"
#include "core/basetype.h"

#include "sqlite.h"


namespace myapp
{

// 属性仓库
// 为了提升性能和简化管理，把属性数据库中的某些相关性较强的属性放在一起
// 底层对应一个数据表
class PropRepo {
public:

    PropRepo();

    // db: 在哪个属性数据库中
    // name: 属性仓库的名称，在底层对应数据表的名称
    //
    // 构造时会调用 loadFromDb 加载数据
    PropRepo(sqlite::Database &db, const std::string &repo);

    // 析构时会调用 saveToDb 存储数据
    ~PropRepo();

    void open(sqlite::Database &db, const std::string &repo);

    void set(const u8str &key, const u8str &val);

    void set(const u8str &key, bool val);

    void set(const u8str &key, int val);

    void set(const u8str &key, long val);

    void set(const u8str &key, long long val);

    bool get(const u8str &key, bool &to) const;

    bool get(const u8str &key, int &to) const;

    bool get(const u8str &key, long &to) const;

    bool get(const u8str &key, long long &to) const;

    bool get(const u8str &key, u8str &to) const;

    bool get(const u8str &key, u32str &to) const;

private:
    void setValue(const u8str &key, const u8str &val);

    bool getValue(const u8str &key, u8str &val) const;

    template <typename T>
    bool getValue(const u8str &key, T &val) const {
        u8str u8val;
        if (getValue(key, u8val)) {
            val = static_cast<T>(u8val);
            return true;
        } else {
            return false;
        }
    }

private:
    std::string repo_;
    mutable sqlite::Statement stmtSelect_;
    mutable sqlite::Statement stmtInsertOrReplace_;
};

}
