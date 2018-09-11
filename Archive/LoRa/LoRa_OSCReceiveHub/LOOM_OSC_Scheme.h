#define STR_(x) #x
#define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)

// C interprets subsequent string literals as concatenation: "Loom" "Ishield" "0" becomes "LoomIshield0"
#define IDString FAMILY DEVICE STR(INSTANCE_NUM) 

//Delimits the FAMILY, DEVICE, and INSTANCE_NUM with '/' for use in OSC Bundles
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE STR(INSTANCE_NUM) STR()
