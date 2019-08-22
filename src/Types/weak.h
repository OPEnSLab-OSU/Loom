//
// Created by Eli Winkelman on 8/20/19.
//

#ifndef WEAK_H
#define WEAK_H

#include "stdlib.h"
#include "type_traits"
#include <utility>
#include "strong_typedef.h"
#include "debug_assert.h"

struct precondition_error_handler
        : debug_assert::set_level<1>,
          debug_assert::default_handler
{};

inline void on_disabled_exception() noexcept
{
    struct handler : debug_assert::set_level<1>, debug_assert::default_handler
    {};
    DEBUG_UNREACHABLE(handler{}, "attempt to throw an exception but exceptions are disabled");
}

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

template <typename ... Types>
class weak;

template <typename ... Types>
class weak {

    class type_id : public strong_typedef<type_id, std::size_t>, comparison<type_id> {

    public:

        // make sure that the type is valid
        template <typename T>
        static constexpr bool valid(weak_type<T>) {
            return get_type_index_impl<T, Types...>::value != 0u;
        }

        //empty type_id
        constexpr type_id() noexcept : strong_typedef<type_id, std::size_t>(0u){};

        //type_id initializer
        template <typename T>
        constexpr type_id(weak_type<T>) noexcept : type_id(get_type_index_impl<T, Types...>::value){

        };

        explicit constexpr type_id(std::size_t value) : strong_typedef<type_id, std::size_t>(value)
        {};

    };

    type_id current_type;

    void* storage;

    template < template<typename Type, typename ... Ts> class Functor, typename ... Ts>
    class using_weak {
        template <typename T, typename ... Args>
        static auto call(weak<Types...>&& ptr, Args&&... args) -> decltype(Functor<T, Ts...>()(std::forward<weak<Types...>>(ptr).template value<T>(), std::forward<Args>(args)...)) {
            return Functor<T, Ts...>()(std::forward<weak<Types...>>(ptr).template value<T>(), std::forward<Args>(args)...);
        };

    public:
        // out of types, do nothing
        template <typename ... Args>
        static void with(weak_types<>, weak<Types...>&&, Args&&...) {};

        template <typename Head, typename ... Tail, typename ... Args>
        static void with(weak_types<Head, Tail...>, weak<Types...>&& ptr, Args&&... args){
            if (ptr.current_type == type_id(weak_type<Head>{})) {
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
        current_type = type_id();
        storage = nullptr;
    };

    //// Constructor, Deconstructor, Copy, Move ////
    template <typename T>
    weak(T val) : weak() {

        storage = nullptr;

        emplace(val);

    }

    /// Destructor
    ~weak(){
        // run functor on object
        if (storage != nullptr) {
            run<destroy>(&storage);
        }
    }

    /// Copy Constructor
    weak(weak<Types...>& ptr) {
        // run the copier
        ptr.template run<copy>(this);
    }

    weak(weak<Types...>&& val) {
        val.template run<copy>(this);
    }

    weak<Types...>& operator=(weak<Types...>&& ptr) {

        // assign with underlying value
        // check for self assignment
        if (this == &ptr) {
            return *this;
        }

        ptr.template run<copy>(this);

        return *this;

    }

    weak<Types...>& operator=(weak<Types...>& ptr) {

        // assign with underlying value
        // check for self assignment

        if (this == &ptr) {
            return *this;
        }

        ptr.template run<copy>(this);

        return *this;
    }

    template <typename T>
    weak<Types...>& operator=(T&& val){

        // otherwise assign normally
        emplace(val);

        return *this;

    }

    template <typename T>
    weak<Types...>& operator=(const T& ptr) {

        emplace(ptr);

        return *this;
    }



    template <typename T>
    void emplace(T val) {

        /// ensure that the type is valid at compile time
        static_assert(type_id::valid(weak_type<T>{}), "Cannot store with non-weak type.");

        current_type = type_id(weak_type<T>());

        if (storage != nullptr) {
            run<destroy>(&storage);
        }

        storage = new typename std::decay<T>::type(val);
    }

    const type_id& type() const noexcept
    {
        return current_type;
    }

    template <typename Type>
    bool isType() {
        return current_type == type_id(weak_type<Type>{});
    }

    template <template<typename Type, typename ... Ts> class Functor, typename ... Ts, typename ... Args>
    void run(Args... args) {
        using_weak<Functor, Ts...>::with(weak_types<Types...>{}, std::move(*this), std::forward<Args>(args)...);
    };

    template <typename T>
    T&& value() {
        check(weak_type<T>{});

        return std::move(*(T*)storage);
    };

private:

    ///// returns the underlying pointer if the type is correct. Otherwise returns a nullptr;

    template <typename Type>
    void check(weak_type<Type> check_type) {
        DEBUG_ASSERT(current_type == check_type, precondition_error_handler{}, "Not a valid type.") ;
    }

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
        void operator() (T val, weak<Types...>* thisWeak) {
            // copy the underlying value
            thisWeak -> emplace(val);
        }
    };


