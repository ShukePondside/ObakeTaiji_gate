#include "KV_HostLink_Ethernet.h"

String KV_HostLink_Ethernet::CheckModel() {
  String plc_response = SendCommandToPLC("?K\r");          // PLC機種要求コマンドを送信
  if (plc_model_.find(plc_response) != plc_model_.end()) { // 受信したコードでmapを検索
    return plc_model_[plc_response];                       // コードが存在すれば機種を返す
  } else {                                                 //
    return "NG";                                           // コードが存在しなければNGを返す
  }
}

String KV_HostLink_Ethernet::ConvertStringToHex(String input_string) {
  String command_value;                               // 変換したデータを格納する戻り値変数
  int str_len = input_string.length();                // 受け取った文字列の文字数
  char target_char[str_len];                          // 一文字ずつ変換する為、一時的に文字型として格納する
  input_string.toCharArray(target_char, str_len + 1); // 末尾のNULLが必要だがlengthに含まれない為+1する
  for (int i = 0; i <= str_len; i++) {                // 文字数分繰り返し
    if (target_char[i] == 0) {                        // 末尾文字NULL到達時の処理
      if (str_len % 2 != 0)                           // 奇数判別計算
        command_value += "00";                        // 文字数が奇数であれば末尾デバイスの下位バイトにNULL(00)を付与
      else                                            //
        command_value += " 0000";                     // 文字数が偶数であれば末尾デバイスの次に半角SP + 1ワードNULL(0000)を付与
    } else {                                          //
      if ((i != 0) && (i % 2 == 0))                   // 1ワードデバイス(2バイト)ごとにスペースを挿入
        command_value += " ";                         //
      command_value += String(target_char[i], HEX);   // 10進数を16進数に変換した上で文字列Stringとして戻り値変数に追加
    }
  }
  return command_value;
}

String KV_HostLink_Ethernet::SendCommandToPLC(String command) {
  String response_from_PLC;                                // PLCからのレスポンスをを格納する戻り値変数
  if (!EthernetClient::connect(IP_Address_, port_)) {          // PLCに接続
    response_from_PLC = "Error:PLC Connection Failed";     //
    return response_from_PLC;                              //
  }                                                        //
  EthernetClient::print(command);                              //
  unsigned long timeout = millis();                        //
  while (EthernetClient::available() == 0) {                   //
    if (millis() - timeout > 5000) {                       //
      EthernetClient::stop();                                  //
      response_from_PLC = "Error:PLC Timeout";             //
      return response_from_PLC;                            //
    }                                                      //
  }                                                        //
  while (EthernetClient::available()) {                        // PLCからのレスポンスを受信する
    response_from_PLC = EthernetClient::readStringUntil('\n'); //
    EthernetClient::stop();                                    //
    response_from_PLC.replace("\r", "");                   //
    response_from_PLC.replace("\n", "");                   //
    if (response_from_PLC == "OK") {                       //
      return "OK";                                         //
    } else if (response_from_PLC == "E0") {                // デバイス番号異常
      return "E0:Device No.Error";                         //
    } else if (response_from_PLC == "E1") {                // コマンド異常
      return "E1:Command Syntax Error";                    //
    } else if (response_from_PLC == "E2") {                // プログラム未登録
      return "E2:Programm No Regist Error";                //
    } else if (response_from_PLC == "E4") {                // 書込み禁止
      return "E4:Write Ban Error";                         //
    } else if (response_from_PLC == "E5") {                // 本体エラー
      return "E5:CPU Unit Error";                          //
    } else if (response_from_PLC == "E6") {                // コメントなし
      return "E6:Comment No Regist Error";                 //
    } else {                                               //
      return response_from_PLC;                            // 受信データをそのまま返す
    }
  }
}

