#include "DynamicClassExamples.h"

void setup() {

    Serial.begin(9600);
    while (!Serial);
    myDynamicClass* p = new myChildDynamicClass();
   
    //this will cast properly
    myChildDynamicClass* cast = Dynamic_Cast<myChildDynamicClass>(p);
    if (cast != nullptr) {
      Serial.println("Cast!");
      Serial.println(cast -> myThirdInt);
      cast -> myFunc(1, 2);
    
      Serial.println(sizeof(*cast));
    }

    //this will not
    myChildDynamicClass2* cast2 = Dynamic_Cast<myChildDynamicClass2>(p);
    if (cast2 != nullptr) {
      
      Serial.println(cast2 -> myThirdInt2);
      cast2 -> myFunc(1, 2);
    
      Serial.println(sizeof(*cast2));
    }
    else {
      Serial.println("Not Cast!");
    }
    
    // but be careful, you can still try to access things and it may not return an error.
    cast2 -> Bar();
}

void loop() {
  // put your main code here, to run repeatedly:
}
