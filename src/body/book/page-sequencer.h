#pragma once

#include "util/signal.hpp"

#include "page-info.h"


namespace myapp {

class PageSequencer {
public:
    using CbPageOrdered = void(sptr<PageInfo> page);

    Signal<CbPageOrdered> sigPageOrdered;

    void feed(sptr<PageInfo> page) {
        if (page) {
            doFeed(page->seqNum, page);
        }
    }

private:

    void doFeed(PageNum seqNum, sptr<PageInfo> page) {
        unorderedPages_[seqNum] = page;

        while (!unorderedPages_.empty()) {
            PageNum firstUnorderedSeq = unorderedPages_.begin()->first;
            sptr<PageInfo> firstUnorderedPage = unorderedPages_.begin()->second;
            if (firstUnorderedSeq == 0) {
                lastOrderedSeqNum_ = firstUnorderedSeq;
                unorderedPages_.erase(unorderedPages_.begin());
                if (firstUnorderedPage) {
                    sigPageOrdered(firstUnorderedPage);
                }
            } else {
                if (!lastOrderedSeqNum_) {
                    break;
                } else {
                    if (*lastOrderedSeqNum_ + 1 == firstUnorderedSeq) {
                        lastOrderedSeqNum_ = firstUnorderedSeq;
                        unorderedPages_.erase(unorderedPages_.begin());
                        if (firstUnorderedPage) {
                            sigPageOrdered(firstUnorderedPage);
                        }
                    } else {
                        break;
                    }
                }
            }
        }
    }



private:
    opt<PageNum> lastOrderedSeqNum_;
    std::map<PageNum, sptr<PageInfo>> unorderedPages_;
};

}
