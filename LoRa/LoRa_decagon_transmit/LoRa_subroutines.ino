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
  while(i < data.count) {
    if (data.type[i] == D_GS3) {
      bndl->add(IDString "/Decagon" STR(i) "/VWC").add((float)data.readings[i][0]);
      bndl->add(IDString "/Decagon" STR(i) "/Temp").add((float)data.readings[i][1]);
      bndl->add(IDString "/Decagon" STR(i) "/ElecCond").add((float)data.readings[i][2]);
    }
    i++;
  }
}

