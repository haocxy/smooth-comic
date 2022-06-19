#pragma once

#include "util/signal.h"

#include "loaded-page.h"


namespace myapp {

class PageSequencer {
public:

    void feed(sptr<LoadedPage> page) {
        unorderedPages_[page->seqNum] = page;

        while (!unorderedPages_.empty()) {
            sptr<LoadedPage> firstUnordered = unorderedPages_.begin()->second;
            if (firstUnordered->seqNum == 0) {
                lastOrderedSeqNum_ = firstUnordered->seqNum;
                unorderedPages_.erase(unorderedPages_.begin());
                sigPageOrdered(firstUnordered);
            } else {
                if (!lastOrderedSeqNum_) {
                    break;
                } else {
                    if (*lastOrderedSeqNum_ + 1 == firstUnordered->seqNum) {
                        lastOrderedSeqNum_ = firstUnordered->seqNum;
                        unorderedPages_.erase(unorderedPages_.begin());
                        sigPageOrdered(firstUnordered);
                    } else {
                        break;
                    }
                }
            }
        }
    }

    using CbPageOrdered = void(sptr<LoadedPage> page);

    Signal<CbPageOrdered> sigPageOrdered;

private:
    opt<PageNum> lastOrderedSeqNum_;
    std::map<PageNum, sptr<LoadedPage>> unorderedPages_;
};

}
