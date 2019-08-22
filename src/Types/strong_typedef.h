//
// Created by Eli Winkelman on 8/21/19.
//

#ifndef UNTITLED_STRONG_TYPEDEF_H
#define UNTITLED_STRONG_TYPEDEF_H

#include "utility"

template <class Tag, typename T>
class strong_typedef
{
public:
    strong_typedef() : value_()
    {
    }

    explicit strong_typedef(const T& value) : value_(value)
    {
    }

    explicit strong_typedef(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value)
    : value_(std::move(value))
    {
    }

    explicit operator T&() noexcept
    {
        return value_;
    }

    explicit operator const T&() const noexcept
    {
        return value_;
    }

    friend void swap(strong_typedef& a, strong_typedef& b) noexcept
    {
        using std::swap;
        swap(static_cast<T&>(a), static_cast<T&>(b));
    }

private:
    T value_;
};

template <typename Tag, typename T>
T underlying_type_impl(strong_typedef<Tag, T>);

template <typename T>
using underlying_type = decltype(underlying_type_impl(std::declval<T>()));
#endif //UNTITLED_STRONG_TYPEDEF_H

template <class StrongTypedef>
struct comparison
{
    friend bool operator==(StrongTypedef& lhs, const StrongTypedef& rhs)
    {
        using type = underlying_type<StrongTypedef>;
        return static_cast<type&>(lhs) == static_cast<const type&>(rhs);

    }
};

