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

void package_data(OSCBundle *bndl, struct SensorList data) {
  int i = 0;
  char buf[5];
  bndl->empty();
  OSCMessage *msg;
  if (data.count > 0) {
    msg = &(bndl->add(IDString));
  }
  while(i < data.count) {
    if (data.type[i] == D_GS3) {
      msg->add("IDtag").add("Decagon" STR(INSTANCE_NUM));
      msg->add("VWC").add((float)data.readings[i][0]);
      msg->add("Temp").add((float)data.readings[i][1]);
      msg->add("ElecCond").add((float)data.readings[i][2]);
    }
    i++;
  }
}

