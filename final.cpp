#include <Arduino.h>
#include <SoftwareSerial.h>
#include <U8x8lib.h>
#include <U8g2lib.h>

#define CLOCK_PIN 7
#define DATA_PIN 6
#define CS_PIN 4
#define DC_PIN 5
#define RESET_PIN 3

#define loading_width 12
#define loading_height 20
static const unsigned char loading_bits[] U8X8_PROGMEM = {
    0xff, 0x0f, 0x03, 0x0c, 0xff, 0x0f, 0x02, 0x04, 0x02, 0x04, 0x22, 0x04,
    0x46, 0x06, 0x0c, 0x03, 0xd8, 0x01, 0xb0, 0x00, 0xb0, 0x00, 0x98, 0x01,
    0x4c, 0x03, 0x06, 0x06, 0x42, 0x04, 0xd2, 0x04, 0x2a, 0x05, 0xff, 0x0f,
    0x03, 0x0c, 0xff, 0x0f};

#define weather_width 30
#define weather_height 30
static const unsigned char weather_bits[] U8X8_PROGMEM = {
    0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
    0x18, 0x08, 0x04, 0x00, 0x30, 0x00, 0x06, 0x00, 0x20, 0x00, 0x02, 0x00,
    0x00, 0x3e, 0x00, 0x00, 0x80, 0x63, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
    0xc0, 0xc0, 0x3f, 0x00, 0xcf, 0xe0, 0x70, 0x00, 0xc0, 0x30, 0xc0, 0x00,
    0xc0, 0x18, 0x80, 0x01, 0x80, 0x19, 0x80, 0x01, 0x00, 0x09, 0x00, 0x03,
    0x00, 0x0c, 0x00, 0x0f, 0x30, 0x0f, 0x00, 0x18, 0x98, 0x01, 0x00, 0x30,
    0xc0, 0x00, 0x00, 0x30, 0xc0, 0x00, 0x00, 0x30, 0xc0, 0x00, 0x00, 0x30,
    0xc0, 0x00, 0x00, 0x10, 0x80, 0x01, 0x00, 0x1c, 0x00, 0xff, 0xff, 0x07,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x04, 0x06,
    0x00, 0x46, 0x26, 0x03, 0x00, 0x60, 0x30, 0x00, 0x00, 0x30, 0x10, 0x00};

#define news_width 25
#define news_height 20
static const unsigned char news_bits[] U8X8_PROGMEM = {
    0xf0, 0xff, 0xff, 0x01, 0x10, 0x00, 0x00, 0x01, 0x10, 0x00, 0x00, 0x01,
    0x90, 0x9f, 0x3f, 0x01, 0x9f, 0x10, 0x00, 0x01, 0x91, 0x10, 0x00, 0x01,
    0x91, 0x90, 0x3f, 0x01, 0x91, 0x10, 0x00, 0x01, 0x91, 0x10, 0x00, 0x01,
    0x91, 0x9f, 0x3f, 0x01, 0x11, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x01,
    0x91, 0xff, 0x3f, 0x01, 0x11, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x01,
    0x91, 0xff, 0x3f, 0x01, 0x11, 0x00, 0x00, 0x01, 0x11, 0x00, 0x00, 0x01,
    0x13, 0x00, 0x00, 0x01, 0xfe, 0xff, 0xff, 0x00};

SoftwareSerial esp8266(10, 11);
U8G2_PCD8544_84X48_F_4W_SW_SPI u8g2 = U8G2_PCD8544_84X48_F_4W_SW_SPI(U8G2_R0, CLOCK_PIN, DATA_PIN, CS_PIN, DC_PIN, RESET_PIN);

String data[9];

void setup()
{
    Serial.begin(115200);
    esp8266.begin(9600);
    u8g2.begin();
    u8g2.setContrast(135);
    u8g2.enableUTF8Print();

    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 2, loading_width, loading_height, loading_bits);
    u8g2.setFont(u8g2_font_profont17_tf);
    u8g2.drawStr(15, 12, "GETTING");
    u8g2.drawStr(15, 24, "DATA...");
    u8g2.setFont(u8g2_font_profont10_tf);
    u8g2.drawStr(0, 32, "ESP8266 + Arduino");
    u8g2.drawStr(5, 39, "Bedside Display");
    u8g2.drawStr(0, 46, "Shafeen & Manveer");
    u8g2.sendBuffer();

    while (esp8266.available() == 0)
    {
    }
    while (esp8266.available())
    {
        esp8266.readStringUntil('\n');
    }

    for (int i = 0; i < 9; i++)
    {
        while (esp8266.available() == 0)
        {
        }

        while (esp8266.available())
        {
            data[i] = esp8266.readStringUntil('\n');
        }
    }
    for (int i = 0; i < 9; i++)
    {
        Serial.println(data[i]);
    }
}

void loop()
{

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_profont10_tf);
    u8g2.drawStr(8, 6, "Data pulled @");
    u8g2.drawRFrame(0, 9, 84, 36, 10);
    u8g2.setFont(u8g2_font_logisoso28_tn);
    u8g2.drawStr(1, 40, data[0].c_str());
    u8g2.sendBuffer();
    delay(3500);

    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 0, weather_width, weather_height, weather_bits);
    u8g2.setFont(u8g2_font_profont10_tf);
    u8g2.drawStr(35, 15, data[1].c_str());
    u8g2.drawStr(2, 40, data[2].c_str());
    u8g2.drawStr(35, 25, data[3].c_str());
    u8g2.drawStr(58, 25, " C");
    u8g2.sendBuffer();
    delay(5000);

    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 0, news_width, news_height, news_bits);
    u8g2.setFont(u8g2_font_profont10_tf);
    for (int i = 1; i < 8; i++)
    {
        int x = i < 4 ? 27 : 2;
        int y = i < 4 ? i * 6 : i * 6 + 2;
        int length = i < 4 ? 11 : 16;
        //u8g2.drawStr(x, y, (const char *)data[4].substring((i - 1) * length, i * length).c_str());
        if (i == 4)
        {
            u8g2.drawStr(x, y, (const char *)data[4].substring((i - 1) * (length - 5), i * length).c_str());
        }
        else
        {
            u8g2.drawStr(x, y, (const char *)data[4].substring((i - 1) * length, i * length).c_str());
        }
    }
    u8g2.sendBuffer();
    delay(5000);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso28_tf);
    u8g2.drawStr(0, 40, data[6].c_str());
    u8g2.sendBuffer();
    delay(5000);

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_logisoso18_tf);
    u8g2.drawStr(0, 22, data[5]);
    u8g2.setFont(u8g2_font_open_iconic_arrow_4x_t);
    if (data[8].indexOf("-") == -1)
    {
        u8g2.drawGlyph(50, 28, 83);
    }
    else
    {
        u8g2.drawGlyph(50, 28, 80);
    }
    u8g2.setFont(u8g2_font_profont10_tf);
    u8g2.drawStr(0, 30, data[6]);
    u8g2.drawStr(55, 30, data[8].c_str());
    u8g2.drawStr(0, 38, data[7]);
    u8g2.sendBuffer();
    delay(5000);
}