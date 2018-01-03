// Copyright 2018 Zorxx Software. All rights reserved.
// File: BH1750.h
// Brief: Library implementing support for BH1750 light intensity sensor 
#ifndef BH1750_H
#define BH1750_H

typedef enum
{
   BH1750_RESOLUTION_HALF_LUX,
   BH1750_RESOLUTION_ONE_LUX,
   BH1750_RESOLUTION_FOUR_LUX
} eBH1750Resolution;

#define BH1750_I2C_ADDRESS_DEFAULT   0x23
#define BH1750_I2C_ADDRESS_ALTERNATE 0x5C

class BH1750
{
   public:
      BH1750(unsigned char i2c_address = BH1750_I2C_ADDRESS_DEFAULT,
        bool continuous = true,
        eBH1750Resolution resolution = BH1750_RESOLUTION_HALF_LUX);

      void begin();
      void Reset();
      void PowerUp();
      void PowerDown();
      bool read(double *lux);

   private:
      unsigned char m_address;
      bool m_continuous;
      eBH1750Resolution m_resolution;
      unsigned short m_delay;
      unsigned char m_mode;
};

#endif // BH1750_H
