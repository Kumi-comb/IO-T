#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

Adafruit_SSD1306 display(128,64,&Wire,2); //displayインスタンス作成 エラーが出た場合は2を5に変えてみる

String httpGET(String url) {
    HTTPClient client;

    client.begin(url);
    int statusCode = client.GET();

    String result = "";
    if (statusCode < 0) {
        result = client.errorToString(statusCode);
    } else {
        result = client.getString();
    }
    client.end();

    return result;
}

void setup() {

    Serial.begin(9600);

    pinMode(14, OUTPUT);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  //ディスプレイ初期化
    display.clearDisplay();
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.println(String("AP Mode"));  //WiFiManagerのSSIDはESP  + MACアドレスの末尾3バイト
    display.print("ID:");
    display.println(ESP.getChipId());
    display.display(); //バッファの内容を表示 これが無いと適用されない バッファの内容を表示 これが無いと適用されない 
    
    WiFiManager wifimanager;
    wifimanager.autoConnect(); //以前接続したときの設定で接続できなかった場合はAPモードになり設定画面をだす おそらく接続できるまで次にいかない
    
    display.clearDisplay();
    display.setCursor(0,0);
    display.println(String("Connected."));
    display.println(WiFi.SSID());
    display.display();//バッファの内容を表示 これが無いと適用されない バッファの内容を表示 これが無いと適用されない 
    delay(5000);
    display.clearDisplay();
    display.display();//バッファの内容を表示 これが無いと適用されない バッファの内容を表示 これが無いと適用されない 
}

void loop(){
    String json = httpGET("http://iot.nsrv.work/api/devices/2/queue");

    const size_t jsonSize = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(6);
    DynamicJsonBuffer jsonBuffer(jsonSize);
    JsonObject& root = jsonBuffer.parseObject(json);

    const char* value = root["value"];
    String valueStatus = String(value);

    Serial.println(json);

    if (valueStatus == "ON") {
        digitalWrite(14, HIGH);
    } else if (valueStatus == "OFF") {
        digitalWrite(14, LOW);
    }

    delay(5000);
}
