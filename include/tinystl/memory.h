#pragma once

#include <cstddef>
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
    using pointer = T *;
    using reference = T &;

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

template <typename T, typename D = default_deleter<T>, typename... Args>
unique_ptr<T, D> make_unique(Args &&...args) {
    return unique_ptr<T, D>(new T(std::forward<Args>(args)...));
}

template <typename T, typename D = default_deleter<T>>
class shared_ptr {
public:
    using element_type = T;

public:
    shared_ptr() : rc_(nullptr) {}
    explicit shared_ptr(T *p);
    shared_ptr(const shared_ptr<T, D> &other);
    shared_ptr<T, D> &operator=(const shared_ptr<T, D> &other);
    shared_ptr(shared_ptr<T, D> &&other);
    shared_ptr<T, D> &operator=(shared_ptr<T, D> &&other);
    ~shared_ptr();
    // TODO: copy from weak ptr constructor
    void swap(shared_ptr<T, D> &other) noexcept;
    void reset(T *p = nullptr);
    element_type *get() const noexcept;
    element_type &operator*() const noexcept;
    element_type *operator->() const noexcept;
    std::size_t use_count() const noexcept;
    bool unique() const noexcept;
    explicit operator bool() const noexcept;

private:
    struct reference_counter {
        T *resource;
        std::size_t ref_count;
    };

private:
    reference_counter *rc_;
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
    T *ret = nullptr;
    std::swap(ret, this->el_);
    return ret;
}

template <typename T, typename D>
void unique_ptr<T, D>::reset(T *p) noexcept {
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

template <typename T, typename D>
shared_ptr<T, D>::shared_ptr(T *p) {
    this->rc_ = new reference_counter{p, 1};
}

template <typename T, typename D>
shared_ptr<T, D>::shared_ptr(const shared_ptr<T, D> &other) {
    this->rc_ = other.rc_;
    this->rc_->ref_count += 1;
}

template <typename T, typename D>
shared_ptr<T, D> &shared_ptr<T, D>::operator=(const shared_ptr<T, D> &other) {
    D deleter;
    if (this->rc_ != nullptr) {
        if (this->rc_->ref_count == 1) {
            deleter(this->rc_->resource);
            delete this->rc_;
        } else {
            this->rc_->ref_count -= 1;
        }
    }
    this->rc_ = other.rc_;
    this->rc_->ref_count += 1;
    return *this;
}

template <typename T, typename D>
shared_ptr<T, D>::shared_ptr(shared_ptr<T, D> &&other) {
    this->rc_ = nullptr;
    std::swap(this->rc_, other.rc_);
}

template <typename T, typename D>
shared_ptr<T, D> &shared_ptr<T, D>::operator=(shared_ptr<T, D> &&other) {
    D deleter;
    if (this->rc_ != nullptr) {
        if (this->rc_->ref_count == 1) {
            deleter(this->rc_->resource);
            delete this->rc_;
        } else {
            this->rc_->ref_count -= 1;
        }
    }
    this->rc_ = other.rc_;
    other.rc_ = nullptr;
    return *this;
}

template <typename T, typename D>
shared_ptr<T, D>::~shared_ptr() {
    D deleter;
    if (this->rc_ != nullptr) {
        if (this->rc_->ref_count == 1) {
            deleter(this->rc_->resource);
            delete this->rc_;
        } else {
            this->rc_->ref_count -= 1;
        }
    }
}

template <typename T, typename D>
void shared_ptr<T, D>::swap(shared_ptr<T, D> &other) noexcept {
    std::swap(this->rc_, other.rc_);
}

template <typename T, typename D>
void shared_ptr<T, D>::reset(T *p) {
    D deleter;
    if (this->rc_ != nullptr) {
        if (this->rc_->ref_count == 1) {
            deleter(this->rc_->resource);
            delete this->rc_;
        } else {
            this->rc_->ref_count -= 1;
        }
    }
    this->rc_ = (p != nullptr) ? new reference_counter{p, 1} : nullptr;
}

template <typename T, typename D>
typename shared_ptr<T, D>::element_type *shared_ptr<T, D>::get() const noexcept {
    return (this->rc_ != nullptr) ? this->rc_->resource : nullptr;
}

template <typename T, typename D>
typename shared_ptr<T, D>::element_type &shared_ptr<T, D>::operator*() const noexcept {
    return *(this->rc_->resource);
}

template <typename T, typename D>
typename shared_ptr<T, D>::element_type *shared_ptr<T, D>::operator->() const noexcept {
    return this->rc_->resource;
}

template <typename T, typename D>
std::size_t shared_ptr<T, D>::use_count() const noexcept {
    return (this->rc_ != nullptr) ? this->rc_->ref_count : 0;
}

template <typename T, typename D>
bool shared_ptr<T, D>::unique() const noexcept {
    if (this->rc_ == nullptr) return false;
    return this->rc_->ref_count == 1;
}

template <typename T, typename D>
shared_ptr<T, D>::operator bool() const noexcept {
    if (this->rc_ == nullptr) return false;
    return this->rc_->resource != nullptr;
}

template <typename T, typename D = default_deleter<T>, typename... Args>
shared_ptr<T, D> make_shared(Args &&...args) {
    return shared_ptr<T, D>(new T(std::forward<Args>(args)...));
}

// TODO: weak ptr

}  // namespace tinystl