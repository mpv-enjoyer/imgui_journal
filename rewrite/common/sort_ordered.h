#pragma once
#include <vector>
#include <functional>

// Source - https://stackoverflow.com/a
// Posted by Konrad Rudolph
// Retrieved 2025-12-01, License - CC BY-SA 3.0
template <typename T>
std::vector<std::size_t> sort_ordered(std::vector<T> const& values, std::function<bool(std::size_t, std::size_t)> comp = nullptr) {
    std::vector<std::size_t> indices(values.size());
    std::iota(std::begin(indices), std::end(indices), static_cast<std::size_t>(0));
    if (!comp) comp = [&](std::size_t a, std::size_t b) { return values[a] < values[b]; };
    std::sort(std::begin(indices), std::end(indices), comp);
    return indices;
}
