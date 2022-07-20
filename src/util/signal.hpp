#pragma once

#include <boost/signals2/signal.hpp>

#include "core/obj-handle.h"


namespace myapp {

template <typename F>
class Signal : public boost::signals2::signal<F> {
public:
    Signal() {}

    virtual ~Signal() {}

    template <typename OwnerT, typename StrandT>
    boost::signals2::connection connect(WeakHandle<OwnerT> handle, StrandT &strand, std::function<F> &&cb) {
        return connect([handle, &strand, cb = std::move(cb)](auto&& ...args) mutable {
            if (auto sharedPtr = handle.obj()) {
                strand.exec([cb, args...]{
                    cb(args...);
                });
            }
        });
    }

    template <typename OwnerT, typename StrandT>
    boost::signals2::connection connect(StrongHandle<OwnerT> &handle, StrandT &strand, std::function<F> &&cb) {
        return connect(handle.weak(), strand, std::move(cb));
    }

    boost::signals2::connection connect(std::function<F> &&cb) {
        return boost::signals2::signal<F>::connect(std::move(cb));
    }
};

}
