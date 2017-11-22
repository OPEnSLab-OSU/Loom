#define STR_(x) #x
#define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)

#define IDString FAMILY DEVICE STR(INSTANCE_NUM) // C interprets subsequent string literals as concatenation: "/Loom" "/Ishield" "0" becomes "/Loom/Ishield0"
//more detail to come here
