//
// Created by Eli Winkelman on 8/15/19.
//

#ifndef UNTITLED_DYNAMICCLASSEXAMPLES_H
#define UNTITLED_DYNAMICCLASSEXAMPLES_H

#include "DynamicClass.h"
// example of creating a parent dynamic class using the DYNAMIC_CLASS and END_DYNAMIC_CLASS macro
class myDynamicClass : DYNAMIC_CLASS

public:
  int myInt;
  int mySecondInt;
  void myFunc(int arg1, int arg2){ 
    Serial.println(arg1);
    Serial.println(arg2); };

  template <typename T, typename V>
  void Foo(T i, V b) {};
END_DYNAMIC_CLASS(myDynamicClass, DynamicClass)

class myChildDynamicClass : public myDynamicClass, DYNAMIC_CLASS

public:
  int myThirdInt;

END_DYNAMIC_CLASS(myChildDynamicClass, myDynamicClass)

class myChildDynamicClass2 : public myDynamicClass, DYNAMIC_CLASS

public:
  int myThirdInt2;
  void Bar() {
    Serial.println("BAR");
  }
END_DYNAMIC_CLASS(myChildDynamicClass2, myDynamicClass)

#endif //UNTITLED_DYNAMICCLASSEXAMPLES_H
