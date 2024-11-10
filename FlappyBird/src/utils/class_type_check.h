#pragma once
class ClassTypeCheck {
public:
    template<typename T, typename T2>
    static bool is_class_type(T2* obj) {
        return dynamic_cast<T*>(obj) == nullptr;
    }
};

