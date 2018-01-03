// Copyright 2018 Zorxx Software. All rights reserved.
// File: BH1750.cpp
// Brief: Library implementing support for BH1750 light intensity sensor 
#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

// Commands
#define BH1750_CMD_POWER_DOWN      0x00
#define BH1750_CMD_POWER_UP        0x01
#define BH1750_CMD_RESET           0x07
#define BH1750_CMD_CONT_HRES_MODE  0x10
#define BH1750_CMD_CONT_HRES2_MODE 0x11
#define BH1750_CMD_CONT_LRES_MODE  0x13
#define BH1750_CMD_ONE_HRES_MODE   0x20
#define BH1750_CMD_ONE_HRES2_MODE  0x21
#define BH1750_CMD_ONE_LRES_MODE   0x23

#define BH1750_RESET_DELAY         200 // ms
#define BH1750_POWER_UP_DELAY      5   // ms
#define BH1750_HRES_DELAY          180 // ms
#define BH1750_LRES_DELAY          24  // ms

#define BH1750_DATA_BYTES          2L

#define BH1750_SendCommand(addr, cmd) \
   Wire.beginTransmission(addr);      \
   Wire.write(cmd);                   \
   Wire.endTransmission();

BH1750::BH1750(unsigned char i2c_address, bool continuous,
  eBH1750Resolution resolution): m_address(i2c_address),
  m_continuous(continuous), m_resolution(resolution)
{
   switch(resolution)
   {
      case BH1750_RESOLUTION_FOUR_LUX:
         m_delay = BH1750_HRES_DELAY;
         m_mode = (m_continuous) ? BH1750_CMD_CONT_LRES_MODE : BH1750_CMD_ONE_LRES_MODE;
         break;

      case BH1750_RESOLUTION_ONE_LUX:
         m_delay = BH1750_HRES_DELAY;
         m_mode = (m_continuous) ? BH1750_CMD_CONT_HRES_MODE : BH1750_CMD_ONE_HRES_MODE;
         break;

      case BH1750_RESOLUTION_HALF_LUX:
      default:
         m_delay = BH1750_HRES_DELAY;
         m_mode = (m_continuous) ? BH1750_CMD_CONT_HRES2_MODE : BH1750_CMD_ONE_HRES2_MODE;
         break;
   }
}

void BH1750::begin()
{
   PowerUp();
   Reset();
   delay(BH1750_RESET_DELAY);

   // If continuous mode was selected, start the first measurement
   if(m_continuous)
   {
      BH1750_SendCommand(m_address, m_mode);
      delay(m_delay);
   }
   else
      PowerDown();
}

void BH1750::Reset()
{
   BH1750_SendCommand(m_address, BH1750_CMD_RESET);
}

void BH1750::PowerUp()
{
   BH1750_SendCommand(m_address, BH1750_CMD_POWER_UP);
}

void BH1750::PowerDown()
{
   BH1750_SendCommand(m_address, BH1750_CMD_POWER_UP);
}

bool BH1750::read(double *lux)
{
   double result;

   if(!m_continuous)
   {
      PowerUp();
      delay(BH1750_POWER_UP_DELAY);
      BH1750_SendCommand(m_address, m_mode);
      delay(m_delay);
   }
   else
   {
      // If continous mode was selected, take the most recent measurement
   }

   Wire.requestFrom(m_address, BH1750_DATA_BYTES);
   if(Wire.available() < BH1750_DATA_BYTES)
      return false;

   switch(m_resolution)
   {
      case BH1750_RESOLUTION_HALF_LUX:
      {
         unsigned char value;
         result = Wire.read() << 7;
         value = Wire.read();
         result += value >> 1;
         result += (value & 0x01) ? 0.5 : 0;
         break;
      }
      default:
         result = Wire.read() << 8;
         result += Wire.read();
         break;
   }

   if(lux != NULL)
      *lux = result / 1.2;

   if(!m_continuous)
      PowerDown(); 

   return true;
}
