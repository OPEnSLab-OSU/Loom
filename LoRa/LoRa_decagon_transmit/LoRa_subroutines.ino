void lora_setup(RH_RF95 *rf95, RHReliableDatagram *manager) {
#if DEBUG == 1
  Serial.println("Initializing manager...");
#endif
  if (!manager->init()) {
#if DEBUG == 1
    Serial.println("init failed");
#endif
  }

#if DEBUG == 1
  Serial.println("Setting Frequency...");
#endif
  if (!rf95->setFrequency(RF95_FREQ)) {
#if DEBUG == 1
    Serial.println("setFrequency failed");
#endif
    while (1);
  }

#if DEBUG == 1
  Serial.println("Setting power...");
#endif
  rf95->setTxPower(23, false);
}

void package_data(OSCBundle *bndl, struct SensorList data, int sensor_number) {
  bndl->empty();
  OSCMessage *msg;
  msg = &(bndl->add(IDString));
  if (data.type[sensor_number] == D_GS3) {
    char buf[30];
    String ID = String(IDString) + String("/Decagon") + String(sensor_number);
    ID.toCharArray(buf, 30);
    msg->add("IDtag").add(buf);
    msg->add("SensorType").add("GS3");
#ifdef RTC3231
    msg->add("Timestamp").add(TimeStamp);
#endif //RTC3231
    msg->add("BatVolt").add((float)measuredvbat);
    msg->add("VWC").add((float)data.readings[sensor_number][0]);
    msg->add("Temp").add((float)data.readings[sensor_number][1]);
    msg->add("ElecCond").add((float)data.readings[sensor_number][2]);
  }
  else if (data.type[sensor_number] == D_5TM) {
    msg->add("IDtag").add("Decagon" STR(INSTANCE_NUM));
    msg->add("Instance").add((int32_t)sensor_number);
    msg->add("SensorType").add("5TM");
#ifdef RTC3231
    msg->add("Timestamp").add(TimeStamp);
#endif //RTC3231
    msg->add("BatVolt").add((float)measuredvbat);
    msg->add("VWC").add((float)data.readings[sensor_number][0]);
    msg->add("Temp").add((float)data.readings[sensor_number][1]);
  }
}

