
#ifdef is_analog // Don't even include in compile if not in use

// Generic subroutine for reading raw sensor data with averaging
uint32_t read_analog(uint8_t chnl)
{
    int i = num_measurements;
    uint32_t reading = 0;
    while(i--)
      reading += analogRead(chnl);
#if (num_measurements == 1) //// Take a reading    
    return (reading);
#endif
#if (num_measurements == 2) //// Take a reading    
    // Then divide by 2 to get average sample value
    return (reading >> 1);
#endif
#if (num_measurements == 4) //// Take a reading    
    // Then divide by 4 to get average sample value
    return (reading >> 2);
#endif
#if (num_measurements == 8) //// Take a reading    
    // Then divide by 8 to get average sample value
    return (reading >> 3);
#endif

}

void measure_analog(void)
{
  char addressString[255];
  // declare the bundle
    OSCBundle bndl;
#if (is_analog > 0) //// Take a reading
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/port0");
    bndl.add(addressString).add((int32_t)read_analog(0));  
#endif 
#if (is_analog > 1) 
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/port1");
    bndl.add(addressString).add((int32_t)read_analog(1));
#endif 
#if (is_analog > 2)
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/port2"); 
    bndl.add(addressString).add((int32_t)read_analog(2));
#endif 
#ifdef transmit_butt
    sprintf(addressString,"%s%s",configuration.packet_header_string,"/butt");
    bndl.add(addressString).add((int32_t)digitalRead(transmit_butt));
#endif

 // UDP Packet
    Udp.beginPacket(configuration.ip_broadcast, 9436);
      bndl.send(Udp); // send the bytes to the SLIP stream
    Udp.endPacket(); // mark the end of the OSC Packet
   
   // empty the bundle to free room for a new one
    bndl.empty(); 
}

#endif // end is_analog test
