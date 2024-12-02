#pragma once
#include <functional>
#include <vector>
#include <utility>
#include "action_base.h"

template<typename... Args>
class Action : public ActionBase {
public:
    using FuncType = std::function<void(Args...)>;

    Action() = default;

    // Add a callable to the collection
    template<typename Callable>
    void add(Callable&& func) {
        m_funcs.emplace_back(std::forward<Callable>(func));
    }

    // Remove all stored functions
    void clear() {
        m_funcs.clear();
    }

    // Invoke all stored functions
    void operator()(Args... args) const {
        for (const auto& func : m_funcs) {
            if (func) {
                func(std::forward<Args>(args)...);
            }
        }
    }

    // Check if there are any stored functions
    bool is_valid() const {
        return !m_funcs.empty();
    }

private:
    std::vector<FuncType> m_funcs;
};


