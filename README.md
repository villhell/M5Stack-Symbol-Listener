# M5Stack-Symbol-Listener

M5Stack-core2でSymbolの特定のアドレスに対するトランザクションを監視します。<br>
※現状はhashのみ表示

# DEMO
![image](https://github.com/villhell/M5Stack-Symbol-Listener/assets/6645392/0448aa25-e522-4cb3-b865-2f2fe5e2deb9)<br>
![image](https://github.com/villhell/M5Stack-Symbol-Listener/assets/6645392/80e40ea9-2d25-4bee-b93e-63dc56c330f7)<br>
![image](https://github.com/villhell/M5Stack-Symbol-Listener/assets/6645392/5a14780a-50dc-4f61-a5c2-ee0634763e57)<br>

# Features
トランザクションが来た事をすぐに確認できる。

# Requirement
platformio.iniを参照してください。

# Installation
* vscodeをインストール
* Platformioの拡張機能をインストール
* ※ArduinoIDEのインストールが必要かも

# Usage
main.cpp<br>
#define ADDRESS = "ここに監視したいアドレスを入力してください！"

SDカードにwifi.txtを作成し、1行目にSSID、2行目にpasswordを記載してください。<br>
※SDカードが読み込めない場合は「Failed to open file for reading」が表示されます。<br>

# License
MIT

