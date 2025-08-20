#include <Arduino.h>
#include "S04.h"

bool Dx::State::S04::PreChangeState() {
  log_d("S04_PreChangeState");
  xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Interval");
  xSemaphoreGive(display_mutex);
  return true;
}

bool Dx::State::S04::PostChangeState() {
  log_d("S04_PostChangeState");
  return true;
}

int Dx::State::S04::WasBtnAPressed() {
  log_d("S04_BtnA");
  return kNone;
}
int Dx::State::S04::WasBtnBPressed() {
  log_d("S04_btnB");
  return kNone;
}
int Dx::State::S04::WasBtnCPressed() {
  log_d("S04_btnC");
  return kNone;
}

int Dx::State::S04::SpecialEvent(int event_kind) {
  // Masterからゲーム後半に変更指令を受け取る
  EspNow::Message recive_message;
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, pdMS_TO_TICKS(100))) {
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_GAME_SECOND) {
      return kStateTransition;
    }
  }
  return kNone;
}
