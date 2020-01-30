#include "SpoolKeyCache.h"

FlashStorage(flash_keys, SpoolKeys);
static constexpr unsigned long build_time = UNIX_TIMESTAMP_UTC;

static bool is_valid_cert_char(const char c) {
  return  c == ' '
          || c == '+'
          || c == '-'
          || c == '/'
          || (c >= '0' && c <= '9')
          || c == '='
          || (c >= 'A' && c <= 'Z')
          || (c >= 'a' && c <= 'z');
}

static bool is_valid_id_char(const char c) {
  return (c >= '0' && c <= '9')
    || (c >= 'A' && c <= 'Z')
    || (c >= 'a' && c <= 'z');
}

static void get_x509_input(char* dest, size_t max_size, const char* name) {
  bool valid = false;
  while(!valid) {
    Serial.print("Please input a ");
    Serial.print(name);
    Serial.println(" (copy and paste is fine). Escaped newlines (\\r or \\n) will automatically be replaced.");
    Serial.println("Press \';\' when done");
    uint16_t index = 0;
    while (true) {
      if (index >= max_size - 5) {
        Serial.println("Your input was too long! Please try again.");
        break;
      }
      if (Serial.available()) {
        const char c = Serial.read();
        // if we find a backslash, we need to replace it approprietly
        if (c == '\\') {
          while(!Serial.available());
          const char e = Serial.read();
          if (e == 'n') dest[index++] = '\n';
          else if (e == 'r') dest[index++] = '\r';
          else {
            Serial.print("Dropped unknown escape sequence: ");
            Serial.print(c);
            Serial.println(e);
          }
        }
        // else if we get a regular certificate character
        else if (is_valid_cert_char(c)) dest[index++] = c;
        else if (c == ';') {
          dest[index++] = '\r';
          dest[index++] = '\n';
          dest[index++] = '\0'; 
          valid = true;
          break;
        }
        else {
          Serial.print("Dropped unknown character: ");
          Serial.println(c);
        }
      }
    }
    // clear the read buffer
    while(Serial.available()) Serial.read();
    // do some extra verification
    if (valid) {
      Serial.println("Your input: ");
      Serial.println(dest);
      Serial.println("Is this correct? (y/N)");
      while(!Serial.available());
      const char c = Serial.read();
      if (c != 'y') valid = false;
      // clear the read buffer again
      while(Serial.available()) Serial.read();
    }
  }
}

static void get_regular_input(char* dest, size_t max_size, const char* name) {
  bool valid = false;
  while(!valid) {
    Serial.print("Please input a ");
    Serial.print(name);
    Serial.println(" (copy and paste is fine).");
    Serial.println("Press Enter when when done");
    uint16_t index = 0;
    while (true) {
      if (index >= max_size - 2) {
        Serial.println("Your input was too long! Please try again.");
        break;
      }
      if (Serial.available()) {
        const char c = Serial.read();
        // else if we get a regular certificate character
        if (is_valid_id_char(c)) dest[index++] = c;
        else if (c == '\r' || c == '\n') {
          dest[index++] = '\0'; 
          valid = true;
          break;
        }
        else {
          Serial.print("Dropped invalid character: ");
          Serial.println(c);
        }
      }
    }
    // clear the read buffer
    while(Serial.available()) Serial.read();
    // do some extra verification
    if (valid) {
      Serial.println("Your input: ");
      Serial.println(dest);
      Serial.println("Is this correct? (y/N)");
      while(!Serial.available());
      const char c = Serial.read();
      if (c != 'y') valid = false;
      // clear the read buffer again
      while(Serial.available()) Serial.read();
    }
  }
}

bool getKeys(SpoolKeys& keys) {
    flash_keys.read(&keys);

    if (!keys.valid || keys.timestamp < build_time) {
        // if the serial monitor is not open, return false, as we can't get the keys from there
        if (!Serial) return false;
        if (!keys.valid) Serial.println("Did not find a valid set of keys!");
        else Serial.println("The keys on this device are older than the build time, you must input new ones!");

        get_regular_input(keys.id, sizeof(keys.id), "device id");
        get_x509_input(keys.certificate, sizeof(keys.certificate), "client certificate");
        get_x509_input(keys.key, sizeof(keys.key), "client private key");
        keys.valid = true;
        keys.timestamp = build_time;

        flash_keys.write(keys);
    }

    return true;
} 