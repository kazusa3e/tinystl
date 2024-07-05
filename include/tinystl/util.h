#pragma once

#include <string>
#include <type_traits>
#include <utility>

namespace tinystl {

template <typename T>
typename std::remove_reference<T>::type &&move(T &&value) noexcept {
    return static_cast<typename std::remove_reference<T>::type &&>(value);
}

}  // namespace tinystl
