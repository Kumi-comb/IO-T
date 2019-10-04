#include <FS.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> 

#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Wire.h>

Adafruit_SSD1306 display(128,64,&Wire,2); //displayインスタンス作成 エラーが出た場合は2を5に変えてみる

void setup(){
    Wire.begin();
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

    const uint8_t ADR = 0x5c;
    uint8_t data[8];

void loop(){
    Wire.beginTransmission(ADR);
    Wire.endTransmission();
    delayMicroseconds(500);

    Wire.beginTransmission(ADR);
    Wire.write(0x03);
    Wire.write(0x00);
    Wire.write(0x04);
    Wire.endTransmission();

    delayMicroseconds(1500);
    Wire.requestFrom(ADR,8);
    delayMicroseconds(3);
    if (Wire.available() >= 8) {
        for(uint8_t i = 0; i = 8 ; i ++){
            data[i] = Wire.read();
        }
    }

    float hm = ((float)(data[2]*256+data[3])) / 10;
    float tp = ((float)(data[4]*256+data[5])) / 10;
    
    display.setCursor(0,0);
    display.println(hm);
    display.println(tp);
}
