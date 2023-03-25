#include <Wire.h>


void setup()
{
  // Init Serial monitor
  Serial.begin(9600);

  // Init I2C library
  Wire.begin();

  // Wait Serial monitor to begin
  while(!Serial);
  Serial.println("I2C bus scan");
}

void scan_i2c_bus()
{
  byte err, i2c_slave_addr;
  const uint8_t i2c_max_addr = 128;
  uint8_t deviceCount = 0;

  for (uint8_t addr = 1; addr < i2c_max_addr; addr++)
  {
    Wire.beginTransmission(addr);
    err = Wire.endTransmission();

    if(err == 0)
    {
      Serial.print("I2C device found at adress : 0x");
      // Check if i2c slave adress is not in 2 bytes add 0 after 0x
      if(addr < 16)
      {
        Serial.print("I2C device found at adress : 0x");
      }
      Serial.println(addr,HEX);
      delay(500);
      deviceCount++;

    }

  }

  if(deviceCount == 0)
  {
    Serial.println("I2C device not found");
  }
  
  Serial.println("Done");

}

void loop()
{
  scan_i2c_bus();
}