#include <Arduino.h>
#include "S00.h"

bool is_peered = false;

bool Dx::State::S00::PreChangeState() {
  log_d("S00_PreChangeState");
  return true;
}

bool Dx::State::S00::PostChangeState() {
  log_d("S00_PostChangeState");
  return true;
}

int Dx::State::S00::WasBtnAPressed() {
  log_d("S00_BtnA");
  return kNone;
}
int Dx::State::S00::WasBtnBPressed() {
  log_d("S00_btnB");
  return kNone;
}
int Dx::State::S00::WasBtnCPressed() {
  log_d("S00_btnC");
  return kNone;
}

int Dx::State::S00::SpecialEvent(int event_kind) {
  log_d("S00_SpecialEvent");

  // ディスプレイ初期化 /////////////////////////////
  xSemaphoreTake(display_mutex, portMAX_DELAY);
  M5.Display.setBrightness(255);  // 0~255
  M5.Display.setRotation(DISPLAY_ROTIATION);
  M5.Display.clearClipRect();     // 描画範囲リセット
  M5.Display.setTextWrap(false);  // 文字を折り返さない
  M5.Display.clear(TFT_BLACK);
  M5.Display.setFont(&fonts::FreeMonoBold12pt7b);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.println("Boot...");
  xSemaphoreGive(display_mutex);
  /////////////////////////////////////////////////

  EspNow::Message recive_message;
  // ブロードキャストのデータを受け取りペアリングするまでループ
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, portMAX_DELAY)) {
    // 対象プロジェクトでなければ処理しない
    if (strncmp(recive_message.magic, MAGIC, 5) != 0) {
      continue;
    }
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_PEERING) {
      if (EspNow::AddPeer("master", recive_message.mac)) {
        log_d("マスターとのペアリングに成功しました");
        EspNow::Message send_message;
        send_message.terminal_kind = EspNow::KIND_GATE;  // ゲートウェイである事を知らせる
        send_message.command = EspNow::CMD_PEERING;
        delay(random(10, 100));  // 10〜100msのランダムディレイ
        esp_now_send(EspNow::GetPeerInfo().at("master").data(), (uint8_t *)&send_message, sizeof(send_message));
        xSemaphoreTake(display_mutex, portMAX_DELAY);
        M5.Display.setCursor(0, 20);
        M5.Display.println("Peer:OK");
        xSemaphoreGive(display_mutex);
        break;
      } else {
        log_d("マスターとのペアリングに失敗しました");
      }
    }
  }

  // Materからユニキャスト通信でゲームパラメータを受け取り変数に格納する
  // while (true) {
  //   if (xQueueReceive(EspNow::GetAppQueue(), &recive_message, portMAX_DELAY)) {
  //     if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
  //         recive_message.command == EspNow::CMD_PARAM_SET) {
  //       SemaphoreHandle_t buf = game_info->mutex_game_data;
  //       *game_info = recive_message.game_info;
  //       game_info->mutex_game_data = buf;
  //       xSemaphoreTake(display_mutex, portMAX_DELAY);
  //       M5.Display.setCursor(0, 40);
  //       M5.Display.println("Param:OK");
  //       xSemaphoreGive(display_mutex);
  //       break;  // 条件に合うデータを処理したら抜ける（必要であれば）
  //     }
  //   }
  // }

  // Masterからユニキャスト通信でタイトルに変更指令を受け取る
  while (xQueueReceive(EspNow::GetAppQueue(), &recive_message, portMAX_DELAY)) {
    // log_d("変更待ち");
    // log_d("%d", recive_message.terminal_kind);
    // log_d("%d", recive_message.command);
    // log_d("%d", recive_message.mode);
    if (recive_message.terminal_kind == EspNow::KIND_MASTER &&
        recive_message.command == EspNow::CMD_MODE_CHANGE &&
        recive_message.mode == EspNow::MODE_TITLE) {
      return kStateTransition;
    }
  }
}