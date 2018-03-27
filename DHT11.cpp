#include "DHT11.h"

DHT11::DHT11(int dhtPin, int captureDelay) {
  dht = dhtPin;
  this->captureDelay = captureDelay;
}

DHT11Data DHT11::get() {
    int pass = 0, fail = -1, errorTimeout = -2, invalidValue = -999, wakeup = 18, timeout = (F_CPU/40000);
    DHT11Data d11data;
    int bits[5];
    float humidity, temperature;

    int result = pass, mask = 128, idx = 0, bit = digitalPinToBitMask(dht), port = digitalPinToPort(dht);
    volatile unsigned char *PIR = portInputRegister(port);
    for(int i = 0; i < 5; i++) bits[i] = 0;

    pinMode(dht, OUTPUT);
    digitalWrite(dht, LOW);
    delay(wakeup);
    digitalWrite(dht, HIGH);
    delayMicroseconds(40);
    pinMode(dht, INPUT);
    
    long loopCntLOW = timeout;
    while((*PIR & bit) == LOW) if (--loopCntLOW == 0) result = errorTimeout;
    long loopCntHIGH = timeout;
    while((*PIR & bit) != LOW) if (--loopCntHIGH == 0) result = errorTimeout;
    
    for (int i = 40; i != 0; i--) {
        loopCntLOW = timeout;
        while((*PIR & bit) == LOW)  if(--loopCntLOW == 0) result = errorTimeout;
        long t = micros();
        loopCntHIGH = timeout;
        
        while((*PIR & bit) != LOW )  if (--loopCntHIGH == 0) result = errorTimeout;
        if((micros() - t) > 40) bits[idx] |= mask;
        mask >>= 1;
        if(mask == 0) {
            mask = 128;
            idx++;
        }
    }

    pinMode(dht, OUTPUT);
    digitalWrite(dht, HIGH);

    bool skipRest = false;
    if(result != pass) {
        d11data.data[0] = invalidValue;
        d11data.data[1] = invalidValue;
        d11data.result = result;
        skipRest = true;
    }

    if(!skipRest) {
      humidity = bits[0];
      temperature = bits[2];
  
      int sum = bits[0] + bits[2];
      if(bits[4] != sum) result = fail;
  
      d11data.data[0] = humidity;
      d11data.data[1] = temperature;
      d11data.result = pass;
    }

    delay(captureDelay);
 
    return d11data;
}

float DHT11::humidity(DHT11Data dd) {
  return dd.data[0];
}

float DHT11::temperature(DHT11Data dd) {
  return dd.data[1];
}

const char *DHT11::requirements() {
  return "Digital Pin, 5V, Ground";
}