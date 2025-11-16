# Display Integration für Waveshare ESP32-S3 Geek

## Hardware-Konfiguration
- **Display**: ST7789 135x240 Pixel
- **Pins**:
  - GPIO8 → DC (Data/Command)
  - GPIO10 → CS (Chip Select)
  - GPIO12 → CLK (Clock)
  - GPIO11 → MOSI (Data)
  - GPIO9 → RST (Reset)
  - GPIO7 → BL (Backlight)

## Änderungen in main.cpp

### 1. Includes hinzufügen (nach den bestehenden Includes)
```cpp
#ifdef ESP32_S3_GEEK_DISPLAY
#include "display.h"
#include "time.h"
#endif
```

### 2. Globale Display-Variablen (nach den bestehenden Variablen)
```cpp
#ifdef ESP32_S3_GEEK_DISPLAY
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;  // CET = UTC+1
const int daylightOffset_sec = 3600;  // Sommerzeit (DST)
unsigned long lastDisplayUpdate = 0;
const unsigned long displayUpdateInterval = 1000;  // Display-Update alle 1 Sekunde
#endif
```

### 3. Zeit-Funktion hinzufügen (vor setup())
```cpp
#ifdef ESP32_S3_GEEK_DISPLAY
String getTimeString() {
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return "--:--:--";
  }
  char timeStr[9];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  return String(timeStr);
}
#endif
```

### 4. In setup() nach WiFi-Verbindung einfügen
Füge nach der Zeile `DEBUG_SERIAL.println(WiFi.localIP());` Folgendes ein:

```cpp
#ifdef ESP32_S3_GEEK_DISPLAY
  // Display initialisieren
  display_init();
  
  // NTP-Zeit konfigurieren
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  delay(2000);  // Warte auf NTP-Synchronisation
  
  // IP-Adresse auf Display anzeigen
  display_show_ip(WiFi.localIP().toString().c_str());
  delay(2000);
#endif
```

### 5. In loop() am Ende hinzufügen
Füge am Ende der `loop()` Funktion (vor der schließenden Klammer) ein:

```cpp
#ifdef ESP32_S3_GEEK_DISPLAY
  // Display regelmäßig aktualisieren
  unsigned long currentMillis_display = millis();
  if (currentMillis_display - lastDisplayUpdate >= displayUpdateInterval) {
    String timeStr = getTimeString();
    String ipStr = WiFi.localIP().toString();
    
    display_update(timeStr.c_str(), ipStr.c_str(),
                   PhasePower[0].power, 
                   PhasePower[1].power, 
                   PhasePower[2].power);
    
    lastDisplayUpdate = currentMillis_display;
  }
#endif
```

## Kompilierung

Für das Kompilieren mit Display-Unterstützung verwende:
```bash
pio run -e esp32-s3-geek-display
```

Oder zum Hochladen:
```bash
pio run -e esp32-s3-geek-display -t upload
```

## Display-Ausgabe

Das Display zeigt folgende Informationen:
- **Header**: "Energy2Shelly"
- **Uhrzeit**: Aktuelle Zeit im Format HH:MM:SS (von NTP-Server)
- **IP-Adresse**: Aktuelle Netzwerk-IP
- **L1 Power**: Leistung Phase 1 (a_act_power) in Watt
- **L2 Power**: Leistung Phase 2 (b_act_power) in Watt
- **L3 Power**: Leistung Phase 3 (c_act_power) in Watt
- **Total**: Gesamtleistung aller drei Phasen

## Zeitzone anpassen

Für andere Zeitzonen, ändere die Werte in main.cpp:
```cpp
const long gmtOffset_sec = 3600;      // Zeitverschiebung in Sekunden (3600 = UTC+1)
const int daylightOffset_sec = 3600;  // Sommerzeit-Offset (3600 = +1 Stunde)
```

Beispiele:
- **UTC**: `gmtOffset_sec = 0;`
- **UTC+2**: `gmtOffset_sec = 7200;`
- **UTC-5**: `gmtOffset_sec = -18000;`

## Schriftgröße

Die Display-Bibliothek verwendet `tft.setTextSize(1)` für 8px Schriftgröße (Standard).
Für größere oder kleinere Schriften kann der Wert in `display.h` angepasst werden:
- `setTextSize(1)` → 8px (Standard)
- `setTextSize(2)` → 16px
- `setTextSize(3)` → 24px

## Farben anpassen

In `display.h` können folgende Farben angepasst werden:
```cpp
#define COLOR_BG TFT_BLACK       // Hintergrundfarbe
#define COLOR_TEXT TFT_WHITE     // Textfarbe
#define COLOR_HEADER TFT_CYAN    // Header-Farbe
#define COLOR_VALUE TFT_GREEN    // Wert-Farbe
#define COLOR_LABEL TFT_YELLOW   // Label-Farbe
```

## Bekannte Probleme / Hinweise

1. Das Display-Update erfolgt jede Sekunde, um die Belastung niedrig zu halten
2. Bei ersten Start kann es 2-3 Sekunden dauern, bis die NTP-Zeit synchronisiert ist
3. Die TFT_eSPI-Bibliothek wird über die `build_flags` in `platformio.ini` konfiguriert
