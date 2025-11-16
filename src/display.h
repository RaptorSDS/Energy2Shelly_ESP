// display.h - ST7789/ST7735 Display support for Waveshare ESP32-S3 Geek
#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef ESP32_S3_GEEK_DISPLAY

#include <TFT_eSPI.h>
#include <Arduino.h>

// Display type selection
#define DISPLAY_TYPE_ST7789 1
#define DISPLAY_TYPE_ST7735 2
#ifndef DISPLAY_TYPE
#define DISPLAY_TYPE DISPLAY_TYPE_ST7789   // Default: ST7789, can be set to DISPLAY_TYPE_ST7735
#endif

#if DISPLAY_TYPE == DISPLAY_TYPE_ST7789
  #define DISPLAY_WIDTH 135
  #define DISPLAY_HEIGHT 240
  #define TFT_DRIVER ST7789_DRIVER
#elif DISPLAY_TYPE == DISPLAY_TYPE_ST7735
  #define DISPLAY_WIDTH 128
  #define DISPLAY_HEIGHT 160
  #define TFT_DRIVER ST7735_DRIVER
#else
  #error "Unknown display type defined!"
#endif

// Display object
TFT_eSPI tft = TFT_eSPI();

#define DISPLAY_ROTATION 0  // 0 = portrait, 1 = landscape
#define COLOR_BG TFT_BLACK
#define COLOR_TEXT TFT_WHITE
#define COLOR_HEADER TFT_CYAN
#define COLOR_VALUE TFT_GREEN
#define COLOR_LABEL TFT_YELLOW

void display_init() {
    tft.init();
    tft.setRotation(DISPLAY_ROTATION);
    tft.fillScreen(COLOR_BG);
    tft.setTextColor(COLOR_TEXT, COLOR_BG);
    tft.setTextSize(1);
    // Turn on backlight (GPIO7 for Geek board)
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);
    tft.setCursor(5, 5);
    tft.setTextColor(COLOR_HEADER);
    tft.println("Energy2Shelly");
    tft.setTextColor(COLOR_TEXT);
    tft.println("Initialisiere...");
}

void display_update(const char* time_str, const char* ip_str, double power_a, double power_b, double power_c) {
    tft.fillScreen(COLOR_BG);
    int y = 5;
    tft.setTextSize(1);
    tft.setTextColor(COLOR_HEADER);
    tft.setCursor(5, y);
    tft.println("Energy2Shelly");
    y += 15;
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("Zeit:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(50, y);
    tft.println(time_str);
    y += 15;
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("IP:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(30, y);
    tft.println(ip_str);
    y += 20;
    tft.drawLine(5, y, DISPLAY_WIDTH-5, y, COLOR_HEADER);
    y += 10;
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("L1:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(30, y);
    tft.print(power_a, 1); tft.print(" W");
    y += 15;
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("L2:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(30, y);
    tft.print(power_b, 1); tft.print(" W");
    y += 15;
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("L3:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(30, y);
    tft.print(power_c, 1); tft.print(" W");
    y += 20;
    double total = power_a + power_b + power_c;
    tft.drawLine(5, y, DISPLAY_WIDTH-5, y, COLOR_HEADER);
    y += 10;
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("Total:");
    tft.setTextColor(COLOR_VALUE);
    tft.setTextSize(1);
    tft.setCursor(50, y);
    tft.print(total, 1); tft.print(" W");
}

void display_show_ip(const char* ip_str) {
    tft.fillScreen(COLOR_BG);
    tft.setTextSize(1);
    tft.setTextColor(COLOR_HEADER);
    tft.setCursor(5, 5);
    tft.println("Energy2Shelly");
    tft.setCursor(5, 25);
    tft.setTextColor(COLOR_LABEL);
    tft.print("IP: ");
    tft.setTextColor(COLOR_VALUE);
    tft.println(ip_str);
    tft.setCursor(5, 45);
    tft.setTextColor(COLOR_TEXT);
    tft.println("Bereit!");
}

#endif // ESP32_S3_GEEK_DISPLAY

#endif // DISPLAY_H
