# M5Stack-Symbol-Listener

M5Stack-core2でSymbolの特定のアドレスに対するトランザクションを監視します。
※現状はhashのみ表示

# DEMO
![image](https://user-images.githubusercontent.com/6645392/236090041-12feb88e-b1f6-4faf-b746-a84cb1b04a9f.png)
![image](https://user-images.githubusercontent.com/6645392/236090116-88895395-7671-41c5-95e1-e4251fe98515.png)
![image](https://user-images.githubusercontent.com/6645392/236090169-ebbbc802-e9c0-4f16-8636-51a39ebbb5c8.png)

# Features
トランザクションが来た事をすぐに確認できる。

# Requirement
platformio.iniを参照してください。

# Installation
・vscodeをインストール
・Platformioの拡張機能をインストール
※ArduinoIDEのインストールが必要かも

# Usage
main.cpp
#define ADDRESS = "ここに監視したいアドレスを入力してください！"

SDカードにwifi.txtを作成し、1行目にSSID、2行目にpasswordを記載してください。
※SDカードが読み込めない場合は「Failed to open file for reading」が表示されます。

# License
MIT

