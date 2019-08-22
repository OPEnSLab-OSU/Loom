//
// Created by Eli Winkelman on 8/14/19.
//

#ifndef UNTITLED_LOOMDATA_H
#define UNTITLED_LOOMDATA_H

#include "stdlib.h"
#include "type_traits"
#include <utility>
//=== get_type_index ==//
template <typename T, typename... Ts>
struct get_type_index_impl;

// base case
// type not found
template <typename T>
struct get_type_index_impl<T> : std::integral_constant<std::size_t, 0>
{};

// second case
// type is at the beginning
template <typename T, typename ... Tail>
struct get_type_index_impl<T, T, Tail...> : std::integral_constant<std::size_t, 1>
{};

// induction step
// type not found at the beginning
// the idea here is that if the type is at the beginning ^ will get called (No Head)
template <typename T, typename Head, typename... Tail>
struct get_type_index_impl<T, Head, Tail...> : std::integral_constant<std::size_t,
        get_type_index_impl<T, Tail...>::value == 0u ?
        0u :
        1 + get_type_index_impl<T, Tail...>::value>
{};

// Wrapper struct to allow running functions on a specific Var for it's underlying type
// Function is a function, Var is a weak class, Types is a weak_types object, and args is a list of argument types to call the function with
// a struct to store a list of weak types

template <typename ... Types>
struct weak_types {

};

template <typename T>
struct weak_type {
    // initialize
    constexpr weak_type(){}
};

// wraps type stuff
template <typename ... Types>
class type_id {
public:

    // make sure that the type is valid
    template <typename T>
    static constexpr bool valid(weak_type<T>) {
        return get_type_index_impl<T, Types...>::value != 0u;
    }

    //empty type_id
    constexpr type_id<Types...>() noexcept : value(0u){};

    //type_id initializer
    template <typename T>
    constexpr type_id<Types...>(weak_type<T>) noexcept : value(get_type_index_impl<T, Types...>::value){

    }

    bool operator==(type_id<Types...> other) {
        return value == other.value;
    }

    std::size_t value;
};

template <typename ... Types>
class weak;

template <typename ... Types>
class weak {

    type_id<Types ...> current_type;

    void* storage;

    template < template<typename Type, typename ... Ts> class Functor, typename ... Ts>
    class using_weak {
        template <typename T, typename ... Args>
        static auto call(weak<Types...>&& ptr, Args&&... args) -> decltype(Functor<T, Ts...>()(std::forward<weak<Types...>>(ptr).value(weak_type<T>{}), std::forward<Args>(args)...)) {
            return Functor<T, Ts...>()(std::forward<weak<Types...>>(ptr).value(weak_type<T>{}), std::forward<Args>(args)...);
        };

    public:
        // out of types, do nothing
        template <typename ... Args>
        static void with(weak_types<>, weak<Types...>&&, Args&&...) {};

        template <typename Head, typename ... Tail, typename ... Args>
        static void with(weak_types<Head, Tail...>, weak<Types...>&& ptr, Args&&... args){
            if (ptr.check(weak_type<Head>{})) {
                //the type is Head, execute function with value as head
                call<Head>(std::forward<weak<Types...>>(ptr), std::forward<Args>(args)...);
            }
            else {
                // keep going down the type list.
                with(weak_types<Tail...>{}, std::forward<weak<Types...>>(ptr), std::forward<Args>(args)...);
            }
        };
    };

public:

    template <typename ... Ts>
    weak(Ts...){
        // initialize with invalid type
        current_type = type_id<Types...>();
        storage = nullptr;
    };

    //// Constructor, Deconstructor, Copy, Move ////
    template <typename T>
    weak(T val) : weak() {
        //set the type_id
        current_type = type_id<Types...>(weak_type<T>());

        // if the type is valid, store the value.
        if (type_id<Types...>::valid(weak_type<T>())) {
            // initializer
            current_type = type_id<Types...>(weak_type<T>());

            storage = new typename std::decay<T>::type(val);
        };

    }

    /// Destructor
    ~weak(){


        // run functor on object
        run<destroy>(&storage);

    }

