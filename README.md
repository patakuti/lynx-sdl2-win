# Lynx SDL2 版

PDCurses for SDL2 を使用して、SDL2ウィンドウで動作する Lynx テキストブラウザです。

## 動作環境

- Ubuntu Linux 22.04 LTS 以降
- Windows 10/11 (未テスト)

## 依存パッケージ (Ubuntu)

```bash
sudo apt update
sudo apt install -y build-essential libsdl2-dev libsdl2-ttf-dev libssl-dev git fonts-noto-cjk
```

## ビルド手順

### 1. PDCurses の取得とビルド

```bash
cd /home/patakuti/CCROOT/lynx-sdl2

# PDCurses を取得
git clone https://github.com/wmcbrine/PDCurses.git

# SDL2版をビルド (WIDE=Y UTF8=Y で日本語対応)
cd PDCurses/sdl2
make WIDE=Y UTF8=Y

# ライブラリ名を修正 (リンカ用)
cp pdcurses.a libpdcurses.a
```

### 2. Lynx の configure パッチ適用

Lynx の configure スクリプトは X11 版 PDCurses を想定しているため、SDL2 版用にパッチを適用します。

```bash
cd /home/patakuti/CCROOT/lynx-sdl2/lynx2.9.2

# オリジナルをバックアップ
cp configure configure.orig

# パッチスクリプトを実行
python3 ../patch_configure.py
```

### 3. Lynx の configure と make

```bash
cd /home/patakuti/CCROOT/lynx-sdl2/lynx2.9.2

# configure 実行
./configure \
  --with-screen=pdcurses \
  --enable-widec \
  --with-ssl \
  --without-x \
  --disable-color-style \
  CPPFLAGS="-I/home/patakuti/CCROOT/lynx-sdl2/PDCurses -DPDCURSES -DPDC_WIDE -DPDC_FORCE_UTF8" \
  LDFLAGS="-L/home/patakuti/CCROOT/lynx-sdl2/PDCurses/sdl2"

# makefile に PDCurses と SDL2 ライブラリを追加
sed -i 's/^LIBS\t\t= /LIBS\t\t= -lpdcurses -lSDL2 -lSDL2_ttf /' makefile
sed -i 's/^LIBS\t\t= /LIBS\t\t= -lpdcurses -lSDL2 -lSDL2_ttf /' src/makefile

# ビルド
make
```

## 実行方法

```bash
cd /home/patakuti/CCROOT/lynx-sdl2/lynx2.9.2

# 日本語フォントを設定
export PDC_FONT=/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc

# Lynx を起動
./lynx

# または特定の URL を開く
./lynx https://www.example.com/
```

## フォント設定

環境変数 `PDC_FONT` で TrueType フォントを指定します。日本語表示には日本語対応フォントが必要です。

推奨フォント:
- `/usr/share/fonts/opentype/noto/NotoSansCJK-Regular.ttc` (Noto Sans CJK)
- `/usr/share/fonts/truetype/fonts-japanese-gothic.ttf` (IPAゴシック等)

## 主な変更点

Lynx 2.9.2 を PDCurses SDL2 版でビルドするために、以下の変更を行っています:

1. `configure` スクリプトのパッチ
   - X11 版 PDCurses (xcurses.h) から SDL2 版 (curses.h) への変更
   - X11 ライブラリチェックのスキップ

2. makefile の修正
   - `-lpdcurses -lSDL2 -lSDL2_ttf` ライブラリの追加

## ファイル構成

```
lynx-sdl2/
├── README.md               # 本ファイル
├── 01_requirements.md      # 要件定義
├── 02_design.md            # 設計書
├── 03_plan.md              # 実装計画
├── patch_configure.py      # configure パッチスクリプト
├── PDCurses/               # PDCurses ソース
│   └── sdl2/
│       ├── libpdcurses.a   # ビルド済みライブラリ
│       └── ...
└── lynx2.9.2/              # Lynx ソース
    ├── lynx                # ビルド済み実行ファイル
    ├── configure           # パッチ済み
    ├── configure.orig      # オリジナル
    └── ...
```

## ライセンス

- Lynx: GPLv2
- PDCurses: Public Domain
- SDL2: zlib License

## トラブルシューティング

### ウィンドウが表示されない

SDL2 が正しくインストールされているか確認:
```bash
sdl2-config --version
```

### 日本語が表示されない

1. フォントパスが正しいか確認
2. `fonts-noto-cjk` パッケージがインストールされているか確認:
   ```bash
   fc-list | grep -i "noto.*cjk"
   ```

### リンクエラー: cannot find -lpdcurses

`libpdcurses.a` が作成されているか確認:
```bash
ls -la PDCurses/sdl2/libpdcurses.a
```
