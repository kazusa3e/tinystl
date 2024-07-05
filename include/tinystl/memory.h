#pragma once

#include <utility>

namespace tinystl {

template <typename T>
class default_deleter {
public:
    default_deleter() = default;
    void operator()(T *p) {
        ::delete (p);
    }
};

template <typename T, typename D = default_deleter<T>>
class unique_ptr {
public:
    using element_type = T;
    using pointer = T*;
    using reference = T&;

public:
    unique_ptr(const unique_ptr<T, D> &other) = delete;
    unique_ptr<T, D> &operator=(const unique_ptr<T, D> &other) = delete;

    unique_ptr() noexcept : el_(nullptr) {}
    explicit unique_ptr(pointer p) : el_(p) {}

    unique_ptr(unique_ptr<T, D> &&other) noexcept;
    unique_ptr<T, D> &operator=(unique_ptr<T, D> &&other);
    ~unique_ptr();
    pointer get() const noexcept;
    explicit operator bool() const noexcept;
    pointer release() noexcept;
    void reset(pointer p = pointer{}) noexcept;
    void swap(unique_ptr &x) noexcept;
    reference operator*() const;
    pointer operator->() const;

private:
    pointer el_;
};

template <typename T, typename D>
unique_ptr<T, D>::unique_ptr(unique_ptr<T, D> &&other) noexcept
    : el_(nullptr) {
    std::swap(this->el_, other.el_);
}

template <typename T, typename D>
unique_ptr<T, D> &unique_ptr<T, D>::operator=(unique_ptr<T, D> &&other) {
    std::swap(this->el_, other.el_);
    return *this;
}

template <typename T, typename D>
unique_ptr<T, D>::~unique_ptr() {
    if (this->el_ != nullptr) {
        D deleter;
        deleter(this->el_);
        this->el_ = nullptr;
    }
}

template <typename T, typename D>
T *unique_ptr<T, D>::get() const noexcept {
    return this->el_;
}

template <typename T, typename D>
unique_ptr<T, D>::operator bool() const noexcept {
    return this->el_ != nullptr;
}

template <typename T, typename D>
T *unique_ptr<T, D>::release() noexcept {
    T* ret = nullptr;
    std::swap(ret, this->el_);
    return ret;
}

template <typename T, typename D>
void unique_ptr<T, D>::reset(T* p) noexcept {
    if (this->el_ != nullptr) {
        D deleter;
        deleter(this->el_);
        this->el_ = nullptr;
    }
    this->el_ = p;
}

template <typename T, typename D>
void unique_ptr<T, D>::swap(unique_ptr<T, D> &x) noexcept {
    std::swap(this->el, x.el_);
}

template <typename T, typename D>
T &unique_ptr<T, D>::operator*() const {
    return *(this->el_);
}

template <typename T, typename D>
T *unique_ptr<T, D>::operator->() const {
    return this->el_;
}

template <typename T, typename D = default_deleter<T>, typename... Args>
unique_ptr<T, D> make_unique(Args&&... args) {
    return unique_ptr<T, D>(new T(std::forward<Args>(args)...));
}

}  // namespace tinystl