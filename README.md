# MCP9804_PIC
これはマイクロチップ・テクノロジーの温度センサMCP9804をPIC16F1619でコントロールするプログラムです。マイコンボードはPIC16F1619が標準で搭載されているCuriosityボードを使用しています。Curiosityボード上からI2C信号を取り出してMCP9804から温度データを取り出してUartで温度を出力しています。  

![teraterm_capture](https://user-images.githubusercontent.com/2620156/36341141-ff87ede2-142c-11e8-8fad-a0e0a707fca0.gif)

## MCCの設定
Microchip Code Configurator(MCC)で基本設定を行っています。MCCはEUSARTのみ使用していてMSSPのライブラリは使用していません。使用していない理由は使いにくいのと無駄にメモリを使用していたためです。

|MSSP | RAM size | ROM size|
|:----|----------|---------|
|使用 | 107      | 1643    |
|未使用|73       | 1393   |

### PIN設定
下図のMCC設定画面を参照してください。  
![pin asign](https://user-images.githubusercontent.com/2620156/36339272-06efcbc2-1405-11e8-9d09-4a7464d4cbc9.png)

## MCP9804コントロール
MCP9804.cがメインの温度取得、設定コードです。上記に記したようにI2C制御にMCCを使用しなかったためI2C制御はオリジナルで書きました。

### MCP9804.h
このヘッダー内の
```
#define MCP9804_PARAM_T_UPPER_H       0x01
#define MCP9804_PARAM_T_UPPER_L       0x60  // 22c
#define MCP9804_PARAM_T_LOWER_H       0x01
#define MCP9804_PARAM_T_LOWER_L       0x00  // 16c
#define MCP9804_PARAM_T_CRIT_H       0x01
//#define MCP9804_PARAM_T_CRIT_L       0xe0  // 30c
#define MCP9804_PARAM_T_CRIT_L       0x70  // 23c
```
でTupper,Tlower,TcritのAlertの設定を行っています。

## Alertピン
現状Alertピンを使用していないためConfigurationの設定は行っていません。そのためデフォルトのまま使用しています。

## その他
次はBME280の制御をPIC16F1619で行ってみたいと思います。すでにBME280_PICというレポジトリを作っていますがこれはPIC16F15344という新しい石で作成したのですがメモリ容量が少なく86%もROMを消費してしまいました。このため作り直します。レポジトリの名前はBME280_PIC16F1619にする予定です。
