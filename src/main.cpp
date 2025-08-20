#include <M5Unified.h>
#include <LittleFS.h>
#include <WiFi.h>
#include <esp_now.h>

#include "modules/filesystem/filesystem.h"
#include "modules/display/display.h"
#include "modules/EspNow/EspNow.h"

#include "state_controller/state_controller.h"
#include "state_interface/state_interface.h"
#include "state_utility/state_utility.h"

#include <DxEthernet.h>
#include <KV_HostLink_Ethernet.h>

#define STATE_QTY 7

IPAddress local_IP(192, 168, 99, 123);  // 固定したいIP
IPAddress gateway(192, 168, 99, 1);     // ルータのゲートウェイ
IPAddress subnet(255, 255, 255, 0);     // サブネットマスク

// 状態遷移管理 ///////////////////////////////////////////////////
StateList current_state = kS00;             // StateList管理
int event_kind;                             // EventKind管理
StateInterface *states[STATE_QTY];          // 状態遷移インスタンス
StateController state_controller(nullptr);  // 状態遷移コントローラ
//////////////////////////////////////////////////////////////////

// ゲームデータ /////////////
EspNow::GameInfo game_info;
////////////////////////////

/* Ethernet */
DxEthernet eth;
KV_HostLink_Ethernet kv;

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);

  // 自身のMACアドレス確認 ///////////////////////
  uint8_t baseMac[6];
  esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
  log_d("MyMAC:%02X:%02X:%02X:%02X:%02X:%02X",
        baseMac[0], baseMac[1], baseMac[2],
        baseMac[3], baseMac[4], baseMac[5]);
  //////////////////////////////////////////////

  // LittleFS初期化 ////////////////////////////////
  if (!LittleFS.begin()) {
    log_e("ファイルストレージの初期化に失敗しました");
    return;
  }
  /////////////////////////////////////////////////

  // Ethernet接続
  eth.IP_Address_[0] = 192;
  eth.IP_Address_[1] = 168;
  eth.IP_Address_[2] = 99;
  eth.IP_Address_[3] = 123;
  eth.subnet_mask_[0] = 255;
  eth.subnet_mask_[1] = 255;
  eth.subnet_mask_[2] = 255;
  eth.subnet_mask_[3] = 0;
  eth.Initialize();

  // KV接続確認 ///////////////////////////////////////////
  kv.IP_Address_[0] = 192;
  kv.IP_Address_[1] = 168;
  kv.IP_Address_[2] = 99;
  kv.IP_Address_[3] = 12;
  int common_retry_count = 0;
  while (kv.CheckModel() == "NG") {
    common_retry_count++;
    if (common_retry_count >= 3) {
      log_e("KV-Connect-Error");
    }
  }
  log_d("KV-Connect-OK");
  /////////////////////////////////////////////////////////

  Display::Initialize();  // ディスプレイ初期化
  EspNow::Initialize();  // ESP-NOW設定初期化
  game_info.mutex_game_data = xSemaphoreCreateMutex();
  if (game_info.mutex_game_data == NULL) {
    log_e("Mutex creation failed!");
    while (1);
  }

  // 状態遷移初期化 ///////////////////////////////////////////
  states[0] = new Dx::State::S00(Display::GetDisplayMutex());
  states[1] = new Dx::State::S01(Display::GetDisplayMutex());
  states[2] = new Dx::State::S02(Display::GetDisplayMutex());
  states[3] = new Dx::State::S03(Display::GetDisplayMutex());
  states[4] = new Dx::State::S04(Display::GetDisplayMutex());
  states[5] = new Dx::State::S05(Display::GetDisplayMutex());
  states[6] = new Dx::State::S06(Display::GetDisplayMutex(), &kv);
  state_controller = StateController(states[current_state]);
  ////////////////////////////////////////////////////////////

  // Boot
  event_kind = state_controller.SpecialEvent(kNone);

  // Boot->Title
  current_state = state_controller.ChangeState(current_state, event_kind, state_transition_table, states);

  log_d("M5 setup end");
}

void loop() {
  M5.update();

  /* ボタン操作以外のイベントハンドラ */
  event_kind = state_controller.SpecialEvent(kNone);
  if (event_kind & kStateTransition) {
    current_state = state_controller.ChangeState(current_state, event_kind, state_transition_table, states);
  }

  /* ボタン操作に関するイベントハンドラ */
  if (M5.BtnA.wasPressed()) {
    event_kind = state_controller.WasBtnAPressed();
    if (event_kind & kStateTransition)
      current_state = state_controller.ChangeState(current_state, event_kind, state_transition_table, states);
  } else if (M5.BtnB.wasPressed()) {
    event_kind = state_controller.WasBtnBPressed();
    if (event_kind & kStateTransition)
      current_state = state_controller.ChangeState(current_state, event_kind, state_transition_table, states);
  } else if (M5.BtnC.wasPressed()) {
    event_kind = state_controller.WasBtnCPressed();
    if (event_kind & kStateTransition)
      current_state = state_controller.ChangeState(current_state, event_kind, state_transition_table, states);
  }

  delay(10);
}
