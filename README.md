# BH1750
Arduino library for BH1750 light intensity sensor

### Example

``` c++
static BH1750 light(BH1750_I2C_ADDRESS_DEFAULT, true, BH1750_RESOLUTION_HALF_LUX);

void setup()
{
   Wire.begin();
   light.begin();

   Serial.begin(115200);
   while (!Serial);
   Serial.println("\nLight Intensity");
}


void loop()
{
   double value;

   if(light.read(&value))
   {
      Serial.print("Brightness: ");
      Serial.print(value);
      Serial.println(" lux");
   }
   else
      Serial.println("Read failed");

   delay(200);
}
```
