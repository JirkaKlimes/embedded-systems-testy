
#define MAX_TIME 2000

void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
}

uint16_t time_on;
uint16_t time_off;

uint64_t prev_millis;

void loop()
{
    if (Serial.available())
    {
        String input = Serial.readStringUntil('\n');

        time_on = min(MAX_TIME, input.substring(0, input.lastIndexOf(' ')).toInt());
        time_off = min(MAX_TIME, input.substring(input.lastIndexOf(' ') + 1, input.length()).toInt());

        uint16_t period = time_on + time_off;
        float frequency = 1000.0 / period;
        float duty = 100.0 * (float)time_on / (float)period;

        Serial.println(String("Frequency: ") + frequency + String("Hz Duty: ") + duty + String("%"));
    }

    if (millis() - prev_millis > (digitalRead(LED_BUILTIN) ? time_on : time_off))
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        prev_millis = millis();
    }
}