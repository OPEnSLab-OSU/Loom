#include <K30_I2C.h>
#include <LiquidCrystal_I2C.h>

K30_I2C k30_i2c = K30_I2C(0x68);
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup() {
  // put your setup code here, to run once:
  lcd.init();
  lcd.backlight(); //turn on backlight
}

int co2 = 0;
int rc  = 1;
String co2Value = "uninit";
String ppmString = " ppm";
String line1;
String line2;
int loopTime = 3000;

void loop() {
  // put your main code here, to run repeatedly:
  rc = k30_i2c.readCO2(co2);
  if (rc == 0) {
    line1 = "CO2 level:";
    line2 = String(co2) + ppmString;
  } else {
    line1 = String("Failure to read sensor\n");
    line2 = String("Return code:  " + String(rc));
  }

  lcd.setCursor(0, 0); // set the cursor to column 0, line 0
  lcd.print(line1);

  lcd.setCursor(0, 1); // set the cursor to column 0, line 1
  lcd.print(line2);
  delay(loopTime);
  lcd.clear();

}

