#ifndef STATE_CONFIG_H
#define STATE_CONFIG_H

#include "../state_interface/state_interface.h"
#include "../states/state00_boot/S00.h"
#include "../states/state01_title/S01.h"
#include "../states/state02_opening/S02.h"
#include "../states/state03_game_first/S03.h"
#include "../states/state04_game_interval/S04.h"
#include "../states/state05_game_second/S05.h"
#include "../states/state06_ending/S06.h"
enum StateList {
  kS00,  // 起動
  kS01,  // タイトル
  kS02,  // オープニング
  kS03,  // ゲーム前半
  kS04,  // ゲームインターバル
  kS05,  // ゲーム後半
  kS06,  // エンディング
};

/* 状態遷移テーブル(pair<現在の状態,起こったイベント>,遷移先) */
using StateTransitionTable = std::map<std::pair<StateList, int>, StateList>;
StateTransitionTable state_transition_table =
    {
        {{kS00, kStateTransition}, kS01},  // Boot->Title(一旦ディレイ入れて分かり易く)

        {{kS01, kStateTransition}, kS02},  // Dlightの値を受け取ってSpecialで状態遷移(仮でCボタン運用)

        {{kS02, kStateTransition}, kS03},  // オープニング終了後->ゲーム前半開始

        {{kS03, kStateTransition}, kS04},  // ゲーム前半->ゲーム後半開始

        {{kS04, kStateTransition}, kS05},  // ゲーム前半->ゲームインターバル

        {{kS05, kStateTransition}, kS06},  // ゲームインターバル->エンディング

        {{kS06, kStateTransition}, kS01}};  // エンディング->タイトル画面

#endif