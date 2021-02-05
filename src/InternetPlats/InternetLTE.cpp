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
  LMark;

  //sets baud rate for SARA-R4 and restarts module
  SerialAT.begin(115200);
  LMark;
  //Uses Analog pin wired to Pin 5 on LTE shield to turn on the LTE shield.
  pinMode(powerPin, OUTPUT);
  LMark;
  power_up();
  LMark;




  String present = modem.getModemInfo();
  LMark;

  //Checks for the presence of LTE shield, else disable LTE module.
  //If you're confident your wiring is correct, try pressing the power button twice rapidly. Module has to be turned on, sometimes responsiveness is buggy.
  if(present == NULL){
    LMark;
    print_module_label();
    LMark;
    LPrintln("LTE Shield not present");
    LMark;
    return;
  }

  connect();
  LMark;
}


///////////////////////////////////////////////////////////////////////////////
LTE::LTE(JsonArrayConst p)
  : LTE(EXPAND_ARRAY(p, 3) ) {}


///////////////////////////////////////////////////////////////////////////////
void LTE::connect()
{
  LMark;
  //Try to connect, attempt connection up to 5 times
  uint8_t attempt_count = 0;
  LMark;

  do {
      print_module_label();
      LMark;
      LPrint("Trying to connect to: ");
      LMark;
      LPrintln(APN);
      LMark;
      //Connects with username and password, even if not provided. TinyGSM handles
      if(modem.gprsConnect(APN, gprsUser, gprsPass)){
        print_module_label();
        LMark;
        LPrintln("Success");
        LMark;
      }
      else{
        print_module_label();
        LMark;
        LPrintln("Fail");
        LMark;
        delay(10000);
        LMark;
        attempt_count++;
        LMark;
      }


    } while(!modem.isGprsConnected() && attempt_count < 5);

    if(attempt_count == 5){
      print_module_label();
      LMark;
      LPrintln("Connection failed!");
      LMark;
      return;
    }
  print_state();
  LMark;
}


///////////////////////////////////////////////////////////////////////////////
void LTE::disconnect(){
  LMark;
  // tells the SARA-R4 to stop
  modem.gprsDisconnect();
  LMark;
  delay(200);
  LMark;
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
    LMark;
  //Since the TinyGSM library currently does not support UDP socket functionality, this method will always return a null pointer and fail.
  	UDPPtr ptr = UDPPtr();
    LMark;
  	// use the object created to open a UDP socket
  	if (ptr && ptr->begin(port)) return std::move(ptr);
    LMark;
  	// return a nullptr if any of that failed
  	return UDPPtr();
}


///////////////////////////////////////////////////////////////////////////////
void LTE::print_config() const
{
  LMark;
  InternetPlat::print_config();
  LMark;
  LPrint("\tAPN:               :", APN, '\n');
  LMark;
}


///////////////////////////////////////////////////////////////////////////////
void LTE::print_state() const
{
  LMark;
  InternetPlat::print_state();
  LMark;
  LPrintln("\tConnected : ", (modem.isGprsConnected()) ? "True" : "False" );
  LMark;
  LPrintln("\tAPN : ", APN );
  LMark;
  LPrintln("\tSignal State : ", modem.getSignalQuality()); //Signal quality report
  LMark;
  LPrintln("\tIP Address : ", modem.localIP());
  LMark;

}

///////////////////////////////////////////////////////////////////////////////
void LTE::power_up()
{
  LMark;
  if(!is_connected()){
    print_module_label();
    LMark;
    LPrintln("Power up function");
    LMark;
    digitalWrite(powerPin, LOW);
    LMark;
    delay(5000);
    LMark;
    modem.restart();
    LMark;
    delay(5000);
    LMark;
  }
}

///////////////////////////////////////////////////////////////////////////////
void LTE::power_down()
{
  print_module_label();
  LMark;
  LPrintln("Power down function");
  LMark;
  modem.poweroff();
  LMark;
  digitalWrite(powerPin, HIGH);
  LMark;
  delay(5000);
  LMark;
}

///////////////////////////////////////////////////////////////////////////////

#endif // ifdef LOOM_INCLUDE_LTE
