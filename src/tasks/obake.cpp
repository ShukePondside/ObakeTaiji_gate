#include "obake.h"

static TaskContext taskContext;

void StartMyTask(EspNow::GameInfo* game_info) {
  // 初期化
  if (taskContext.handle == nullptr) {
    taskContext.running = true;
    taskContext.game_info = game_info;
    // taskContext.dlight = _dlight;
    // タスク起動
    xTaskCreatePinnedToCore(tObake, "MyTask", 4096, &taskContext, 1, &taskContext.handle, 1);
  }
}

void StopMyTask() {
  if (taskContext.handle != nullptr) {
    taskContext.running = false;
    // タスクが終了するまで待機（最大1秒）
    for (int i = 0; i < 100; ++i) {
      if (eTaskGetState(taskContext.handle) == eDeleted) {
        break;
      }
      vTaskDelay(10 / portTICK_PERIOD_MS);
    }
    taskContext.handle = nullptr;  // ハンドル無効化
  }
}

void tObake(void* pvParameters) {
  TaskContext* ctx = (TaskContext*)pvParameters;

  randomSeed(analogRead(0));  // ランダムシードを初期化
  bool enabled = false;
  bool moving = false;
  unsigned long now = millis();
  unsigned long prevMillis = now;

  // 初回の出現間隔
  unsigned long interval;

  if (!ctx->game_info->data.game_round) {
    interval = random(ctx->game_info->param.first_arrival_min,
                      ctx->game_info->param.first_arrival_max);
  } else {
    interval = 0;  // Second Gameは初回に全部出てくる
  }

  unsigned long enemy_kind_lottely;  // 敵の抽選変数

  // 初回の敵の種類決定////////////////////////////////////////////////////////
  xSemaphoreTake(ctx->game_info->mutex_game_data, portMAX_DELAY);
  if (!ctx->game_info->data.game_round) {
    ctx->game_info->data.enemy_kind = EspNow::ENEMY_WHITE;  // 初回は必ずホワイト
  } else {
    if (!ctx->game_info->data.bonus_time) {
      enemy_kind_lottely = random(0, 1000);
      if (enemy_kind_lottely < ctx->game_info->param.reinbow_ratio) {
        ctx->game_info->data.enemy_kind = EspNow::ENEMY_REINBOW;
      } else if (enemy_kind_lottely < ctx->game_info->param.reinbow_ratio +
                                          ctx->game_info->param.red_ratio) {
        ctx->game_info->data.enemy_kind = EspNow::ENEMY_RED;
      } else {
        ctx->game_info->data.enemy_kind = EspNow::ENEMY_WHITE;
      }
    } else {
      ctx->game_info->data.enemy_kind = EspNow::ENEMY_RED;
    }
  }
  xSemaphoreGive(ctx->game_info->mutex_game_data);
  //////////////////////////////////////////////////////////////////////

  while (1) {
    // log_d("敵の種類:%d", ctx->game_info->data.enemy_kind);
    now = millis();
    // lux = sensor.getLUX();
    // log_d("lux:%d\n", lux);
    // 一定周期が経過したらenabledをtrueにする

    if (now - prevMillis >= interval) {
      moving = true;
      prevMillis = now;
      // 逃げるまでの間隔
      if (!ctx->game_info->data.game_round) {
        interval = random(ctx->game_info->param.first_escape_min,
                          ctx->game_info->param.first_escape_max);
      } else {
        interval = random(ctx->game_info->param.second_escape_min,
                          ctx->game_info->param.second_escape_max);
      }
    }

    if (moving) {
      moving = false;

      if (!enabled) {
        M5.Display.startWrite();
        for (int i = -128; i <= 0; i += 4) {
          if (xSemaphoreTake(ctx->game_info->mutex_game_data, 0) == pdTRUE) {
            Display::Draw(ctx->game_info->data.enemy_kind, 0, i);
            xSemaphoreGive(ctx->game_info->mutex_game_data);
          }
          if (!ctx->running) {
            M5.Display.endWrite();
            vTaskDelete(NULL);  // 自タスクを削除
          }
        }
        M5.Display.endWrite();
        enabled = true;
      } else {
        enabled = false;
        M5.Display.startWrite();
        for (int i = 0; i >= -128; i -= 4) {
          if (xSemaphoreTake(ctx->game_info->mutex_game_data, 0) == pdTRUE) {
            Display::Draw(ctx->game_info->data.enemy_kind, 0, i);
            xSemaphoreGive(ctx->game_info->mutex_game_data);
          }
          if (!ctx->running) {
            M5.Display.endWrite();
            vTaskDelete(NULL);  // 自タスクを削除
          }
        }
        M5.Display.endWrite();
        M5.Display.fillScreen(TFT_BLACK);
        // 敵の種類決定////////////////////////////////////////////////////////
        xSemaphoreTake(ctx->game_info->mutex_game_data, portMAX_DELAY);
        if (!ctx->game_info->data.bonus_time) {
          enemy_kind_lottely = random(0, 1000);
          if (enemy_kind_lottely < ctx->game_info->param.reinbow_ratio) {
            ctx->game_info->data.enemy_kind = EspNow::ENEMY_REINBOW;
          } else if (enemy_kind_lottely < ctx->game_info->param.reinbow_ratio +
                                              ctx->game_info->param.red_ratio) {
            ctx->game_info->data.enemy_kind = EspNow::ENEMY_RED;
          } else {
            ctx->game_info->data.enemy_kind = EspNow::ENEMY_WHITE;
          }
        } else {
          ctx->game_info->data.enemy_kind = EspNow::ENEMY_RED;
        }
        xSemaphoreGive(ctx->game_info->mutex_game_data);
        //////////////////////////////////////////////////////////////////////
      }
    }

    if (!ctx->running) {
      vTaskDelete(NULL);  // 自タスクを削除
    }
    delay(1);
  }
}
