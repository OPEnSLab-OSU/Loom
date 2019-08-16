//
// Created by Eli Winkelman on 8/14/19.
//

#ifndef UNTITLED_LOOMDATA_H
#define UNTITLED_LOOMDATA_H

#include "stdlib.h"
#include "type_traits"
#include "stdexcept"
#include "iostream"

#define LOOM_DATA_BINARY_OP(type, op) a.as<type>() op b.as<type>()
#define LOOM_DATA_AUTOCAST(type)  operator type() const { return as<type>(); }

    // loom graph Data Types
    enum type {t_int, t_float, t_none};

    // handy function to get the loom type from the actual type
    template <typename _T>
    type loomTypeOf() {
        if (std::is_same<int, _T>::value) {
            return t_int;
        }
        else if (std::is_same<float, _T>::value or std::is_same<double, _T>::value ) {
                return t_float;
        }
        return t_none;
    }

    // Loom var struct. Holds a pointer to either an int or a float with functionality to do basic arithmetic without knowing the underlying type.
    struct var {

        //TYPE
        enum type type_id;

        /// Constructor. Data type is infered from the data passed in.
        template <class T>
        var(T d)  {
            type_id = loomTypeOf<T>();
            if (type_id == t_none) {
                return;
            }
            data = new T(d);
        }

        /// Destructor
        ~var() {
            delete data;
            data = nullptr;
        }

        /// Copy Constructor
        var(const var& data1) {
            type_id = data1.type_id;
            if (type_id == t_int) {
                data = new int(data1.as<int>());
            }
            else if (type_id == t_float) {
                data = new float(data1.as<float>());
            }
        }

        ///===============================================///
        ///============= Operator Overloads =============///
        ///=============================================///

        /// Assignment Operator

        var& operator=(const var& data1) {
            type_id = data1.type_id;
            delete data;
            data = nullptr;
            if (type_id == t_int) {
                data = new int(data1.as<int>());
            }
            else if (type_id == t_float) {
                data = new float(data1.as<float>());
            }
            return *this;
        }

        /// Comparison Operator
        /// Note that this comparison operator doesn't ask if the underlying types are the same. i.e. 1 = 1.0f.
        bool operator==(const var& data1) {
            if (data1.type_id == t_int) {
                return equals<int>(data1.as<int>());
            }
            else if (data1.type_id == t_float) {
                return equals<float>(data1.as<float>());
            }
            return false;
        }

        bool operator < (const var& data1) {
            if (data1.type_id == t_int) {
                return lessThan(data1.as<int>());
            }
            else if (data1.type_id == t_float) {
                return lessThan(data1.as<float>());
            }
            return false;
        }

        bool operator > (var& data1) {
            return (data1 < *this);
        }

        bool operator >= (const var& data1) {
            return !(*this < data1);
        }

        bool operator <= (var& data1) {
            return !(*this > data1);
        }

        var& operator += (const var& data1) {
            *this = *this+data1;
            return *this;
        }

        var& operator -= (const var& data1) {
            *this = *this - data1;
            return *this;
        }

        var& operator++ () {
            if (type_id == t_int) {
                (*(int*)data)++;
            }
            else if (type_id == t_float) {
                (*(float*)data)++;
            }
            return *this;
        }

        var operator++ (int n) {
            var temp = *this;
            ++*this;
            return temp;
        }

        var& operator-- () {
            if (type_id == t_int) {
                (*(int*)data)--;
            }
            else if (type_id == t_float) {
                (*(float*)data)--;
            }
            return *this;
        }

        var operator-- (int n) {
            var temp = *this;
            --*this;
            return temp;
        }

        var operator +(const var& data1) {

            if (type_id == t_int and data1.type_id == t_int) {
                return add<int>(*this, data1);
            }
            else {
                return add<float>(*this, data1);
            }
        }

        var operator -(const var& data1) {
            if (type_id == t_int and data1.type_id == t_int) {
                return subtract<int>(*this, data1);
            }
            else {
                return subtract<float>(*this, data1);
            }
        }

        var operator /(const var&  data1) {
            if (type_id == t_int and data1.type_id == t_int) {
                return divide<int>(*this, data1);
            }
            else {
                return divide<float>(*this, data1);
            }
        }

        var operator *(const var& data1) {
            if (type_id == t_int and data1.type_id == t_int) {
                return multiply<int>(*this, data1);
            }
            else {
                return multiply<float>(*this, data1);
            }
        }

        ///======================================================///
        ///===================== Auto Cast =====================///
        ///====================================================///

        LOOM_DATA_AUTOCAST(int)
        LOOM_DATA_AUTOCAST(float)

        ///======================================================///
        ///============= Arithmetic Implementation =============///
        ///====================================================///

        template <typename T>
        bool lessThan(T val1) {
            if (type_id == t_int) {
                return as<int>() < val1;
            }
            else if (type_id == t_float) {
                return as<float>() < val1;
            }
            return false;
        }

        template <typename T>
        bool equals(T val1) {
            if (type_id == t_int) {
                return as<int>() == val1;
            }
            else if (type_id == t_float) {
                return as<float>() == val1;
            }
            return false;
        }

        template <typename _T>
        static var add(const var& a,const var& b) {
            return var(LOOM_DATA_BINARY_OP(_T, +));
        }

        template <typename _T>
        static var subtract(const var& a,const var& b) {
            return var(LOOM_DATA_BINARY_OP(_T, -));
        }

        template <typename _T>
        static var multiply(const var&  a, const var& b) {
            return var(LOOM_DATA_BINARY_OP(_T, *));
        }

        template <typename _T>
        static var divide(const var& a,const var&  b) {
            return var(LOOM_DATA_BINARY_OP(_T, /));
        }

        template <typename T>
        T as() const {
            if (loomTypeOf<int>() == type_id) {
                return (T)*((int *) data);

            } else if (loomTypeOf<float>() == type_id) {
                return (T)*((float *) data);

            }
            return (T)0;
        };

        template <typename T>
        void set(T value) {
            delete (T*)data;
            data = nullptr;
            type_id = loomTypeOf<T>();
            data = new T(value);
        }

        template <typename _T>
        bool is() {
            return loomTypeOf<_T>() == type_id;
        }

    private:
        void* data = nullptr;
    };


#endif //UNTITLED_LOOMDATA_H