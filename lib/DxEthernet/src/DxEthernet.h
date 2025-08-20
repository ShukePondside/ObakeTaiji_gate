#ifndef DXETHERNET_H
#define DXETHERNET_H

#include <Arduino.h>
#include <Ethernet.h>
#include <SPI.h>

// #include <M5Unified.h>
// #include "DxLED/DxLED.h"
// #include "DxSpeaker/DxSpeaker.h"
// #include "DxWiFi/DxWiFi.h"
// #include <DxInfoRegistration.h>

#if defined(ARDUINO_M5Stack_Core_ESP32)
#define SCK
#define MISO
#define MOSI
#define CS
#endif

#if defined(ARDUINO_M5STACK_FIRE)
#define SCK
#define MISO
#define MOSI
#define CS
#endif

#if defined(ARDUINO_M5Stack_ATOM)
#define SCK 22
#define MISO 23
#define MOSI 33
#define CS 19
#endif

#if defined(ARDUINO_M5Stack_ATOMS3)
#define SCK 5
#define MISO 7
#define MOSI 8
#define CS 6
#endif

class DxEthernet {

  public:                             //
  uint8_t IP_Address_[4];             // IPアドレス
  uint8_t subnet_mask_[4];            // サブネットマスク
  uint8_t gateway_[4] = {0, 0, 0, 0}; // デフォルトゲートウェイ
  uint8_t dns_[4] = {0, 0, 0, 0};     // DNSサーバー
  bool Initialize();                  // 初期化

  private:
  byte MAC_address_[6] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06};
};

#endif