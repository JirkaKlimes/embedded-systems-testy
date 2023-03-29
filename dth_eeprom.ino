#include <EEPROM.h>
#include <DHT.h>

#define DHT_PIN 11

#define TEMP_MIN_ADDRESS 1
#define TEMP_MAX_ADDRESS 2
#define HUMIDITY_MIN_ADDRESS 3
#define HUMIDITY_MAX_ADDRESS 4

#define DELAY 2000

// leave uncommented to skip reading the eeprom data
#define INITIALIZE_EEPROM

DHT dht(DHT_PIN, DHT11);

int8_t temp_min;
int8_t temp_max;
int8_t temp_current;

uint8_t humidity_min;
uint8_t humidity_max;
uint8_t humidity_current;

void setup()
{
    Serial.begin(9600);
    dht.begin();

#ifdef INITIALIZE_EEPROM
    temp_min = 127;
    temp_max = -128;
    humidity_max = 0;
    humidity_min = 100;
    saveToEEPROM();
#else
    temp_min = EEPROM.read(TEMP_MIN_ADDRESS);
    temp_max = EEPROM.read(TEMP_MAX_ADDRESS);
    humidity_max = EEPROM.read(HUMIDITY_MAX_ADDRESS);
    humidity_min = EEPROM.read(HUMIDITY_MIN_ADDRESS);
#endif
}

void loop()
{
    readTemp();
    if (updateMinMax())
    {
        saveToEEPROM();
    }
    printReadings();
    delay(DELAY);
}

void readTemp()
{
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)){
        return;
    }

    temp_current = (int8_t)t;
    humidity_current = (uint8_t)h;
}

bool updateMinMax()
{
    bool changed = false;
    if (temp_current < temp_min)
    {
        temp_min = temp_current;
        changed = true;
    }
    else if (temp_current > temp_max)
    {
        temp_max = temp_current;
        changed = true;
    }

    if (humidity_current < humidity_min)
    {
        humidity_min = humidity_current;
        changed = true;
    }
    else if (humidity_current > humidity_max)
    {
        humidity_max = humidity_current;
        changed = true;
    }
    return changed;
}

void saveToEEPROM()
{
    EEPROM.write(TEMP_MAX_ADDRESS, temp_max);
    EEPROM.write(TEMP_MIN_ADDRESS, temp_min);
    EEPROM.write(HUMIDITY_MAX_ADDRESS, humidity_max);
    EEPROM.write(HUMIDITY_MIN_ADDRESS, humidity_min);
}

void printReadings()
{
    Serial.println(String("MIN: ") + temp_min + String("C AKT: ") + temp_current + String("C MAX: ") + temp_max + String("C"));
    Serial.println(String("MIN: ") + humidity_min + String("% AKT: ") + humidity_current + String("% MAX: ") + humidity_max + String("%"));
    Serial.println();
}