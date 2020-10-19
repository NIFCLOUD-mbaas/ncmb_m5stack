# ニフクラ mobile backend M5Stack ライブラリ

## 概要

ニフクラ mobile backend M5Stack ライブラリは、
モバイルアプリのバックエンド機能を提供するクラウドサービス
[ニフクラ mobile backend](https://mbaas.nifcloud.com) の M5Stack 用ライブラリです。

データストアの一部機能について利用することが可能です。

このライブラリを利用する前に、ニフクラ mobile backendのアカウントを作成する必要があります。

## 動作環境

- M5Stack
- Arduino IDE
- M5Stack Library <https://github.com/m5stack/M5Stack>
- Arduino core for the ESP32 <https://github.com/espressif/arduino-esp32>
  - ライブラリ内にて HTTPClient.h, base64.h , mbedtls/md.h を使用しています。

## 試験実装ライブラリ

こちらのライブラリにつきまして試験実装であるためテクニカルサポートは対応しておりません。御注意ください。

## ライセンス

このライブラリのライセンスにつきましては LICENSE ファイルをご覧ください。

## インストール

- [ニフクラ mobile backend M5Stack ライブラリ](https://github.com/NIFCLOUD-mbaas/ncmb_m5stack) のページにある `[↓Code]` (Download code) ボタンより、 `"Download ZIP"` を選択しライブラリの zip ファイルをダウンロードします。
- Arduino IDE を起動し、ツールバーより `"スケッチ"` → `"ZIP形式のライブラリをインストール"` を選択し、先ほどダウンロードしたライブラリを読み込み、ライブラリを利用可能にします。
- インストール後は Arduino IDE ツールバーより `"ファイル"` → `"スケッチ例"` の `"カスタムライブラリのスケッチ例"` に `"ncmb_M5stack"` という項目が追加されます。この中の `"NCMBQuickStart"` からサンプルスケッチを開くことができます。

## リファレンス

### NCMB::init

- 説明: NCMBインスタンスにアプリケーションキーなどを登録し、リクエスト可能な状態にします。
- 書式

```cpp
#include "ncmb.h"

void NCMB::init(String appkey, String clikey, String domainName, String apiVersion);
```

- 引数:
  - `appkey`: ニフクラ mobile backend アプリケーションキー
  - `clikey`: ニフクラ mobile backend クライアントキー
  - `domainName`: ニフクラ mobile backend 接続先ドメイン（省略可）
  - `apiVersion`: ニフクラ mobile backend APIバージョン（省略可）
- 戻り値:
  - (なし)
- 使用例

```cpp
#include "ncmb.h"

NCMB ncmb;

ncmb.init(
    "0000111122223333444455556666777788889999aaaabbbbccccddddeeeeffff", // アプリケーションキー
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef");  // クライアントキー
```

### NCMB::registerObject

- 説明: 指定したクラスに対して、オブジェクトの登録を行います。
- 書式

```cpp
#include "ncmb.h"

NCMBResponse NCMB::registerObject(String className, String content, String timestamp);
```

- 引数:
  - `className`: データストア クラス名
  - `content`: 登録内容 (Json形式)
  - `timestamp`: タイムスタンプ (`[YYYY]-[MM]-[DD]T[HH]:[mm]:[ss].[sss]Z` 形式)
- 戻り値:
  - NCMBResponse
- 使用例

```cpp
#include "ncmb.h"

NCMB ncmb;

NCMBResponse response = ncmb.registerObject(
    "TestClass",　// データストア クラス名
    "{\"item1\":\"takano-kun\"}", // 登録内容
    "1986-02-04T12:34:56.123Z"); // タイムスタンプ
if (response.isSuccess) {
  // 登録成功時
  // response.content に登録したレコードのオブジェクトId と作成日時タイムスタンプが Json 形式文字列(String型)にて格納されている。
} else {
  // 登録失敗時
  // response.content にエラーコードとエラーメッセージが Json 形式文字列(String型)にて格納されている。
}
```

### NCMB::getObject

- 説明: 指定オブジェクトの取得を行います。
- 書式

```cpp
#include "ncmb.h"

NCMBResponse NCMB::getObject(String className, String objectId, String timestamp);
```

- 引数:
  - `className`: データストア クラス名
  - `objectId`: オブジェクトID
  - `timestamp`: タイムスタンプ (`[YYYY]-[MM]-[DD]T[HH]:[mm]:[ss].[sss]Z` 形式)
- 戻り値:
  - NCMBResponse
- 使用例

```cpp
NCMBResponse response = ncmb.getObject(
    "TestClass",　// データストア クラス名
    "abcdef012345", // 取得対象レコードのオブジェクトId
    "1986-02-04T12:34:56.123Z"); // タイムスタンプ
if (response.isSuccess) {
  // 取得成功時
  // response.content に取得したレコードの内容が Json 形式文字列(String型)にて格納されている。
} else {
  // 取得失敗時
  // response.content にエラーコードとエラーメッセージが Json 形式文字列(String型)にて格納されている。
}
```

### NCMB::updateObject

- 説明: 指定オブジェクトに対して、フィールドの更新を行います。
- 書式

```cpp
#include "ncmb.h"

NCMBResponse NCMB::updateObject(String className, String objectId, String content, String timestamp);
```

- 引数:
  - `className`: データストア クラス名
  - `objectId`: オブジェクトID
  - `content`: 更新フィールドと更新内容 (Json形式)
  - `timestamp`: タイムスタンプ (`[YYYY]-[MM]-[DD]T[HH]:[mm]:[ss].[sss]Z` 形式)
- 戻り値:
  - NCMBResponse
- 使用例

```cpp
NCMBResponse response = ncmb.updateObject(
    "TestClass",　// データストア クラス名
    "abcdef012345", // 更新対象レコードのオブジェクトId
    "{\"item1\":\"takanosan\"}", // 更新フィールドと更新内容
    "1986-02-04T12:34:56.123Z"); // タイムスタンプ
if (response.isSuccess) {
  // 更新成功時
  // response.content に更新日時が Json 形式文字列(String型)にて格納されている。
} else {
  // 更新失敗時
  // response.content にエラーコードとエラーメッセージが Json 形式文字列(String型)にて格納されている。
}
```

### NCMB::searchObject

- 説明: クエリ条件に合致したオブジェクトの情報取得を行います。
- 書式

```cpp
#include "ncmb.h"

NCMBResponse NCMB::searchObject(String className, String query, String timestamp);
```

- 引数:
  - `className`: データストア クラス名
  - `query`: 検索パラメタ
  - `timestamp`: タイムスタンプ (`[YYYY]-[MM]-[DD]T[HH]:[mm]:[ss].[sss]Z` 形式)
- 戻り値:
  - NCMBResponse
- 使用例

```cpp
NCMBResponse response = ncmb.searchObject(
    "TestClass",　// データストア クラス名
    "limit=1&where=%7B%22item1%22%3A%22takano-kun%22%7D" // URIエンコードされた検索パラメタ
    "1986-02-04T12:34:56.123Z"); // タイムスタンプ
if (response.isSuccess) {
  // 取得成功時
  // response.content に取得した内容が Json 形式文字列(String型)にて格納されている。
} else {
  // 取得失敗時
  // response.content にエラーコードとエラーメッセージが Json 形式文字列(String型)にて格納されている。
}
```

- 検索パラメタについて
  - **検索パラメタ** は `検索キー=検索値` を `&` で接続した文字列です。
    - e.g. `searchItem1=value1&searchItem2=value2`
  - **検索キー** には絞り込みを行う `where` や取得件数の上限を定める `limit` などが存在します。
  - **検索値** は URI エンコードされていることが求めれられます。
    - `where={"item1":"takano-kun"}` → `where=%7B%22item1%22%3A%22takano-kun%22%7D` ※ `=` は検索値に含まないため URI エンコードをしない
  - 検索キーと検索値の並び順は検索キーのアルファベット昇順が求められます。
    - OK: `limit=1&where=%7B%22item1%22%3A%22takano-kun%22%7D`
    - NG: `where=%7B%22item1%22%3A%22takano-kun%22%7D&limit=1`
  - 詳しくは [こちら](https://mbaas.nifcloud.com/doc/current/rest/common/query.html) をご確認ください。

### NCMBResponse

- 説明: リクエスト結果を格納する構造体です。

```cpp
struct NCMBResponse {
  boolean isSuccess;
  String content;
};
```

- メンバ
  - `isSuccess`: リクエストが成功した場合は `true` が、失敗した場合は `false` が格納されます。
  - `content`: レスポンス内容 (Json形式)

### スケッチ例

ご使用にあたりサンプルコード内  `WIFI SSID`, `WIFI PASSPHRASE`, `NCMB APPLICATION KEY`, `NCMB CLIENT KEY` を適宜変更してください。

```cpp
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
```

## 制限事項

ライブラリ使用によりスケッチに利用可能な領域の圧迫を低減させるため、一部の引数、戻り値について機能提供していない部分が存在します。

### Json の操作

オブジェクト登録、更新内容、レスポンス内容は Json 形式の文字列ですが Json オブジェクトでの操作機能は提供しておりません。
そのためユーザー自身での文字列操作実装により必要情報の取得、Json 文字列の生成をおこなう必要があります。

Jsonの生成例

```cpp
String targetValue1 = "takano-kun";
float targetValue2 = 3.14;

String requestContent = "{\"item1\":\"" + targetValue1 + "\",\"item2\":" + String(targetValue2) + "}";
M5.Lcd.println(requestContent); // {"item1":"takano-kun","item2":3.14}
```

Json から必要情報の抽出例

```cpp
String responseContent = "{\"item1\":\"takano-kun\",\"item2\":3.14}";

String targetValue1tmp = responseContent
    .substring(responseContent.indexOf("\"item1\":") + String("\"item1\":\"").length()); // 取得したい文字列より前を除去
String targetValue1 = targetValue1tmp.substring(0, targetValue1tmp.indexOf("\"")); // 取得したい文字列より後を除去
M5.Lcd.println(targetValue1); // takano-kun

String targetValue2tmp = responseContent
    .substring(responseContent.indexOf("\"item2\":") + String("\"item2\":").length());
float targetValue2 = targetValue2tmp
    .substring(0, targetValue2tmp.indexOf("}")) // 文字列の場合は項目の終端がダブルクォーテーションであるが、数値などの場合は位置により `}`、`,` 2パターンが考えられるため。
    .substring(0, targetValue2tmp.indexOf(","))
    .toFloat();
M5.Lcd.println(targetValue2); // 3.14
```

複雑な Json 操作が必要な場合は JSON Library for Arduino <https://github.com/arduino-libraries/Arduino_JSON> など Json 操作用のライブラリを導入することをご検討ください。

### URI エンコード

検索パラメタにて URI エンコード文字列を用意する必要がありますが、こちらにつきましても URI エンコード化機能につきましては提供しておりません。そのためユーザー自身での文字列操作実装によりエンコードをおこなう必要があります。

### タイムスタンプ生成

M5Stack は標準では内部時計を使用できません。そのためユーザー自身にてタイムスタンプ生成を実装していただく必要があります。

time.h を用いた、タイムスタンプ実装例

```cpp
#include <time.h>

// 事前に Wifi の接続が必要

// 時刻取得の事前準備。実行時に１回だけ実行。時刻取得先であるNTPサーバは最大3箇所まで設定可能。
configTime(0, 0, "ntp.nict.jp");

// ...

String timestamp = getTimestamp();

// ...

// タイムスタンプ取得関数
String getTimestamp() {
  struct tm timeInfo;
  getLocalTime(&timeInfo);
  // inde             012345678901234567890123
  String timestamp = "20YY-MM-DDTHH:mm:ss.000Z";
  timestamp.setCharAt(2, ((0 < (timeInfo.tm_year - 100) / 10) ? (timeInfo.tm_year - 100) / 10 : 0) + 48);
  timestamp.setCharAt(3, (timeInfo.tm_year - 100) % 10 + 48);
  timestamp.setCharAt(5, ((0 < (timeInfo.tm_mon + 1) / 10) ? (timeInfo.tm_mon + 1) / 10 : 0) + 48);
  timestamp.setCharAt(6, (timeInfo.tm_mon + 1) % 10 + 48);
  timestamp.setCharAt(8, ((0 < timeInfo.tm_mday / 10) ? timeInfo.tm_mday / 10 : 0) + 48);
  timestamp.setCharAt(9, timeInfo.tm_mday % 10 + 48);
  timestamp.setCharAt(11, ((0 < timeInfo.tm_hour / 10) ? timeInfo.tm_hour / 10 : 0) + 48);
  timestamp.setCharAt(12, timeInfo.tm_hour % 10 + 48);
  timestamp.setCharAt(14, ((0 < timeInfo.tm_min / 10) ? timeInfo.tm_min / 10 : 0) + 48);
  timestamp.setCharAt(15, timeInfo.tm_min % 10 + 48);
  timestamp.setCharAt(17, ((0 < timeInfo.tm_sec / 10) ? timeInfo.tm_sec / 10 : 0) + 48);
  timestamp.setCharAt(18, timeInfo.tm_sec % 10 + 48);
  return timestamp;  
}
```

## 参考URL集

- [ニフクラ mobile backend](https://mbaas.nifcloud.com/)
- [ドキュメント](https://mbaas.nifcloud.com/doc/current/)
