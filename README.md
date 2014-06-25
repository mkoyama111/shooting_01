shooting_01
===========

# 概要
cocos2d-x v3.0により開発された縦スクロールシューティングゲームです。  
Windows, Android, iPhoneに対応しています。  
ゲームプログラミンの練習として作成したものです。

#動作環境
Windows 8.1, Android実機(4.1.2), iPhoneシミュレータ上で動作確認。

# 開発環境
ゲームエンジン: Cocos2d-x v3.0  
Windows版: Visual C++ 2012  
Android版: Eclipse ADT v22.6.2, Android NDK r9d, Android API Level 16  
iPhone版: Xcode Version 5.1.1  

# 遊び方
* ゲーム開始画面で「ゲームスタート」をタッチするとゲームが開始します。
* 自機の移動は画面タッチにより行います（タッチした位置に自機が移動します）。
* 画面タッチ中、弾が自動で発射されます。
    * 通常の敵(飛行している敵)への攻撃はこの弾で行います。
* 地上の敵はロックオンレーザーで攻撃します。
    * 地上敵に近づくと、自動でロックオンします。
    * ロックオンした後、画面からタッチを外すと、レーザーが発射されます。
* 自機の残機は画面下のLife(緑色のマーク)で表示されます。
    * Lifeが無くなるとゲームオーバーです。
* 敵ボスを倒すとゲームクリアとなります。
