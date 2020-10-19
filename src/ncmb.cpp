/*
 Copyright 2020 FUJITSU CLOUD TECHNOLOGIES LIMITED All Rights Reserved.
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
 http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#include <HTTPClient.h>
#include <base64.h>
#include "mbedtls/md.h"
#include "ncmb.h"


String NCMB::createSigniturePlaintextLastLine(
    String timestamp,
    String query) {
  String result = "SignatureMethod=HmacSHA256&SignatureVersion=2&X-NCMB-Application-Key="
      + m_applicationKey
      + "&X-NCMB-Timestamp=" + timestamp;
  if (query != "") {
    result += "&" + query;
  }
  return result;
}

String NCMB::createSigniturePlaintext(
    String methodString,
    String timestamp,
    String path,
    String query) {
  String result =  methodString 
      + "\n"
      + m_domainName
      + "\n/"
      + m_apiVersion
      + "/"
      + path
      + "\n"
      + createSigniturePlaintextLastLine(timestamp, query);
  return result;
}

String NCMB::calculateSignature(
    String methodString,
    String timestamp,
    String path,
    String query) {
  String plaintext = createSigniturePlaintext(methodString, timestamp, path, query);
  unsigned char input[plaintext.length() + 1];
  plaintext.getBytes(input, sizeof(input));
  unsigned char key[m_clientKey.length() + 1]; 
  m_clientKey.getBytes(key, sizeof(key));

  unsigned char signature[32];
  int res = mbedtls_md_hmac(
      mbedtls_md_info_from_type(MBEDTLS_MD_SHA256),
      key,
      sizeof(key) - 1,
      input,
      sizeof(input) - 1,
      signature);

  if(res) {
    return "ERROR!!!";
  }
  return base64::encode(signature, 32).c_str();
}

String NCMB::getRequestURL(String path, String query = "") {
  String url = "https://" + m_domainName + "/" + m_apiVersion + "/" + path;
  if(query != "") {
    url += "?" + query;
  }
  return url;
}

NCMBResponse NCMB::request(
    String methodString,
    String timestamp,
    String path,
    String query,
    String content) {
  HTTPClient httpClient;
  String url = getRequestURL(path, query);
  String signature = calculateSignature(methodString, timestamp, path, query);
  httpClient.begin(url, CA);
  httpClient.addHeader("Content-Type", "application/json");
  httpClient.addHeader("X-NCMB-Application-Key", m_applicationKey);
  httpClient.addHeader("X-NCMB-Timestamp", timestamp);
  httpClient.addHeader("X-NCMB-Signature", signature);

  int httpCode = requestMethod(&httpClient, methodString, content);
  NCMBResponse response = createResponse(httpCode, &httpClient);
  httpClient.end();
  return response;
}

int NCMB::requestMethod(HTTPClient *httpClient, String methodString, String content) {
  if (methodString == "GET") {
    return httpClient->GET();
  }
  if (methodString == "POST") {
    return httpClient->POST(content);
  }
  if (methodString == "PUT") {
    return httpClient->PUT(content);
  }
  return -1;
}

NCMBResponse NCMB::createResponse(int httpCode, HTTPClient *httpClient) {
  if(httpCode > 0) {
    boolean isSuccess = (httpCode == 200 || httpCode == 201);
    String responseContents = getResponseContents(httpClient);
    return NCMBResponse{isSuccess, responseContents};
  }
  String responseContents = "code:" + String(httpCode) 
      + ", error:" + httpClient->errorToString(httpCode).c_str();
  return NCMBResponse{false, responseContents};
}

String NCMB::getResponseContents(HTTPClient *httpclient) {
  int len = httpclient->getSize();
  String responseContents = "";
  uint8_t buff[128] = { 0 };
  WiFiClient * stream = httpclient->getStreamPtr();

  while(httpclient->connected() && (len > 0 || len == -1)) {
    size_t size = stream->available();
    if(size) {
      int count = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));
      String buffString = (const char *) buff;
      responseContents.concat(buffString.substring(0, count));
      if(len > 0) {
        len -= count;
      }
    } else {
      break;
    }
    delay(1);
  }
  return responseContents.substring(
      responseContents.indexOf("{"),
      responseContents.lastIndexOf("}") + 1);
}

NCMB::NCMB() {
  m_applicationKey = "";
  m_clientKey = "";
  m_domainName = "mbaas.api.nifcloud.com";
  m_apiVersion = "2013-09-01";
}

void NCMB::init(String applicationKey, String clientKey, String domainName, String apiVersion) {
  m_applicationKey = applicationKey;
  m_clientKey = clientKey;
  m_domainName = domainName;
  m_apiVersion = apiVersion;
}

NCMBResponse NCMB::registerObject(String className, String content, String timestamp) {
  String path = "classes/" + className;
  return request("POST", timestamp, path, "", content);
}

NCMBResponse NCMB::getObject(String className, String objectId, String timestamp) {
  String path = "classes/" + className + "/" + objectId;
  return request("GET", timestamp, path, "", "");
}

NCMBResponse NCMB::updateObject(String className, String objectId, String content, String timestamp) {
  String path = "classes/" + className + "/" + objectId;
  return request("PUT", timestamp, path, "", content);
}

NCMBResponse NCMB::searchObject(String className, String query, String timestamp) {
  String path = "classes/" + className;
  return request("GET", timestamp, path, query, "");
}
