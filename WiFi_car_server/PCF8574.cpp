#include "PCF8574.h"

PCF8574::PCF8574(byte scl, byte sda) {
  Wire.begin(scl, sda);
  clearAll();
}
PCF8574::PCF8574(byte scl, byte sda, byte addr): m_addr(addr)
{
  Wire.begin(scl, sda);
  clearAll();
}

void PCF8574::clearAll() {
  Wire.beginTransmission(m_addr);
  Wire.write(m_data);
  Wire.endTransmission();
}
void PCF8574::digitalWrite(byte pin, byte state) {
  if (state == HIGH)
    m_data |= 1 << pin;
  else
    m_data &= ~(1 << pin);

  Wire.beginTransmission(m_addr);
  Wire.write(m_data);
  Wire.endTransmission();
}

