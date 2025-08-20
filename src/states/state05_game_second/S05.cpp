#include <Arduino.h>
#include "S05.h"

bool Dx::State::S05::PreChangeState() {
  log_d("S05_PreChangeState");
  xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("GameSecond");
  xSemaphoreGive(display_mutex);
  return true;
}

bool Dx::State::S05::PostChangeState() {
  log_d("S05_PostChangeState");
  return true;
}

int Dx::State::S05::WasBtnAPressed() {
  log_d("S05_BtnA");
  return kNone;
}
int Dx::State::S05::WasBtnBPressed() {
  log_d("S05_btnB");
  return kNone;
}
int Dx::State::S05::WasBtnCPressed() {
  log_d("S05_btnC");
  return kNone;
}

int Dx::State::S05::SpecialEvent(int event_kind) {
  // Masterからエンディングに変更指令を受け取る
  EspNow::Message recive_message;
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, pdMS_TO_TICKS(100))) {
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_ENDING) {
      // StopMyTask();  // Obakeタスク終了
      return kStateTransition;
    }
  }
  return kNone;
}
