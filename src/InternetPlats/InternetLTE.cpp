///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_IntenertLTE.cpp
/// @brief		File for Loom_LTE implementation.
/// @author		Adam Kerr, based on work by Noah Koontz
/// @date		2020
///
///
///////////////////////////////////////////////////////////////////////////////

#include "InternetLTE.h"
#define SerialAT Serial1
TinyGsm modem(SerialAT);


///////////////////////////////////////////////////////////////////////////////
Loom_LTE::Loom_LTE(
    LoomManager* manager,
    const char* apn,
    const char* user,
    const char* pass
  )
  : LoomInternetPlat(manager, "LTE", Type::LTE)
  , APN(apn)
  , gprsUser(user)
  , gprsPass(pass)
  , m_base_client(modem)
  , m_client(m_base_client, TAs, (size_t)TAs_NUM, A5, 1, SSLClient::SSL_INFO)
{
  //sets baud rate for SARA-R4 and restarts module
  SerialAT.begin(115200);
  modem.restart();

  String present = modem.getModemInfo();

  //Checks for the presence of LTE shield, else disable LTE module.
  //If you're confident your wiring is correct, try pressing the power button twice rapidly. Module has to be turned on, sometimes responsiveness is buggy.
  if(present == NULL){
    print_module_label();
    LPrintln("LTE Shield not present");
    return;
  }

  connect();
}


///////////////////////////////////////////////////////////////////////////////
Loom_LTE::Loom_LTE(LoomManager* manager, JsonArrayConst p)
  : Loom_LTE(manager, EXPAND_ARRAY(p, 3) ) {}


///////////////////////////////////////////////////////////////////////////////
void Loom_LTE::connect()
{
  //Try to connect, attempt connection up to 5 times
  uint8_t attempt_count = 0;

  do {
      LPrint("Trying to connect to: ");
      LPrintln(APN);
      //Connects with username and password, even if not provided. TinyGSM handles
      if(modem.gprsConnect(APN, gprsUser, gprsPass)){
        LPrintln("Success");
      }
      else{
        LPrintln("Fail");
        delay(10000);
        attempt_count++;
      }


    } while(!modem.isGprsConnected() && attempt_count < 5);

    if(attempt_count == 5){
      LPrintln("Connection failed!");
      return;
    }
  print_state();
}


///////////////////////////////////////////////////////////////////////////////
void Loom_LTE::disconnect(){
  // tells the SARA-R4 to stop
  modem.gprsDisconnect();
  delay(200);
}


///////////////////////////////////////////////////////////////////////////////
bool Loom_LTE::is_connected() const
{
  return modem.isGprsConnected();
}


///////////////////////////////////////////////////////////////////////////////

LoomInternetPlat::UDPPtr Loom_LTE::open_socket(const uint port)
{
  // create the unique pointer
  	UDPPtr ptr = UDPPtr();
  	// use the object created to open a UDP socket
  	if (ptr && ptr->begin(port)) return std::move(ptr);
  	// return a nullptr if any of that failed
  	return UDPPtr();
}


///////////////////////////////////////////////////////////////////////////////
void Loom_LTE::print_config() const
{
  LoomInternetPlat::print_config();
  LPrint("\tAPN:               :", APN, '\n');
}


///////////////////////////////////////////////////////////////////////////////
void Loom_LTE::print_state() const
{
  LoomInternetPlat::print_state();
  LPrintln("\tConnected : ", (modem.isGprsConnected()) ? "True" : "False" );
  LPrintln("\tAPN : ", APN );
  LPrintln("\tSignal State : ", modem.getSignalQuality()); //Signal quality report
  LPrintln("\tIP Address : ", modem.localIP());

}

///////////////////////////////////////////////////////////////////////////////