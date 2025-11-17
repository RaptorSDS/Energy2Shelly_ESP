// display.h - Mit Custom Font
#ifndef DISPLAY_H
#define DISPLAY_H

#ifdef ESP32_S3_GEEK_DISPLAY

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST7735.h>
#include <SPI.h>
#include <Fonts/FreeSans9pt7b.h>   // 12px hoch
#include <Fonts/FreeSans12pt7b.h>  // 16px hoch
#include <Fonts/FreeSansBold12pt7b.h>  // Bold 16px

// Display Pins
#define TFT_CS    10
#define TFT_DC    8
#define TFT_RST   9
#define TFT_MOSI  11
#define TFT_SCLK  12
#define TFT_BL    7

// Display Type Selection
#ifndef DISPLAY_TYPE
#define DISPLAY_TYPE 1  // 1=ST7789, 2=ST7735
#endif

// Colors
#define COLOR_BG      ST77XX_BLACK
#define COLOR_TEXT    ST77XX_WHITE
#define COLOR_HEADER  ST77XX_CYAN
#define COLOR_VALUE   ST77XX_GREEN
#define COLOR_LABEL   ST77XX_YELLOW

// Display Objekt
#if DISPLAY_TYPE == 1
  Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
  #define DISPLAY_WIDTH 135
  #define DISPLAY_HEIGHT 240
#else
  Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
  #define DISPLAY_WIDTH 128
  #define DISPLAY_HEIGHT 160
#endif

void display_init() {
    Serial.println("Display: Init start...");
    
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, LOW);
    delay(100);
    
    SPI.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
    delay(100);
    
#if DISPLAY_TYPE == 1
    Serial.println("Display: ST7789 init...");
    tft.init(DISPLAY_WIDTH, DISPLAY_HEIGHT);
#else
    Serial.println("Display: ST7735 init...");
    tft.initR(INITR_BLACKTAB);
#endif
    
    delay(200);
    
    tft.setRotation(0);
    tft.fillScreen(COLOR_BG);
    
    digitalWrite(TFT_BL, HIGH);
    delay(100);
    
    // Test mit Custom Font
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(COLOR_HEADER);
    tft.setCursor(5, 15);  // Y-Position muss angepasst werden bei Custom Fonts!
    tft.println("Energy");
    tft.setCursor(5, 30);
    tft.println("2Shelly");
    
    tft.setFont();  // Zurück zum Standard-Font
    tft.setTextSize(1);
    tft.setTextColor(COLOR_TEXT);
    tft.setCursor(5, 45);
    tft.println("Display OK!");
    
    Serial.println("Display: Init complete!");
}

void display_update(const char* time_str, const char* ip_str, 
                    double power_a, double power_b, double power_c) {
    tft.fillScreen(COLOR_BG);
    
    // Header - Standard Font, klein
    tft.setFont();  // Standard Font
    tft.setTextSize(1);
    tft.setTextColor(COLOR_HEADER);
    tft.setCursor(2, 2);
    tft.print("E2S ");
    tft.setTextColor(COLOR_VALUE);
    tft.print(time_str);
    
    // IP - Standard Font
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(2, 12);
    tft.print(ip_str);
    
    // Linie
    tft.drawLine(0, 22, DISPLAY_WIDTH, 22, COLOR_HEADER);
    
    // Jetzt Custom Font für Leistungswerte
    tft.setFont(&FreeSans9pt7b);  // 12px Font
    
    int y = 40;  // Startposition (bei Custom Fonts ist Y die Baseline!)
    
    // L1
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("L1:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(40, y);
    if (power_a >= 0) {
        tft.print((int)power_a);
    } else {
        tft.print((int)power_a);  // Zeigt automatisch "-" an
    }
    tft.print("W");
    y += 22;
    
    // L2
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("L2:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(40, y);
    if (power_b >= 0) {
        tft.print((int)power_b);
    } else {
        tft.print((int)power_b);
    }
    tft.print("W");
    y += 22;
    
    // L3
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, y);
    tft.print("L3:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(40, y);
    if (power_c >= 0) {
        tft.print((int)power_c);
    } else {
        tft.print((int)power_c);
    }
    tft.print("W");
    y += 28;
    
    // Linie vor Total
    tft.drawLine(0, y-5, DISPLAY_WIDTH, y-5, COLOR_HEADER);
    
    // Total - größerer Font
    tft.setFont(&FreeSans12pt7b);  // 16px Font für Total
    double total = power_a + power_b + power_c;
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(5, y+15);
    tft.print((int)total);
    tft.print("W");
}

void display_show_ip(const char* ip_str) {
    tft.fillScreen(COLOR_BG);
    
    // Header mit Custom Font
    tft.setFont(&FreeSans12pt7b);
    tft.setTextColor(COLOR_HEADER);
    tft.setCursor(5, 25);
    tft.println("Energy");
    tft.setCursor(5, 50);
    tft.println("2Shelly");
    
    // IP mit Standard Font
    tft.setFont();
    tft.setTextSize(1);
    tft.setTextColor(COLOR_LABEL);
    tft.setCursor(5, 70);
    tft.print("IP:");
    tft.setTextColor(COLOR_VALUE);
    tft.setCursor(5, 82);
    tft.println(ip_str);
    
    tft.setTextColor(COLOR_TEXT);
    tft.setCursor(5, 100);
    tft.println("Bereit!");
}

#endif // ESP32_S3_GEEK_DISPLAY
#endif // DISPLAY_H
