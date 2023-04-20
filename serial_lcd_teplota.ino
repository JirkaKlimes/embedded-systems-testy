#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();
    update_lcd();
}

uint16_t min_temprature;
uint16_t max_temprature;
uint16_t current_temprature;
uint32_t num_values;

void loop()
{
    if (parse_serial_data())
    {
        update_lcd();
    }
}

bool parse_serial_data()
{
    if (Serial.available() <= 0)
    {
        return false;
    }

    String incoming_data = Serial.readStringUntil('\n');
    if (!incoming_data.startsWith("T:"))
    {
        return false;
    }

    int new_temprature = incoming_data.substring(2).toInt();
    if (new_temprature <= 0)
    {
        return false;
    }
    set_temps((uint16_t)new_temprature);
    return true;
}

void set_temps(uint16_t temprature)
{
    if (min_temprature == 0)
    {
        min_temprature = temprature;
    }
    if (min_temprature == 0)
    {
        min_temprature = temprature;
    }
    current_temprature = temprature;
    min_temprature = min(min_temprature, temprature);
    max_temprature = max(max_temprature, temprature);
    num_values++;
}

void update_lcd()
{
    lcd.clear();
    uint8_t cursor_pos = 0;
    lcd.setCursor(cursor_pos, 0);
    lcd.print(current_temprature + String(" C"));
    cursor_pos = 16 - ((uint8_t)log10(num_values) + 2);
    lcd.setCursor(cursor_pos, 0);
    lcd.print(num_values + String("x"));

    cursor_pos = 0;
    lcd.setCursor(cursor_pos, 1);
    lcd.print(min_temprature + String(" C"));
    cursor_pos = 16 - ((uint8_t)log10(max_temprature) + 3);
    lcd.setCursor(cursor_pos, 1);
    lcd.print(max_temprature + String(" C"));
}
