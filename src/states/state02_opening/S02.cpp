#include <Arduino.h>
#include "S02.h"

bool Dx::State::S02::PreChangeState() {
  log_d("S02_PreChangeState");
    xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Opening");
  xSemaphoreGive(display_mutex);
  return true;
}

bool Dx::State::S02::PostChangeState() {
  log_d("S02_PostChangeState");
  return true;
}

int Dx::State::S02::WasBtnAPressed() {
  log_d("S02_BtnA");
  return kNone;
}
int Dx::State::S02::WasBtnBPressed() {
  log_d("S02_btnB");
  return kNone;
}
int Dx::State::S02::WasBtnCPressed() {
  log_d("S02_btnC");
  return kNone;
}

int Dx::State::S02::SpecialEvent(int event_kind) {
  // Masterからゲーム前半に変更指令を受け取る
  EspNow::Message recive_message;
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, pdMS_TO_TICKS(100))) {
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_GAME_FIRST) {
      return kStateTransition;
    }
  }
  return kNone;
}
