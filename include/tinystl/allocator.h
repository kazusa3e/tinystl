#pragma once

#include <cstddef>

namespace tinystl {

template <typename T>
class allocator {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_reference = const T &;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    allocator() = default;
    allocator(const allocator &other) = default;
    allocator(allocator &&other) = default;
    allocator<T> &operator=(const allocator &other) = default;
    allocator<T> &operator=(allocator &&other) = default;

    pointer allocate(size_type n);
    void deallocate(pointer p, size_type n);
    template <typename... Args>
    void construct(pointer p, Args &&...args);
    void destroy(pointer p);
    pointer address(reference x) const noexcept;
    size_type max_size() const noexcept;
};

template <typename T>
typename allocator<T>::pointer allocator<T>::allocate(allocator<T>::size_type n) {
    if (n == 0) return nullptr;
    return static_cast<allocator<T>::pointer>(::operator new(n * sizeof(T)));
}

template <typename T>
void allocator<T>::deallocate(allocator<T>::pointer p, allocator<T>::size_type n) {
    ::operator delete(p);
}

template <typename T>
template <typename... Args>
void allocator<T>::construct(allocator<T>::pointer p, Args &&...args) {
    new (p) T(std::forward<Args>(args)...);
}

template <typename T>
void allocator<T>::destroy(allocator<T>::pointer p) {
    p->~T();
}

template <typename T>
typename allocator<T>::pointer allocator<T>::address(allocator<T>::reference x) const noexcept {
    return std::addressof(x);
}

template <typename T>
typename allocator<T>::size_type allocator<T>::max_size() const noexcept {
    return static_cast<allocator<T>::size_type>(-1) / sizeof(T);
}

}  // namespace tinystl