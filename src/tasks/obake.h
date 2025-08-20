#pragma once

#include <M5Unified.h>
// #include <M5_DLight.h>

#include "../modules/display/display.h"
#include "../modules/espnow/espnow.h"

// タスクで使う共有データ
struct TaskContext {
  TaskHandle_t handle;
  volatile bool running;
  EspNow::GameInfo* game_info;
  // M5_DLight* dlight;
};

// 敵の種類
// enum EnemyKind {
//   ENEMY_WHITE = 0,
//   ENEMY_RED = 2,
//   ENEMY_REINBOW = 4
// };

// 起動/停止関数の宣言
void StartMyTask(EspNow::GameInfo* game_info);
void StopMyTask();

void tObake(void* pvParameters);