int KV_HostLink_Ethernet::SplitString(String original_data, char delimiter, String *split_data) {
  int index = 0;                           //
  int dataLength = original_data.length(); //
  original_data.replace("\r", "");         // 余計なターミネータを除去
  original_data.replace("\n", "");         //
  for (int i = 0; i < dataLength; i++) {   // 文字数繰り返し
    char tmp = original_data.charAt(i);    // 文字を変数に取り出し
    if (tmp == delimiter)                  // 取り出した文字がデリミタ文字があれば次の配列に移行
      index++;                             //
    else                                   // デリミタ文字でなければ文字列に追加格納
      split_data[index] += tmp;            //
  }                                        //
  return (index + 1);                      // 分割数を戻り値とする
}

String KV_HostLink_Ethernet::WriteDevice(String device, bool bit) {
  return SendCommandToPLC("WR " + device + " " + String(bit) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int8_t value) {
  return SendCommandToPLC("WR " + device + ".S" + " " + String(value) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, uint8_t value) {
  return SendCommandToPLC("WR " + device + ".U" + " " + String(value) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int16_t value) {
  return SendCommandToPLC("WR " + device + ".S" + " " + String(value) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, uint16_t value) {
  return SendCommandToPLC("WR " + device + ".U" + " " + String(value) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int32_t value) {
  return SendCommandToPLC("WR " + device + ".L" + " " + String(value) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, uint32_t value) {
  return SendCommandToPLC("WR " + device + ".D" + " " + String(value) + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, String value) {
  if (value == "") {                                       // 書き込み文字列NULL処理
    return SendCommandToPLC("WR " + device + ".H 0000\r"); //
  }
  String command = ConvertStringToHex(value); // 文字列をソケット通信コマンド形式に変換
  // 送信データ量をワードデバイス数に変換
  // 【例】ABCDEFの文字列を送信する場合。1ワードデバイス(2バイト)に2文字格納する為
  // 　　　4142 4344 4546で3ワードデバイスとなる
  // 　　　1ワードデバイスの形式は4文字+SP1文字で5文字となるが最後のデバイスにSPは含まれない為
  // 　　　5で割った結果切り捨てられる1デバイス分を+1して補填する。
  int volume = command.length() / 5 + 1;
  return SendCommandToPLC("WRS " + device + ".H " + String(volume) + " " + command + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, bool bit[]) {
  String bits;
  for (int i = 0; i < qty; i++)
    bits += (i == qty - 1) ? String(bit[i]) : String(bit[i]) + " ";
  return SendCommandToPLC("WRS " + device + " " + String(qty) + " " + bits + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, int8_t value[]) {
  String values;
  for (int i = 0; i < qty; i++)
    values += (i == qty - 1) ? String(value[i]) : String(value[i]) + " ";
  return SendCommandToPLC("WRS " + device + ".S" + " " + String(qty) + " " + values + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, uint8_t value[]) {
  String values;
  for (int i = 0; i < qty; i++)
    values += (i == qty - 1) ? String(value[i]) : String(value[i]) + " ";
  return SendCommandToPLC("WRS " + device + ".U" + " " + String(qty) + " " + values + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, int16_t value[]) {
  String values;
  for (int i = 0; i < qty; i++)
    values += (i == qty - 1) ? String(value[i]) : String(value[i]) + " ";
  return SendCommandToPLC("WRS " + device + ".S" + " " + String(qty) + " " + values + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, uint16_t value[]) {
  String values;
  for (int i = 0; i < qty; i++)
    values += (i == qty - 1) ? String(value[i]) : String(value[i]) + " ";
  return SendCommandToPLC("WRS " + device + ".U" + " " + String(qty) + " " + values + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, int32_t value[]) {
  String values;
  for (int i = 0; i < qty; i++)
    values += (i == qty - 1) ? String(value[i]) : String(value[i]) + " ";
  return SendCommandToPLC("WRS " + device + ".L" + " " + String(qty) + " " + values + "\r");
}

String KV_HostLink_Ethernet::WriteDevice(String device, int qty, uint32_t value[]) {
  String values;
  for (int i = 0; i < qty; i++)
    values += (i == qty - 1) ? String(value[i]) : String(value[i]) + " ";
  return SendCommandToPLC("WRS " + device + ".D" + " " + String(qty) + " " + values + "\r");
}

String KV_HostLink_Ethernet::ReadDevice(String device, bool &bit) {
  String plc_response = SendCommandToPLC("RD " + device + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  bit = plc_response.toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int8_t &value) {
  String plc_response = SendCommandToPLC("RD " + device + ".S" + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  value = plc_response.toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, uint8_t &value) {
  String plc_response = SendCommandToPLC("RD " + device + ".U" + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  value = plc_response.toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int16_t &value) {
  String plc_response = SendCommandToPLC("RD " + device + ".S" + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  value = plc_response.toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, uint16_t &value) {
  String plc_response = SendCommandToPLC("RD " + device + ".U" + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  value = plc_response.toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int32_t &value) {
  String plc_response = SendCommandToPLC("RD " + device + ".L" + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  value = plc_response.toDouble();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, uint32_t &value) {
  String plc_response = SendCommandToPLC("RD " + device + ".D" + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  value = plc_response.toDouble();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, String &value) {
  String plc_response = SendCommandToPLC("RDS " + device + ".H" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  plc_response.replace(" ", "");                          // 受信データからスペース除去
  value = "";                                             // 参照渡し引数初期化
  for (int i = 0; i < qty * 2; i++) {                     // 指定デバイス数×2Byte繰り返し1Byteずつ文字変換
    String byte_of_value =                                // 1Byte抽出
        plc_response.substring(0 + (i * 2), 2 + (i * 2)); // 〃
    if (byte_of_value == "00") {                          // 末尾NULLであれば
      break;                                              // forを抜ける
    } else {                                              //
      char convert_hex_data[3];                           // 1Byte16進数格納変数
      byte_of_value.toCharArray(convert_hex_data, 3);     // String文字列をChar型に変換
      value += char(strtol(convert_hex_data, NULL, 16));  // Char型から16進数→10進数→文字に変換
    }
  }
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, bool bit[]) {
  String bits[qty];
  String plc_response = SendCommandToPLC("RDS " + device + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', bits);
  for (int i = 0; i < qty; i++)
    bit[i] = bits[i].toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, int8_t value[]) {
  String values[qty];
  String plc_response = SendCommandToPLC("RDS " + device + ".S" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', values);
  for (int i = 0; i < qty; i++)
    value[i] = values[i].toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, uint8_t value[]) {
  String values[qty];
  String plc_response = SendCommandToPLC("RDS " + device + ".U" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', values);
  for (int i = 0; i < qty; i++)
    value[i] = values[i].toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, int16_t value[]) {
  String values[qty];
  String plc_response = SendCommandToPLC("RDS " + device + ".S" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', values);
  for (int i = 0; i < qty; i++)
    value[i] = values[i].toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, uint16_t value[]) {
  String values[qty];
  String plc_response = SendCommandToPLC("RDS " + device + ".U" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', values);
  for (int i = 0; i < qty; i++)
    value[i] = values[i].toInt();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, int32_t value[]) {
  String values[qty];
  String plc_response = SendCommandToPLC("RDS " + device + ".L" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', values);
  for (int i = 0; i < qty; i++)
    value[i] = values[i].toDouble();
  return "OK";
}

String KV_HostLink_Ethernet::ReadDevice(String device, int qty, uint32_t value[]) {
  String values[qty];
  String plc_response = SendCommandToPLC("RDS " + device + ".D" + " " + String(qty) + "\r");
  if (plc_response.substring(0, 1) == "E") {
    return plc_response;
  }
  SplitString(plc_response, ' ', values);
  for (int i = 0; i < qty; i++)
    value[i] = values[i].toDouble();
  return "OK";
}