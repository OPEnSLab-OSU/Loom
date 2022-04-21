///////////////////////////////////////////////////////////////////////////////
///
/// @file		Loom_IntenertLTE.cpp
/// @brief		File for LTE implementation.
/// @author		Adam Kerr, based on work by Noah Koontz
/// @date		2020
/// @copyright	GNU General Public License v3.0
///
///////////////////////////////////////////////////////////////////////////////

#ifdef LOOM_INCLUDE_LTE

#include "InternetLTE.h"
#include "Trust_Anchors.h"
#include "Module_Factory.h"

using namespace Loom;

///////////////////////////////////////////////////////////////////////////////

#define SerialAT Serial1
TinyGsm modem(SerialAT);

///////////////////////////////////////////////////////////////////////////////
LTE::LTE(
    const char* apn,
    const char* user,
    const char* pass,
    const int analog_pin
  )
  : InternetPlat("LTE")
  , APN(apn)
  , gprsUser(user)
  , gprsPass(pass)
  , powerPin(analog_pin)
  , m_base_client(modem)
  , m_client(m_base_client, TAs, (size_t)TAs_NUM, A7, 1, SSLClient::SSL_INFO)
{

  //sets baud rate for SARA-R4 and restarts module
  SerialAT.begin(115200);
  //Uses Analog pin wired to Pin 5 on LTE shield to turn on the LTE shield.
  pinMode(powerPin, OUTPUT);
  power_up();


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
LTE::LTE(JsonArrayConst p)
  : LTE(EXPAND_ARRAY(p, 3) ) {}


///////////////////////////////////////////////////////////////////////////////
void LTE::connect()
{
  //Try to connect, attempt connection up to 5 times
  uint8_t attempt_count = 0;

  do {
      print_module_label();
      LPrint("Trying to connect to: ");
      LPrintln(APN);
      LMark;
      //Connects with username and password, even if not provided. TinyGSM handles
      if(modem.gprsConnect(APN, gprsUser, gprsPass)){
        print_module_label();
        LPrintln("Success");
      }
      else{
        print_module_label();
        LPrintln("Fail");
        delay(10000);
        LMark;
        attempt_count++;
      }


    } while(!modem.isGprsConnected() && attempt_count < 5);

    if(attempt_count == 5){
      print_module_label();
      LPrintln("Connection failed!");
      return;
    }
  print_state();
}


///////////////////////////////////////////////////////////////////////////////
void LTE::disconnect(){
  LMark;
  // tells the SARA-R4 to stop
  modem.gprsDisconnect();
  delay(200);
}


///////////////////////////////////////////////////////////////////////////////
bool LTE::is_connected() const
{
  LMark;
  return modem.isGprsConnected();
}


///////////////////////////////////////////////////////////////////////////////

InternetPlat::UDPPtr LTE::open_socket(const uint port)
{
  //Since the TinyGSM library currently does not support UDP socket functionality, this method will always return a null pointer and fail.
  	UDPPtr ptr = UDPPtr();
    LMark;
  	// use the object created to open a UDP socket
  	if (ptr && ptr->begin(port)) return std::move(ptr);
  	// return a nullptr if any of that failed
  	return UDPPtr();
}


///////////////////////////////////////////////////////////////////////////////
void LTE::print_config() const
{
  InternetPlat::print_config();
  LPrint("\tAPN:               :", APN, '\n');
}


///////////////////////////////////////////////////////////////////////////////
void LTE::print_state() const
{
  InternetPlat::print_state();
  LPrintln("\tConnected : ", (modem.isGprsConnected()) ? "True" : "False" );
  LPrintln("\tAPN : ", APN );
  LPrintln("\tSignal State : ", modem.getSignalQuality()); //Signal quality report
  LPrintln("\tIP Address : ", modem.localIP());
}

///////////////////////////////////////////////////////////////////////////////
void LTE::power_up()
{
  if(!is_connected()){
    print_module_label();
    LPrintln("Power up function");
    digitalWrite(powerPin, LOW);
    delay(5000);
    LMark;
    modem.restart();
    delay(5000);
  }
}

///////////////////////////////////////////////////////////////////////////////
void LTE::power_down()
{
  print_module_label();
  LPrintln("Power down function");
  LMark;
  modem.poweroff();
  digitalWrite(powerPin, HIGH);
  delay(5000);
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_LTE
