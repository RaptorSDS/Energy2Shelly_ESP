// PATCH: VolkszählerDB (VDB) support - Added for custom energy data via UUID and average extraction
// --- Original header and includes ---
#include <Arduino.h>
#include <Preferences.h>
#ifndef ESP32
  #define WEBSERVER_H "fix WifiManager conflict"
#endif
#ifdef ESP32
  #include <HTTPClient.h>
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
  #include <WiFi.h>
#else
  #include <ESP8266HTTPClient.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266mDNS.h>
#endif
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <PubSubClient.h>
#include <WiFiClient.h>
#include <ESPAsyncWebServer.h>
#include <WiFiUdp.h>
#include <ModbusIP_ESP8266.h>

#define DEBUG true // set to false for no DEBUG output
#define DEBUG_SERIAL if(DEBUG)Serial

// --- Essentials ... Old code up to char force_pwr_decimals ... ----
// (The contents between headers and WifiManagerSetup function are unchanged, only skipped here for patching clarity)

// --- ADDED: New options for VDB VolkszählerDB ---
char vdb_enable[6] = "false";
char vdb_uuid_total[40] = "";
char vdb_uuid_l1[40] = "";
char vdb_uuid_l2[40] = "";
char vdb_uuid_l3[40] = "";

// --- VDB Helper Function ---
double fetchVDBValue(const char *uuid) {
  char vdb_url[128];
  sprintf(vdb_url, "%s/data/%s.json?from=now", mqtt_server, uuid);
  DEBUG_SERIAL.print("Fetching VDB: ");
  DEBUG_SERIAL.println(vdb_url);
  http.begin(wifi_client, vdb_url);
  int httpCode = http.GET();
  double value = 0.0;
  if (httpCode == 200) {
    DynamicJsonDocument doc(1024);
    DeserializationError err = deserializeJson(doc, http.getStream());
    if (!err && doc["data"]["average"].is<JsonVariant>()) {
      value = doc["data"]["average"];
      DEBUG_SERIAL.print("VDB value fetched: ");
      DEBUG_SERIAL.println(value);
    } else {
      DEBUG_SERIAL.println("VDB: No average value in JSON response");
    }
  } else {
    DEBUG_SERIAL.print("Failed HTTP");
    DEBUG_SERIAL.println(httpCode);
  }
  http.end();
  return value;
}

// --- PATCH to WifiManagerSetup: add VDB section and fields ---
// (Find the WiFiManagerParameter custom_section1 ... custom_section4 in WifiManagerSetup)
// Add after custom_section4:
WiFiManagerParameter custom_sectionVDB("<hr><h3>VolkszählerDB Support (VDB)</h3>");
WiFiManagerParameter custom_vdb_enable("vdb_enable", "<b>Enable VDB (VolkszählerDB)</b>", vdb_enable, 6);
WiFiManagerParameter custom_vdb_uuid_total("vdb_uuid_total", "<b>VDB UUID Gesamtleistung/Total Power</b>", vdb_uuid_total, 40);
WiFiManagerParameter custom_vdb_uuid_l1("vdb_uuid_l1", "<b>VDB UUID L1</b>", vdb_uuid_l1, 40);
WiFiManagerParameter custom_vdb_uuid_l2("vdb_uuid_l2", "<b>VDB UUID L2</b>", vdb_uuid_l2, 40);
WiFiManagerParameter custom_vdb_uuid_l3("vdb_uuid_l3", "<b>VDB UUID L3</b>", vdb_uuid_l3, 40);

wifiManager.addParameter(&custom_sectionVDB);
wifiManager.addParameter(&custom_vdb_enable);
wifiManager.addParameter(&custom_vdb_uuid_total);
wifiManager.addParameter(&custom_vdb_uuid_l1);
wifiManager.addParameter(&custom_vdb_uuid_l2);
wifiManager.addParameter(&custom_vdb_uuid_l3);

// --- PATCH in WifiManagerSetup read/update block ---
strcpy(vdb_enable, custom_vdb_enable.getValue());
strcpy(vdb_uuid_total, custom_vdb_uuid_total.getValue());
strcpy(vdb_uuid_l1, custom_vdb_uuid_l1.getValue());
strcpy(vdb_uuid_l2, custom_vdb_uuid_l2.getValue());
strcpy(vdb_uuid_l3, custom_vdb_uuid_l3.getValue());
// Save also to preferences if shouldSaveConfig...
if (shouldSaveConfig) {
  preferences.putString("vdb_enable", vdb_enable);
  preferences.putString("vdb_uuid_total", vdb_uuid_total);
  preferences.putString("vdb_uuid_l1", vdb_uuid_l1);
  preferences.putString("vdb_uuid_l2", vdb_uuid_l2);
  preferences.putString("vdb_uuid_l3", vdb_uuid_l3);
}

// --- PATCH: Add VDB reading to data fetch logic in loop() ---
// Insert after other data type checks in loop(), e.g. after if (dataHTTP) { ... }
if (strcmp(vdb_enable, "true") == 0) {
  // If PowerPath is UUID, use VDB fetch logic:
  if (strcmp(power_path, "TRIPHASE") == 0) {
    double l1 = fetchVDBValue(vdb_uuid_l1);
    double l2 = fetchVDBValue(vdb_uuid_l2);
    double l3 = fetchVDBValue(vdb_uuid_l3);
    setPowerData(l1, l2, l3);
  } else if (strlen(vdb_uuid_total) > 0) {
    double power = fetchVDBValue(vdb_uuid_total);
    setPowerData(power);
  }
}
//--- END PATCH ---
