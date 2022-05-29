#pragma once

#include <vector>
#include <string>

#include "page-num.h"


namespace myapp {

class PageKey {
private:
    using Parts = std::vector<PageNum>;

public:
    PageKey() {}

    PageKey(const std::u8string_view &path);

    PageKey(const PageKey &other) : parts_(other.parts_) {}

    PageKey(PageKey &&other) noexcept : parts_(std::move(other.parts_)) {}

    PageKey &operator=(const PageKey &other) {
        if (this != &other) {
            parts_ = other.parts_;
        }
        return *this;
    }

    PageKey &operator=(PageKey &&other) noexcept {
        if (this != &other) {
            parts_ = std::move(other.parts_);
        }
        return *this;
    }

    struct CmpAsc {
        bool operator()(const PageKey &keyA, const PageKey &keyB) const
        {
            using Size = Parts::size_type;
            using Index = Size;

            const Parts &partsA = keyA.parts_;
            const Parts &partsB = keyB.parts_;

            const Size sizeA = partsA.size();
            const Size sizeB = partsB.size();

            const Size sizeCommon = sizeA < sizeB ? sizeA : sizeB;

            for (Index i = 0; i < sizeCommon; ++i) {
                if (partsA[i] < partsB[i]) {
                    return true;
                }
                if (partsA[i] > partsB[i]) {
                    return false;
                }
            }

            // 按照通常理解，123-456 应该在 123-456-789 的前面
            // 因为额外的序号应该表示更具体的东西

            if (sizeA < sizeB) {
                return true;
            }

            if (sizeA > sizeB) {
                return false;
            }

            return false;
        }
    };

private:

    // 把文件名中的连续数字串提取出来
    // 考虑到可能由多个不连续的数字串，用 vector 存储
    Parts parts_;
};

}
