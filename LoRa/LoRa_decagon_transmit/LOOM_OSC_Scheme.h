#define STR_(x) #x
#define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)

// C interprets subsequent string literals as concatenation: "Loom" "Ishield" "0" becomes "LoomIshield0"
#define IDString FAMILY DEVICE STR(INSTANCE_NUM) 

//Defines the string used to address packets to and from this device
#define PacketHeaderString FAMILY DEVICE STR(INSTANCE_NUM)
