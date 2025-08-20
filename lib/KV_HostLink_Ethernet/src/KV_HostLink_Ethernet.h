#ifndef KV_HOSTLINK_H
#define KV_HOSTLINK_H

#include <DxEthernet.h>
#include <map>
#include <vector>

class KV_HostLink_Ethernet : public EthernetClient {
  public:
  bool enable_;                       // PLC上位リンク有効
  uint8_t IP_Address_[4];             // PLC IPアドレス
  struct tm next_send_date_;          // 次回のKV上位リンク送信予定日時
  struct tm last_send_date_;          // 前回のKV上位リンク送信日時
  bool send_cycle_type_;              // KV送信単位[0:分／1:秒]
  uint8_t send_cycle_;                // KV上位リンク周期[分]
  uint8_t send_sec_;                  // KV上位リンク時間[秒]
  char first_device_[9];              // KV登録先頭デバイス
  std::vector<String> regist_device_; // KV登録デバイス
  int16_t correction_hour_;           // NTP無効時のPLCカレンダタイマ時補正
  int16_t correction_min_;            // NTP無効時のPLCカレンダタイマ分補正
  int16_t correction_sec_;            // NTP無効時のPLCカレンダタイマ秒補正

  String CheckModel();                                          // PLCの型式を問い合わせる
  String WriteDevice(String device, bool bit);                  // 指定デバイスへの1bit単体書き込み
  String WriteDevice(String device, int8_t value);              // 指定デバイスへの符号付き8bit整数単体書き込み
  String WriteDevice(String device, uint8_t value);             // 指定デバイスへの符号無し8bit整数単体書き込み
  String WriteDevice(String device, int16_t value);             // 指定デバイスへの符号付き16bit整数単体書き込み
  String WriteDevice(String device, uint16_t value);            // 指定デバイスへの符号無し16bit整数単体書き込み
  String WriteDevice(String device, int32_t value);             // 指定デバイスへの符号付き32bit整数単体書き込み
  String WriteDevice(String device, uint32_t value);            // 指定デバイスへの符号無し32bit整数単体書き込み
  String WriteDevice(String device, String value);              // 指定デバイスへの文字列書き込み
  String WriteDevice(String device, int qty, bool bit[]);       // 指定デバイスへの1bit連続書き込み
  String WriteDevice(String device, int qty, int8_t value[]);   // 指定デバイスへの符号付き8bit整数連続書き込み
  String WriteDevice(String device, int qty, uint8_t value[]);  // 指定デバイスへの符号無し8bit整数連続書き込み
  String WriteDevice(String device, int qty, int16_t value[]);  // 指定デバイスへの符号付き16bit整数連続書き込み
  String WriteDevice(String device, int qty, uint16_t value[]); // 指定デバイスへの符号無し16bit整数連続書き込み
  String WriteDevice(String device, int qty, int32_t value[]);  // 指定デバイスへの符号付き32bit整数連続書き込み
  String WriteDevice(String device, int qty, uint32_t value[]); // 指定デバイスへの符号無し32bit整数連続書き込み
  String ReadDevice(String device, bool &bit);                  // 指定デバイスから1bit単体読み込み
  String ReadDevice(String device, int8_t &value);              // 指定デバイスから符号付き8bit整数単体読み込み
  String ReadDevice(String device, uint8_t &value);             // 指定デバイスから符号無し8bit整数単体読み込み
  String ReadDevice(String device, int16_t &value);             // 指定デバイスから符号付き16bit整数単体読み込み
  String ReadDevice(String device, uint16_t &value);            // 指定デバイスから符号無し16bit整数単体読み込み
  String ReadDevice(String device, int32_t &value);             // 指定デバイスから符号付き32bit整数単体読み込み
  String ReadDevice(String device, uint32_t &value);            // 指定デバイスから符号無し32bit整数単体読み込み
  String ReadDevice(String device, int qty, String &value);     // 指定デバイスから文字列読み込み
  String ReadDevice(String device, int qty, bool bit[]);        // 指定デバイスから1bit連続読み込み
  String ReadDevice(String device, int qty, int8_t value[]);    // 指定デバイスから符号付き8bit整数連続読み込み
  String ReadDevice(String device, int qty, uint8_t value[]);   // 指定デバイスから符号無し8bit整数連続読み込み
  String ReadDevice(String device, int qty, int16_t value[]);   // 指定デバイスから符号付き16bit整数連続読み込み
  String ReadDevice(String device, int qty, uint16_t value[]);  // 指定デバイスから符号無し16bit整数連続読み込み
  String ReadDevice(String device, int qty, int32_t value[]);   // 指定デバイスから符号付き32bit整数連続読み込み
  String ReadDevice(String device, int qty, uint32_t value[]);  // 指定デバイスから符号無し32bit整数連続読み込み

  private:
  std::map<String, String> plc_model_ = {
      {"54", "KV-7300"},
      {"55", "KV-7500"},
      {"57", "KV-8000"},
      {"58", "KV-8000A"}};
  uint16_t port_ = 8501;                                                     // PLC通信ポート
  String ConvertStringToHex(String input_string);                            // 書込む文字列を16進数に変換
  String SendCommandToPLC(String command);                                   // PLCにコマンド出力
  int SplitString(String original_data, char delimiter, String *split_data); // 文字列分割
};

#endif