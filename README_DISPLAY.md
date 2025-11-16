# Energy2Shelly_ESP - Display Branch

## Überblick

Dieser Branch enthält Unterstützung für das **Waveshare ESP32-S3 Geek** mit **ST7789 Display** (135x240 Pixel).

## Features

- ✅ Alle Funktionen des Haupt-Branches (VDB-Integration)
- ✅ ST7789 Display-Unterstützung (135x240 Pixel)
- ✅ Anzeige von Echtzeit-Informationen:
  - Aktuelle Uhrzeit (via NTP)
  - IP-Adresse
  - Leistung pro Phase (L1, L2, L3)
  - Gesamtleistung
- ✅ 8px Font (klein und lesbar)
- ✅ Automatische Display-Updates (1x pro Sekunde)

## Hardware

### Waveshare ESP32-S3 Geek
- **MCU**: ESP32-S3
- **Display**: ST7789 135x240 Pixel
- **Display-Pins**:
  - GPIO8 → DC (Data/Command)
  - GPIO10 → CS (Chip Select)
  - GPIO12 → CLK (Clock/SCLK)
  - GPIO11 → MOSI (Data)
  - GPIO9 → RST (Reset)
  - GPIO7 → BL (Backlight)

## Installation

### Voraussetzungen
- PlatformIO (empfohlen) oder Arduino IDE
- USB-Kabel für ESP32-S3

### Schritte

1. **Repository klonen**:
   ```bash
   git clone https://github.com/RaptorSDS/Energy2Shelly_ESP.git
   cd Energy2Shelly_ESP
   git checkout esp32-s3-display
   ```

2. **Kompilieren und Hochladen** (PlatformIO):
   ```bash
   pio run -e esp32-s3-geek-display -t upload
   ```

3. **Konfiguration**:
   - Nach dem ersten Start erstellt das Gerät einen WiFi-Hotspot "Energy2Shelly"
   - Verbinde dich mit dem Hotspot und konfiguriere:
     - WiFi-Zugangsdaten
     - Datenquelle (MQTT, HTTP, VDB, SMA, etc.)
     - Je nach Datenquelle: Server-IP, Port, UUIDs, etc.

## main.cpp Anpassungen

Die `main.cpp` muss manuell angepasst werden. Siehe [DISPLAY_INTEGRATION.md](DISPLAY_INTEGRATION.md) für detaillierte Anweisungen.

**Wichtigste Änderungen**:
1. Display-Header einbinden
2. NTP-Zeit konfigurieren
3. Display in `setup()` initialisieren
4. Display in `loop()` aktualisieren

## Display-Ausgabe

```
+----------------------+
| Energy2Shelly        |
| Zeit: 18:30:45       |
| IP: 192.168.1.100    |
|----------------------|
| L1:    125.5 W       |
| L2:    230.8 W       |
| L3:    180.2 W       |
|----------------------|
| Total: 536.5 W       |
+----------------------+
```

## Konfiguration

### Zeitzone
Standardmäßig ist die Zeitzone auf **CET (UTC+1)** mit Sommerzeit eingestellt.
Zum Ändern in `main.cpp`:
```cpp
const long gmtOffset_sec = 3600;      // 3600 = UTC+1 (CET)
const int daylightOffset_sec = 3600;  // 3600 = +1h Sommerzeit
```

### Display Update-Rate
Standardmäßig: 1 Update pro Sekunde
Zum Ändern in `main.cpp`:
```cpp
const unsigned long displayUpdateInterval = 1000;  // in Millisekunden
```

### Farben
Farben können in `src/display.h` angepasst werden:
```cpp
#define COLOR_BG TFT_BLACK       // Hintergrund
#define COLOR_TEXT TFT_WHITE     // Standard-Text
#define COLOR_HEADER TFT_CYAN    // Header
#define COLOR_VALUE TFT_GREEN    // Werte
#define COLOR_LABEL TFT_YELLOW   // Labels
```

### Schriftgröße
Standard: `setTextSize(1)` = 8px
Zum Ändern in `src/display.h` die Werte bei `tft.setTextSize()` anpassen.

## Bibliotheken

Dieser Branch verwendet folgende zusätzliche Bibliothek:
- **TFT_eSPI** v2.5.43+ (für ST7789 Display)

Alle Bibliotheken werden automatisch von PlatformIO heruntergeladen.

## Datenquellen

Unterstützt alle Datenquellen des Hauptprojekts:
- ✅ **MQTT** - MQTT Broker
- ✅ **HTTP** - Generischer HTTP-JSON-Endpunkt
- ✅ **VDB** - Volkszaehler Database (VolkszaehlerDB)
- ✅ **SMA** - SMA Energy Meter/Home Manager (Multicast)
- ✅ **SHRDZM** - SHRDZM Smart Meter Interface (UDP)
- ✅ **SUNSPEC** - SUNSPEC Modbus TCP (z.B. Kostal Smart Energy Meter)

## Entwicklung

### Branch-Struktur
- **main**: Hauptbranch (Original)
- **VDB-integration**: VDB-Support
- **esp32-s3-display**: Dieser Branch (Display-Support)

### Commits
Dieser Branch wird **nicht automatisch in den Hauptbranch gemerged**. Er bleibt als separater Feature-Branch bestehen.

## Bekannte Probleme

1. **NTP-Sync**: Beim ersten Start kann es 2-3 Sekunden dauern, bis die Zeit vom NTP-Server synchronisiert ist
2. **Display-Flackern**: Bei sehr schnellen Updates kann das Display leicht flackern - daher 1s Update-Intervall
3. **Kleine Schrift**: Bei 135x240 Pixel ist 8px Schrift optimal für Lesbarkeit

## Troubleshooting

### Display bleibt schwarz
- Prüfe die Pin-Verbindungen
- Stelle sicher, dass `esp32-s3-geek-display` Environment verwendet wird
- Prüfe, ob Backlight-Pin (GPIO7) HIGH ist

### Keine Zeit angezeigt
- Prüfe WiFi-Verbindung
- Warte 5-10 Sekunden nach dem Start
- Überprüfe NTP-Server-Erreichbarkeit

### Falsche Zeitzone
- Passe `gmtOffset_sec` in `main.cpp` an
- Für Deutschland: `3600` (CET) oder `7200` (CEST)

## Lizenz

Apache-2.0 License (wie Hauptprojekt)

## Credits

- Original Projekt: [TheRealMoeder/Energy2Shelly_ESP](https://github.com/TheRealMoeder/Energy2Shelly_ESP)
- VDB-Integration: RaptorSDS
- Display-Support: RaptorSDS
- TFT_eSPI Library: [Bodmer](https://github.com/Bodmer/TFT_eSPI)

## Support

Bei Fragen oder Problemen:
1. Überprüfe [DISPLAY_INTEGRATION.md](DISPLAY_INTEGRATION.md)
2. Überprüfe die GitHub Issues
3. Erstelle ein neues Issue mit detaillierter Beschreibung
