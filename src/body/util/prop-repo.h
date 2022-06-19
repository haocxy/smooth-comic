#pragma once

#include <chrono>

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

    void set(const u8view &key, const u8str &val);

    void set(const u8view &key, bool val);

    void set(const u8view &key, int val);

    void set(const u8view &key, long val);

    void set(const u8view &key, long long val);

    template <typename ClockT>
    void set(const u8view &key, const std::chrono::time_point<ClockT> &time)
    {
        set(key, time.time_since_epoch().count());
    }

    bool has(const u8view &key) const;

    class BadPropKey : public std::exception {
    public:
        BadPropKey(const u8view &key)
            : std::exception(std::format("prop key [{}] not exist",
                static_cast<std::string>(u8str(key))).c_str()) {}

        virtual ~BadPropKey() {}
    };

    class GetHelper {
    public:
        GetHelper(const u8view &key, const PropRepo &repo) : key_(key), repo_(repo) {}

        template <typename T>
        operator opt<T>() const {
            T to{};
            if (repo_.get(key_, to)) {
                return to;
            } else {
                return std::nullopt;
            }
        }

    private:
        const u8view &key_;
        const PropRepo &repo_;
    };

    GetHelper get(const u8view &key) const {
        return GetHelper(key, *this);
    }

    bool get(const u8view &key, bool &to) const;

    bool get(const u8view &key, int &to) const;

    bool get(const u8view &key, long &to) const;

    bool get(const u8view &key, long long &to) const;

    bool get(const u8view &key, u8str &to) const;

    bool get(const u8view &key, u32str &to) const;

    template <typename ClockT>
    bool get(const u8view &key, std::chrono::time_point<ClockT> &time) const
    {
        using Timepoint = std::chrono::time_point<ClockT>;
        using Duration = Timepoint::duration;
        typename Duration::rep count = 0;
        if (get(key, count)) {
            time = Timepoint(Duration(count));
            return true;
        } else {
            return false;
        }
    }

private:
    void setValue(const u8view &key, const u8str &val);

    bool getValue(const u8view &key, u8str &val) const;

    template <typename T>
    bool getValue(const u8view &key, T &val) const {
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
