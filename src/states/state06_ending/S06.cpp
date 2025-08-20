#include <Arduino.h>
#include "S06.h"

bool Dx::State::S06::PreChangeState() {
  log_d("S06_PreChangeState");
  xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Ending");
  xSemaphoreGive(display_mutex);
  // 実際にはSpecialEventでデータを受け取った時に処理
  // uint16_t result[4] = {100, 200, 300, 400};
  // Serial.println(kv->WriteDevice("DM100", 4, result));
  return true;
}

bool Dx::State::S06::PostChangeState() {
  log_d("S06_PostChangeState");
  return true;
}

int Dx::State::S06::WasBtnAPressed() {
  log_d("S06_BtnA");
  return kButtonA_Transition;
}
int Dx::State::S06::WasBtnBPressed() {
  log_d("S06_btnB");
  return kNone;
}
int Dx::State::S06::WasBtnCPressed() {
  log_d("S06_btnC");
  return kNone;
}

int Dx::State::S06::SpecialEvent(int event_kind) {
  EspNow::Message recive_message;
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, pdMS_TO_TICKS(100))) {
    // Masterからゲーム結果を受け取る
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_DATA_UPDATE) {
      uint16_t result[4] = {0};
      result[0] = recive_message.game_info.data.defert_count;
      Serial.println(kv->WriteDevice("DM100", 4, result));
    }
    // Masterからタイトルに変更指令を受け取る
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_TITLE) {
      return kStateTransition;
    }
  }
  return kNone;
}