    template <typename T, typename V>
    struct cast {
        void operator() (T val, V* returnVal) {
            *returnVal = static_cast<V>(val);
        };
    };


public:

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
        return operator+(other*-1);
    }

    template <typename T>
    weak<Types...> operator- (T& other){
        return operator+(other*-1);
    }

    /// Multiplication
    /// Weak Types

    weak<Types...> operator* (weak<Types...>& other) {

        weak<Types...> multiplied;

        run<multTop>(&other, &multiplied);

        return multiplied;

    }

    weak<Types...> operator* (weak<Types...>&& other) {

        weak<Types...> multiplied;

        run<multTop>(&other, &multiplied);

        return multiplied;

    }

    /// Arithmetic Types
    template <typename T>
    weak<Types...> operator* (T& val) {
        weak<Types...> multiplied;
        run<multBottom, T>(val, &multiplied);
        return multiplied;
    }
    template <typename T>
    weak<Types...> operator* (T&& val) {
        weak<Types...> multiplied;
        run<multBottom, T>(val, &multiplied);
        return multiplied;
    }

    /// Division
    /// Weak Types
    weak<Types...> operator / (weak<Types...>& other) {
        weak<Types...> divided;
        other.run<divideTop>(this, &divided);

        return divided;
    }

    weak<Types...> operator / (weak<Types...>&& other) {
        weak<Types...> divided;
        other.run<divideTop>(this, &divided);

        return divided;
    }


    /// Arithmetic Types
    template <typename T>
    weak<Types...> operator / (T& val) {
        weak<Types...> divided;
        run<divideBottom, T>(val, &divided);

        return divided;
    }

    /// Arithmetic Types
    template <typename T>
    weak<Types...> operator / (T&& val) {
        weak<Types...> divided;
        run<divideBottom, T>(val, &divided);

        return divided;
    }

    bool operator < (weak<Types...>& other) {
        bool result;

        other.run<lessTop>(this, &result);

        return result;
    }

    bool operator < (weak<Types...>&& other) {
        bool result;

        other.run<lessTop>(this, &result);

        return result;
    }

    bool operator > (weak<Types...>& other) {
        return other.operator<(*this);
    }

    bool operator > (weak<Types...>&& other) {
        return other.operator<(*this);
    }


    bool operator <= (weak<Types...>& other){
        return !operator>(other);
    }

    bool operator <= (weak<Types...>&& other){
        return !operator>(other);
    }

    bool operator >= (weak<Types...>& other){
        return !operator<(other);
    }

    bool operator >= (weak<Types...>&& other){
        return !operator<(other);
    }

    weak<Types...>& operator +=(weak<Types...>& other) {
        *this = *this+other;
        return *this;
    };

    template <typename T>
    weak<Types...>& operator+=(T val) {
        *this=*this+val;
        return *this;
    }

    weak<Types...>& operator -=(weak<Types...>& other) {
        *this = *this-other;
        return *this;
    };

    template <typename T>
    weak<Types...>& operator-=(T val) {
        *this=*this-val;
        return *this;
    }


private:
    /// Addition implementation
    template <typename T>
    struct addTop {
        void operator() (T&& val, weak<Types...>* other, weak<Types...>* adding) {
            other -> run<addBottom, T>(val, adding);
        }
    };

    template <typename T, typename V>

    struct addBottom {
        void operator() (T&& val, V&& val1, weak<Types...>* adding) {
            auto plus = val + val1;
            adding -> emplace(plus);
        }
    };

    /// Multiplication implementation
    template <typename T>
    struct multTop {
        void operator() (T val, weak<Types...>* other, weak<Types...>* multiplying) {
            other -> run<multBottom, T>(val, multiplying);
        }
    };

    template <typename T, typename V>
    struct multBottom {
        void operator() (T val, V val1, weak<Types...>* multiplying) {
                auto mult = val * val1;
                multiplying -> emplace(mult);
          
        }
    };

    template <typename T>
    struct divideTop {
        void operator() (T val, weak<Types...>* numerator, weak<Types...>* dividing ) {
            numerator -> run<divideBottom, T>(val, dividing);
        }
    };

    template< typename  T, typename V>
    struct divideBottom {
        void operator() (T val, V val2, weak<Types...>* dividing) {

            auto divide = val/val2;
            dividing -> emplace(divide);

        }
    };

    template <typename T>
    struct lessTop {
        void operator() (T val, weak<Types...>* _this, bool* result) {
            _this->run<lessBottom, T>(val, result);
        }
    };

    template <typename T, typename V>
    struct lessBottom {
        void operator() (T val, V val2, bool* result) {
            *result = val2 > val;
        }
    };

};

#endif //WEAK_H
