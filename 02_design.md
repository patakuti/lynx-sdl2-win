- Lynx SDL2 版 設計書

- 1. アーキテクチャ概要

  - 1.1 システム構成図
    ```
    ┌─────────────────────────────────────────┐
    │           Lynx 2.9.2                    │
    │  (既存コード、最小限の変更)              │
    │  --with-screen=pdcurses でビルド        │
    ├─────────────────────────────────────────┤
    │      PDCurses (SDL2 port)               │
    │  WIDE=Y UTF8=Y でビルド                 │
    │  curses API → SDL2 描画変換             │
    ├─────────────────────────────────────────┤
    │         SDL2 + SDL2_ttf                 │
    │  ウィンドウ管理、フォント描画           │
    │  SDL_StartTextInput() で IME 対応       │
    └─────────────────────────────────────────┘
    ```

  - 1.2 コンポーネント
    - Lynx: テキストブラウザ本体
    - PDCurses (SDL2): curses互換レイヤー（SDL2バックエンド）
    - SDL2: クロスプラットフォームマルチメディアライブラリ
    - SDL2_ttf: TrueTypeフォント描画ライブラリ

- 2. ビルド構成

  - 2.1 PDCurses (SDL2 port) ビルド
    - ソース: https://github.com/wmcbrine/PDCurses
    - ビルドオプション:
      - WIDE=Y: 16bit Unicode文字対応（SDL2_ttf使用）
      - UTF8=Y: UTF-8文字列処理
    - 成果物: libpdcurses.a (または pdcurses.dll)

  - 2.2 Lynx ビルド
    - configure オプション:
      - --with-screen=pdcurses
      - --enable-widec (ワイド文字対応)
      - --with-ssl (オプション: HTTPS対応)
    - 環境変数:
      - CPPFLAGS: PDCursesヘッダパス
      - LDFLAGS: PDCursesライブラリパス
    - 成果物: lynx 実行ファイル

- 3. プラットフォーム別設計

  - 3.1 Ubuntu Linux
    - 依存パッケージ:
      - libsdl2-dev
      - libsdl2-ttf-dev
      - build-essential
      - libssl-dev (オプション)
    - ビルド手順:
      1. PDCurses SDL2 をビルド
      2. Lynx を PDCurses にリンクしてビルド
    - フォント: /usr/share/fonts から日本語等幅フォントを使用

  - 3.2 Windows
    - ビルド環境: MinGW-w64 または MSVC
    - 依存ライブラリ:
      - SDL2 (開発ライブラリ)
      - SDL2_ttf (開発ライブラリ)
      - OpenSSL (MinGW用バイナリ、HTTPS対応)
    - ビルド手順:
      1. PDCurses SDL2 をビルド
      2. Lynx を PDCurses + OpenSSL にリンクしてビルド
    - フォント: C:\Windows\Fonts から日本語等幅フォントを使用
    - SSL/TLS:
      - OpenSSL 3.x系を使用
      - MinGW用のクロスコンパイル済みバイナリを使用
      - configure時に --with-ssl オプションを指定

- 4. 日本語対応設計

  - 4.1 文字表示
    - PDCurses WIDE=Y で Unicode 対応
    - SDL2_ttf で TrueType フォント描画
    - 日本語等幅フォント（例: Noto Sans Mono CJK JP）を使用

  - 4.2 日本語入力 (IME)
    - SDL2 の SDL_StartTextInput() API を使用
    - SDL_TEXTINPUT イベントで確定文字を受信
    - SDL_TEXTEDITING イベントで変換中文字を受信
    - PDCurses PR #37 で複数文字入力対応済み

  - 4.3 フォント設定
    - 環境変数 PDC_FONT でフォントファイルを指定
    - 例: export PDC_FONT=/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc

- 5. ディレクトリ構成

  ```
  lynx-sdl2/
  ├── 01_requirements.md      # 要件定義
  ├── 02_design.md            # 設計書（本文書）
  ├── 03_plan.md              # 実装計画
  ├── README.md               # ビルド手順書
  ├── CLAUDE.md               # 開発ルール
  ├── lynx2.9.2/              # Lynx ソースコード
  ├── PDCurses/               # PDCurses ソースコード（取得後）
  └── build/                  # ビルド成果物（生成後）
      ├── pdcurses/           # PDCurses ビルド
      └── lynx/               # Lynx ビルド
  ```

- 6. 制約事項・リスク

  - 6.1 既知の制約
    - PDCurses SDL2 は X11 版と比較して一部機能制限あり
    - IME 候補リストの表示はOS標準に依存

  - 6.2 リスク
    - SDL2 の IME 対応がプラットフォームにより異なる
      - 対策: 実機テストで動作確認
    - フォント依存の表示崩れ
      - 対策: 等幅フォントを推奨、設定ガイド提供

- 7. テスト計画

  - 7.1 機能テスト
    - Lynx 起動確認
    - Webページ表示
    - 日本語ページ表示
    - 日本語入力（検索、フォーム）
    - マウス操作

  - 7.2 プラットフォームテスト
    - Ubuntu Linux での動作確認
    - Windows 10/11 での動作確認
