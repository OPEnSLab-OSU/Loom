#define STR_(x) #x
#define STR(x) STR_(x) //to concatenate a predefined number to a string literal, use STR(x)

//Delimits the FAMILY, DEVICE, and INSTANCE_NUM with '/' for use in OSC Bundles
#define PacketHeaderString STR(/) FAMILY STR(/) DEVICE STR(INSTANCE_NUM)