    /// Copy Constructor
    weak(weak<Types...>& ptr) {
        // run the copier

        ptr.template run<copy>(&storage, &current_type);
    }

    weak(weak<Types...>&& val) {
        // move semantics
    }


    /*template <typename T>
    weak<Types...>& operator=(const T& val) {
        if (std::is_same<weak<Types...>, typename std::decay<T>::type>::value) {
            // assign with underlying value


        }

    }*/

    weak<Types...>& operator=(weak<Types...>&& ptr) {

        // assign with underlying value
        // check for self assignment
        if (this == &ptr) {
            return *this;
        }

        ptr.template run<copy>(&storage, &current_type);

        return *this;

    }

    weak<Types...>& operator=(weak<Types...>& ptr) {

        // assign with underlying value
        // check for self assignment

        if (this == &ptr) {
            return *this;
        }

        ptr.template run<copy>(&storage, &current_type);

        return *this;
    }


    template <typename T>
    weak<Types...>& operator=(T&& val){

        // otherwise assign normally
        emplace(val);

        return *this;

    }

    ///// returns the underlying pointer if the type is correct. Otherwise returns a nullptr;
    template <typename T>
    T&& value(weak_type<T> type) {

        if (check(type)) {
            return std::move(*(T*)storage);
        }

        return std::move((T)0);
    };

    template <typename T>
    void emplace(T val) {
        if (type_id<Types...>::valid(weak_type<T>{})) {

            current_type = type_id<Types...>(weak_type<T>());

            if (storage != nullptr) {
                run<destroy>(&storage);
            }

            storage = new typename std::decay<T>::type(val);
        }
    }

    type_id<Types...> type() {
        return current_type;
    }

    template <typename Type>
    bool check(weak_type<Type> check_type) {
        return current_type == type_id<Types...>(weak_type<Type>{});
    }

    template <template<typename Type, typename ... Ts> class Functor, typename ... Ts, typename ... Args>
    void run(Args... args) {
        using_weak<Functor, Ts...>::with(weak_types<Types...>{}, std::move(*this), std::forward<Args>(args)...);
    };

    ///// Arithmetic Operators /////

    /// Addition
    /// Weak types

    /// rvalue
    weak<Types...> operator+ (weak<Types...>&& other) {
        weak<Types...> added;

        run<addTop>(&other, &added);

        return added;
    }

    /// lvalue
    weak<Types...> operator+ (weak<Types...>& other) {
        weak<Types...> added;

        run<addTop>(&other, &added);

        return added;
    }

    /// Arithmetic Type
    /// rvalue
    template <typename T>
    weak<Types...> operator+ (T&& val) {
        weak<Types...> added;

        run<addBottom, T>(val, &added);

        return added;
    }
    /// lvalue
    template <typename T>
    weak<Types...> operator+ (T& val) {
        weak<Types...> added;

        run<addBottom, T>(val, &added);

        return added;
    }

    /// Subtraction
    /// Weak Types
    weak<Types...> operator- (weak<Types...>& other){
        return operator+(-1*other);
    }

    /// Multiplication
    /// Weak Types
    weak<Types...> operator* (weak<Types...>& other) {
        return other;
    }

private:
    //// Functors to destroy, copy, move, assign without knowing the underlying value
    template <typename T>
    struct destroy {
        void operator() (T val, void** storage) {
            delete (T*)*storage;
            *storage = nullptr;
        }
    };

    template <typename T>
    struct copy {
        void operator() (T val, void** storage, type_id<Types...>* current_type) {
            // copy the underlying value
            *current_type = type_id<Types...>(weak_type<T>{});

            *storage = new T(val);

        }
    };

    /// Addition implementation
    template <typename T>
    struct addTop {
        void operator() (T val, weak<Types...>* other, weak<Types...>* adding) {
            other -> run<addBottom, T>(val, adding);
        }
    };

    template <typename T, typename V>
    struct addBottom {
        void operator() (T val, V val1, weak<Types...>* adding) {

            auto plus = val + val1;
            adding -> emplace(plus);
        }
    };

    /// Multiplication implementation
};

#endif //UNTITLED_LOOMDATA_H
