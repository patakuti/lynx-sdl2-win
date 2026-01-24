- Lynx SDL2 版 要件定義書

- 1. 概要
  - 目的: テキストブラウザ Lynx を SDL2 ベースの自前ウィンドウで動作させる
  - ベース: Lynx 2.9.2
  - 画面描画ライブラリ: PDCurses for SDL2

- 2. 機能要件
  - 2.1 基本機能
    - Lynx の既存機能をすべて維持する
    - SDL2 ウィンドウ内でテキスト表示を行う
    - キーボード入力を受け付ける
    - マウス操作に対応する（Lynx の既存マウス機能）

  - 2.2 日本語対応
    - 日本語テキストの表示に対応する
    - 日本語入力（IME）に対応する
    - UTF-8 エンコーディングをサポートする
    - CJK文字の幅を考慮したカーソル位置・ハイライト表示
      - リンク選択時のハイライト表示
      - 検索文字列のハイライト表示

  - 2.3 フォント
    - TrueType フォントを使用する（SDL2_ttf）
    - 日本語対応等幅フォントをサポートする
    - 環境変数 PDC_FONT でフォント指定可能

  - 2.4 SSL/TLS対応
    - HTTPSプロトコルに対応する
    - Windows版でSSL/TLS通信を有効にする
    - OpenSSLライブラリを使用する

- 3. 非機能要件
  - 3.1 対応プラットフォーム
    - Ubuntu Linux (22.04 LTS 以降)
    - Windows 10/11

  - 3.2 ビルド環境
    - Ubuntu: GCC, GNU Make, CMake
    - Windows: MSVC または MinGW

  - 3.3 依存ライブラリ
    - SDL2
    - SDL2_ttf
    - PDCurses (SDL2 port, WIDE=Y UTF8=Y でビルド)
    - OpenSSL (Windows版のみ、HTTPS対応)

- 4. 制約事項
  - ncurses/slang への依存を PDCurses に置き換える
  - Lynx のソースコード改変は最小限とする
  - ライセンス: Lynx (GPLv2), PDCurses (Public Domain), SDL2 (zlib)

- 5. 成果物
  - ビルド手順書（README.md に記載）
  - Ubuntu 用ビルドスクリプト
  - Windows 用ビルド設定
  - 動作する Lynx 実行ファイル
