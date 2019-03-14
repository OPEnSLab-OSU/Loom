
#include "Loom_nRF.h"


/////////////////////////////////////////////////////////////////////
// --- CONSTRUCTOR ---
Loom_nRF::Loom_nRF( 	
		char*		module_name,
		uint16_t	max_message_len,
		
		// CommScope subnet_scope,
		bool		compress_messages,

		uint8_t		address,
		uint8_t		friend_address,

		uint8_t		data_rate,
		uint8_t		power_level,
		uint8_t		retry_count,
		uint16_t	retry_timeout,

		uint8_t		multicast_level	
	)
	: LoomCommPlat( module_name, max_message_len, compress_messages )
{

	// Create nRF manager objects
	radio	= new RF24(5,6);
	network	= new RF24Network(*radio);

	SPI.begin();
	radio->begin();

	// Set data rate
	this->data_rate = data_rate;
	print_module_label();
	Print("Setting nRF data rate to: ");
	switch (this->data_rate) {
		case 0: 
			Println("Default (1Mbps)");
			break; 
		case 1: 
			radio->setDataRate(RF24_250KBPS);
			Println("250 kbps");
			break;
		case 2: 
			radio->setDataRate(RF24_1MBPS);
			Println("1 Mbps");
			break;
		case 3: 
			radio->setDataRate(RF24_2MBPS);
			Println("2 Mbps");
			break;
	}

	// Set power level
	this->power_level = power_level;
	print_module_label();
	Print("Setting nRF power level to: ");
	switch (this->power_level) {
		case 0: 
			Println("Default");
			break; 
		case 1: 
			radio->setPALevel(RF24_PA_MIN);
			Println("RF24_PA_MIN (-18dBm)");
			break;
		case 2: 
			radio->setPALevel(RF24_PA_LOW);
			Println("RF24_PA_LOW (-12dBm)");
			break;
		case 3: 
			radio->setPALevel(RF24_PA_HIGH);
			Println("RF24_PA_HIGH (-6dBM)");
			break;
		case 4: 
			radio->setPALevel(RF24_PA_MAX);
			Println("RF24_PA_MAX (0dBm)");
			break;
	}


	// Set retry behavior
	this->retry_count 	= retry_count;
	this->retry_timeout	= retry_timeout;
	if (  ( (retry_timeout >= 0) && (retry_timeout <= 15) ) &&
		  ( (retry_count >= 0) && (retry_count <= 15) )    ) {

		radio->setRetries(retry_timeout, retry_count);

		print_module_label();
		Println4("\t", "Retry delay: ", 250*retry_timeout, " microseconds");
		Println3("\t", "Retry count: ", retry_count);
	} 

	this->multicast_level = multicast_level;

	// Begin network manager with provided address
	network->begin(90, address); // determine good way to convert change to octal address?
}

/////////////////////////////////////////////////////////////////////
// --- DESTRUCTOR ---
Loom_nRF::~Loom_nRF() 
{
	delete network;
	delete radio;
}

/////////////////////////////////////////////////////////////////////
// --- PUBLIC METHODS ---
void Loom_nRF::print_config() 
{
	LoomCommPlat::print_config();

	Println3('\t', "Address             : ", address );
	Println3('\t', "Data Rate           : ", data_rate );
	Println3('\t', "Power Level         : ", power_level );
	Println3('\t', "Retry Count         : ", retry_count );
	Println3('\t', "Retry Timeout       : ", retry_timeout );
}

/////////////////////////////////////////////////////////////////////

bool Loom_nRF::receive_bundle(OSCBundle& bndl) 
{
	network->update();                      // Check the network regularly

	while ( network->available() ) {        // Is there anything ready for us?
		
		RF24NetworkHeader header;          // If so, grab it and print it out
		char buf[NRF_MESSAGE_SIZE];
		memset(buf, '\0', NRF_MESSAGE_SIZE);
		network->read(header, &buf, NRF_MESSAGE_SIZE-1);

		// Println2("Compressed buf :", buf);

		// This is done just in case the compressed string
		// uncompresses to more than 251 characters
		char larger_buf[180];
		memset(larger_buf, '\0', sizeof(larger_buf));
		strcpy(larger_buf, (const char*)buf);

		LoomCommPlat::convert_string_to_bundle((char*)larger_buf, bndl); 
		
		// Apply filtering based on family and subnet
		bool in_scope = LoomCommPlat::scope_filter(bndl);
		if (print_verbosity == Verbosity::V_HIGH) {
			if (!in_scope) {
				Println("Received nRF bundle out of scope");
			}
		}
	} // of while ( network->available() )
}

/////////////////////////////////////////////////////////////////////
bool Loom_nRF::send_bundle(OSCBundle& bndl, uint16_t destination) 
{
	char message[NRF_MESSAGE_SIZE];
	memset(message, '\0', NRF_MESSAGE_SIZE);
	
	LoomCommPlat::convert_bundle_to_string(bndl, message);
	
	RF24NetworkHeader header(destination);									

	bool is_sent = network->write( header, message, strlen(message) );

	Println2( "Send nRF bundle " , (is_sent) ? "successful" : "failed" );

	return is_sent;
}

/////////////////////////////////////////////////////////////////////
bool Loom_nRF::send_bundle(OSCBundle& bndl) 
{
	send_bundle(bndl, friend_address);
}

/////////////////////////////////////////////////////////////////////
void Loom_nRF::broadcast_bundle(OSCBundle& bndl) 
{
	char message[NRF_MESSAGE_SIZE];
	memset(message, '\0', NRF_MESSAGE_SIZE);
	
	LoomCommPlat::convert_bundle_to_string(bndl, message);
	
	RF24NetworkHeader header(00);									

	bool is_sent = network->multicast( header, message, strlen(message), multicast_level );

	Println2("Multicast nRF bundle " , (is_sent) ? "successful" : "failed" );

}

/////////////////////////////////////////////////////////////////////
void Loom_nRF::set_address(uint addr)    // Need to test this
{ 
	address = addr;
	delete network;

	network = new RF24Network(*radio);
	network->begin(90, address);
}

/////////////////////////////////////////////////////////////////////
uint Loom_nRF::get_address() 
{ 
	return address; 
}

/////////////////////////////////////////////////////////////////////
void Loom_nRF::set_friend_address(uint addr) 
{ 
	friend_address = addr; 
}

/////////////////////////////////////////////////////////////////////
uint Loom_nRF::get_friend_address() 
{ 
	return friend_address; 
}

/////////////////////////////////////////////////////////////////////
void Loom_nRF::set_multicast_level(uint8_t level)
{
	multicast_level = level;
}

/////////////////////////////////////////////////////////////////////
uint8_t Loom_nRF::get_multicast_level()
{
	return multicast_level;
}



