//
// Created by Eli Winkelman on 8/15/19.
//

#ifndef UNTITLED_DYNAMIC_CAST_H
#define UNTITLED_DYNAMIC_CAST_H

// MACROS TO MAKE IT SIMPLER TO CREATE A DYNAMIC CLASS

// start a dynamic class
#define DYNAMIC_CLASS public DynamicClass {  \
public: \
   virtual const int typeID() const { return id; } \
   static const int id; \
private:

//end a dynamic class
#define END_DYNAMIC_CLASS(classname, parent)\
};\
\
const int classname::id(getTypeId(parent::id, nextPrimeId()));

/**
 * Example Usage:
 * 
 * Base dynamic class =>
 * class myDynamicClass : DYNAMIC_CLASS
      int myInt;
      int mySecondInt;
      void myFunc(int arg1, int arg2){ 
          Serial.println(arg1);
          Serial.println(arg2); };

      template <typename T, typename V>
      void Foo(T i, V b) {};

   END_DYNAMIC_CLASS(myDynamicClass, DynamicClass)

   Child dynamic class =>
   
    class myChildDynamicClass : public myDynamicClass, DYNAMIC_CLASS
    public:
      int myThirdInt;
    END_DYNAMIC_CLASS(myChildDynamicClass, myDynamicClass)
 */

bool IsPrime(int number)
{
    if (number == 2 || number == 3)
        return true;

    if (number % 2 == 0 || number % 3 == 0)
        return false;

    int divisor = 6;
    while (divisor * divisor - 2 * divisor + 1 <= number)
    {

        if (number % (divisor - 1) == 0)
            return false;

        if (number % (divisor + 1) == 0)
            return false;

        divisor += 6;
    }
    return true;
}

int NextPrime(int a)
{
    while (!IsPrime(++a))
    { }
    return a;
}

int getTypeId(int parentTypeId, int currentPrimeID) {
    return parentTypeId * currentPrimeID;
}

int nextPrimeId() {
    static int current_prime = 2;

    current_prime = NextPrime(current_prime);
    return current_prime;
}

/**
 * Dynamic Class Base Class.
 */
class DynamicClass
{

public:
    virtual const int typeID() const { return id; }
    static const int id = 1;
};

template <class T, class V>
T* Dynamic_Cast(V* obj) {
    // check if the object is in the inheritance prime tree
    if (obj -> typeID() % T::id == 0) {
        //cast object
        
        return (T*)obj;
    }
    //otherwise return a null pointer
    return nullptr;
}

#endif //UNTITLED_DYNAMIC_CAST_H
