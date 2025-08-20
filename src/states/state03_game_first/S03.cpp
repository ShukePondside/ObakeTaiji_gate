#include <Arduino.h>
#include "S03.h"

bool Dx::State::S03::PreChangeState() {
  log_d("S03_PreChangeState");
  xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("GameFirst");
  xSemaphoreGive(display_mutex);
  return true;
}

bool Dx::State::S03::PostChangeState() {
  log_d("S03_PostChangeState");
  return true;
}

int Dx::State::S03::WasBtnAPressed() {
  log_d("S03_BtnA");
  return kNone;
}
int Dx::State::S03::WasBtnBPressed() {
  log_d("S03_btnB");
  return kNone;
}
int Dx::State::S03::WasBtnCPressed() {
  log_d("S03_btnC");
  return kNone;
}

int Dx::State::S03::SpecialEvent(int event_kind) {
  EspNow::Message recive_message;
  // Masterからインターバルに変更指令を受け取る
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, pdMS_TO_TICKS(100))) {
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_GAME_INTERVAL) {
      // StopMyTask();  // Obakeタスク終了
      return kStateTransition;
    }
  }
  return kNone;
}
