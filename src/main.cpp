#include <Arduino.h>
#include <M5Core2.h>

#include <SD.h>
#include <WiFiClientSecure.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

#define USE_SERIAL Serial1
#define ADDRESS "MONITORING_ADDRESS"
WebSocketsClient webSocket;
String uid;

/**
 * @brief 画面初期化
*/
void initScreen(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
}

/**
 * @brief バイナリダンプ
*/
void hexdump(const void *mem, uint32_t len, uint8_t cols = 16) {
	const uint8_t* src = (const uint8_t*) mem;
	USE_SERIAL.printf("\n[HEXDUMP] Address: 0x%08X len: 0x%X (%d)", (ptrdiff_t)src, len, len);
	for(uint32_t i = 0; i < len; i++) {
		if(i % cols == 0) {
			USE_SERIAL.printf("\n[0x%08X] 0x%08X: ", (ptrdiff_t)src, i);
		}
		USE_SERIAL.printf("%02X ", *src);
		src++;
	}
	USE_SERIAL.printf("\n");
}

/**
 * @brief Uncomfirmed
*/
void subscribeUncomfirmed(String uid){
  String message = "{\"uid\":\"" + uid + "\", \"subscribe\":\"unconfirmedAdded/" + ADDRESS + "\"}";
  webSocket.sendTXT(message);
  M5.Lcd.println("subscribe unconfirmedAdded");
}

/**
 * @brief Comfirmed
*/
void subscribeComfirmed(String uid){
  String message = "{\"uid\":\"" + uid + "\", \"subscribe\":\"confirmedAdded/" + ADDRESS + "\"}";
  webSocket.sendTXT(message);
  M5.Lcd.println("subscribe confirmedAdded");
}

/**
 * @brief WebSocketイベント
*/
void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {

  // 画面初期化
  initScreen();

  switch(type) {
		case WStype_DISCONNECTED:
      M5.Lcd.print("[WSc] Disconnected!\n");
			break;
		case WStype_CONNECTED:
      M5.Lcd.printf("[WSc] Connected to url: %s\n", payload);
			break;
		case WStype_TEXT:
    {
      DynamicJsonDocument json_payload(1024);
      DeserializationError error = deserializeJson(json_payload, (const char*)payload);
      if (error) {
        M5.Lcd.print(F("deserializeJson() failed: "));
        M5.Lcd.println(error.c_str());
        uid = "";
      }else{
        if(json_payload.containsKey("uid")) {
          uid = json_payload["uid"].as<String>();
          
          // uncomfirmedを購読
          subscribeUncomfirmed(uid);
          // comfirmedを購読
          subscribeComfirmed(uid);

        } else{
          M5.Lcd.println(json_payload["topic"].as<String>());
          M5.Lcd.println(json_payload["data"]["meta"]["hash"].as<String>());
        }
        
      }
			break;
    }

		case WStype_BIN:
			M5.Lcd.printf("[WSc] get binary length: %u\n", length);
			hexdump(payload, length);
			break;
		case WStype_ERROR:
      M5.Lcd.printf("[WSc] get error: %s\n", payload);
      break;
		case WStype_FRAGMENT_TEXT_START:
      M5.Lcd.print("WStype_FRAGMENT_TEXT_START");
      break;
    case WStype_FRAGMENT_BIN_START:
      M5.Lcd.print("WStype_FRAGMENT_BIN_START");
      break;
		case WStype_FRAGMENT:
      M5.Lcd.print("WStype_FRAGMENT");
      break;
		case WStype_FRAGMENT_FIN:
      M5.Lcd.print("WStype_FRAGMENT_FIN");
			break;
	}
}

void setup() {
  M5.begin();

  // 画面初期化
  initScreen();

  // 1000ms待つ
  delay(1000);
  
  // SDカード読込
  File f = SD.open("/wifi.txt", FILE_READ);
  
  // SDカードが読み込めたかどうか
  if (f) {
    M5.Lcd.println("open SD");
    String line;

    // ファイルのサイズを取得
    size_t sz = f.size();
    
    // ファイルの内容を格納するバッファを確保
    char buf[sz + 1];

    // ファイルの内容を読み込む
    f.read((uint8_t*)buf, sz);
    
    // バッファの終端にヌル文字を追加
    buf[sz] = 0;
    
    // ファイルを閉じる
    f.close();

    int y = 0;
    for(int x = 0; x < sz; x++) {
      if(buf[x] == 0x0a || buf[x] == 0x0d)
        buf[x] = 0;
      else if (!y && x > 0 && !buf[x - 1] && buf[x])
        y = x;
    }
    M5.Lcd.print(line);
    
    // WiFi接続
    WiFi.begin(buf, &buf[y]);
    
    // WiFi接続待ち
    while (WiFi.status() != WL_CONNECTED) {
      //M5.Lcd.println("wifi no connected");
      delay(500);
      M5.Lcd.print(".");
    }

    M5.Lcd.println("wifi connected");
    M5.Lcd.println(WiFi.localIP());
    
    // server address, port and URL
    webSocket.beginSSL("mikun-testnet.tk", 3001, "/ws");

    // event handler
    webSocket.onEvent(webSocketEvent);

    // try ever 5000 again if connection has failed
    webSocket.setReconnectInterval(5000);
  } else {
    M5.Lcd.print("Failed to open file for reading");
  }
}

void loop() {
  webSocket.loop();
}