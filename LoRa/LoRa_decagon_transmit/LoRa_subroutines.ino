void lora_setup(RH_RF95 *rf95, RHReliableDatagram *manager) {
  Serial.println("Initializing manager...");
  if (!manager->init())
    Serial.println("init failed");

  Serial.println("Setting Frequency...");
  if (!rf95->setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  Serial.println("Setting power...");
  rf95->setTxPower(23, false);
}

void package_data(OSCBundle *bndl, struct SensorList data, int sensor_number) {
  bndl->empty();
  OSCMessage *msg;
  msg = &(bndl->add(IDString));
  if (data.type[sensor_number] == D_GS3) {
    msg->add("IDtag").add("Decagon" STR(INSTANCE_NUM));
    msg->add("Instance").add((int32_t)sensor_number);
    msg->add("Timestamp").add(TimeStamp);
    msg->add("BatVolt").add((float)measuredvbat);
    msg->add("VWC").add((float)data.readings[sensor_number][0]);
    msg->add("Temp").add((float)data.readings[sensor_number][1]);
    msg->add("ElecCond").add((float)data.readings[sensor_number][2]);
  }
}

