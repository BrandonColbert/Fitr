#ifndef dht11_h
#define dht11_h

#include <Arduino.h>

struct DHT11Data {
  float data[2];
  int result;
};

class DHT11 {
  private:
    int dht, captureDelay;
  public:
    DHT11(int, int);
    DHT11Data get();
    static float humidity(DHT11Data);
    static float temperature(DHT11Data);
    static const char *requirements();
};

#endif