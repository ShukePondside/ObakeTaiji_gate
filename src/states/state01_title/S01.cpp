#include <Arduino.h>
#include "S01.h"

bool Dx::State::S01::PreChangeState() {
  log_d("S01_PreChangeState");
  xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Title");
  xSemaphoreGive(display_mutex);
  return true;
}

bool Dx::State::S01::PostChangeState() {
  log_d("S01_PostChangeState");
  return true;
}

int Dx::State::S01::WasBtnAPressed() {
  log_d("S01_BtnA");
  return kNone;
}
int Dx::State::S01::WasBtnBPressed() {
  log_d("S01_btnB");
  return kNone;
}
int Dx::State::S01::WasBtnCPressed() {
  log_d("S01_btnC");
  return kNone;
}

int Dx::State::S01::SpecialEvent(int event_kind) {
  // Masterからオープニングに変更指令を受け取る
  EspNow::Message recive_message;
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, pdMS_TO_TICKS(100))) {
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_OPENING) {
      return kStateTransition;
    }
  }
  return kNone;
}
