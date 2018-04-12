
// Generic subroutine for reading raw sensor data with averaging
uint32_t read_analog(uint8_t chnl)
{
  int i = num_measurements;
  uint32_t reading = 0;
  
  while (i--) 
    reading += analogRead(chnl);
    
  #if (num_measurements == 1) // Take a reading    
    return (reading);
  #endif
  #if (num_measurements == 2) // Take a reading    
    return (reading >> 1);    // Then divide by 2 to get average sample value
  #endif
  #if (num_measurements == 4) // Take a reading    
    return (reading >> 2);    // Then divide by 4 to get average sample value
  #endif
  #if (num_measurements == 8) // Take a reading    
    return (reading >> 3);    // Then divide by 8 to get average sample value
  #endif
}



void measure_analog(void)
{
  char addressString[255];
  OSCBundle bndl; 

  // Get reading from relevant ports and the button if enabled          
  #if (is_analog > 0) 
    sprintf(addressString, "%s%s", configuration.packet_header_string, "/port0");
    bndl.add(addressString).add((int32_t)read_analog(0));
  #endif
  #if (is_analog > 1)
    sprintf(addressString, "%s%s", configuration.packet_header_string, "/port1");
    bndl.add(addressString).add((int32_t)read_analog(1));
  #endif
  #if (is_analog > 2)
    sprintf(addressString, "%s%s", configuration.packet_header_string, "/port2");
    bndl.add(addressString).add((int32_t)read_analog(2));
  #endif
  #ifdef button
    sprintf(addressString, "%s%s", configuration.packet_header_string, "/butt");
    bndl.add(addressString).add((int32_t)digitalRead(button));
  #endif

  // UDP Packet
  Udp.beginPacket(configuration.ip_broadcast, configuration.localPort);
  bndl.send(Udp);     // Send the bytes to the SLIP stream
  Udp.endPacket();    // Mark the end of the OSC Packet
  bndl.empty();       // Empty the bundle to free room for a new one
}


