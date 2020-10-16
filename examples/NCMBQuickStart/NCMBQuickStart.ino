/*
  NIFCLOUD mobile backend M5Stack quick start

  This example shows
  1. Wifi connection.
  2. Initialization of ncmb instance.
  3. Data registration in the data store.
  4. Search data to data store

  Please enter `WIFI SSID`, `WIFI PASSPHRASE`, `NCMB APPLICATION KEY`, `NCMB CLIENT KEY`.
*/

#include <M5Stack.h>
#include <WiFi.h>
#include "ncmb.h"

NCMB ncmb;

void setup() {

  // Initialization M5Stack LCD.
  M5.begin();
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(2);

  // 1. Wifi connection.

  // ** Please enter wifi information **
  WiFi.begin(
      "WIFI SSID",
      "WIFI PASSPHRASE");

  String wifiInitStatement = "init wifi |";
  // 'kurukuru' is used for progress display. It has nothing to do with wifi initialization.
  const char kurukuruChar[] = "|/-\\";
  int kurukuru = 0;
  while (WiFi.status() != WL_CONNECTED) {
    M5.Lcd.setCursor(0, 0);
    wifiInitStatement.setCharAt(
        wifiInitStatement.length() - 1,
        kurukuruChar[kurukuru % 4]);
    M5.Lcd.println(wifiInitStatement);
    kurukuru++;
    delay(50);
  }

  // 2. Initialization of ncmb instance.
  M5.Lcd.println("init ncmb");

  // ** Please enter NIFCLOUD mobile backend information **
  ncmb.init(
      "NCMB APPLICATION KEY",
      "NCMB CLIENT KEY");

  // 3. Data registration in the data store.
  M5.Lcd.println("register object");

  String itemValue1 = "takano-kun";
  String content1 = "{\"item1\":\"" + itemValue1 + "\"}";
  String timestamp1 = "1986-02-04T12:34:56.123Z";

  NCMBResponse response1 = ncmb.registerObject("TestClass", content1, timestamp1);

  M5.Lcd.println(response1.isSuccess? "true" : "false");
  M5.Lcd.println(response1.content);

  // 4. Search data to data store
  M5.Lcd.println("search object");

  // means "limit=1&where={"item1":"takano-kun"}"
  // see also https://mbaas.nifcloud.com/doc/current/rest/common/query.html
  String itemValue2 = "takano-kun";
  String query2 = "limit=1&where=%7B%22item1%22%3A%22" + itemValue2 + "%22%7D";
  String timestamp2 = "1986-02-04T12:34:56.123Z";

  NCMBResponse response2 = ncmb.searchObject("TestClass", query2, timestamp2);
  M5.Lcd.println(response2.isSuccess? "true" : "false");
  M5.Lcd.println(response2.content);

}

void loop() {
}