- Git/GitHub管理戦略
  - 作成日: 2026-01-24
  - 目的: PDCursesおよびLynx修正版をGitHub管理し、将来的な公開・本家へのPR提出を可能にする

- リポジトリ構成
  - 2つのprivateリポジトリを作成
    1. PDCurses修正版
    2. lynx-sdl2-win (メインプロジェクト)
  - lynx-sdl2-winはPDCursesをgit submoduleとして参照

- リポジトリ1: PDCurses
  - 目的: SDL2バックエンドの改善（CJK表示、IME対応、レンダリング改善）
  - リポジトリ名: PDCurses
  - 公開設定: Private（将来的にPublicまたはPR提出）
  - GitHubユーザー名: [username]
  - URL: github.com/[username]/PDCurses
  - 本家情報
    - URL: https://github.com/wmcbrine/PDCurses.git
    - ベースバージョン: 3.9-158-g09cf16db
    - ベースコミット: 09cf16db29753305e4241d4ae609aac997fa730d
    - ベース日付: 2025-11-23
  - 管理内容
    - pdcurses/ ディレクトリ配下の修正
    - sdl2/ ディレクトリ配下の修正
    - その他必要なファイル
  - 修正済みファイル
    - pdcurses/addch.c: waddchでワイド文字のプレースホルダー挿入
    - sdl2/pdcdisp.c: SDL2レンダリングでCJK文字の全体表示、PDC_doupdateで強制画面更新
    - sdl2/pdcscrn.c: SDL_HINT_IME_SHOW_UIをウィンドウ作成前に設定

- リポジトリ2: lynx-sdl2-win
  - 目的: Windows用SDL2版Lynxブラウザ（CJK表示、日本語入力、SSL/TLS対応）
  - リポジトリ名: lynx-sdl2-win
  - 公開設定: Private（将来的にPublicまたはPR提出）
  - GitHubユーザー名: [username]
  - URL: github.com/[username]/lynx-sdl2-win
  - 本家情報
    - URL: https://invisible-island.net/lynx/
    - GitHubミラー: https://github.com/ThomasDickey/lynx-snapshots (推定)
    - ベースバージョン: 2.9.2
    - リリース日: 2024-05-31
  - 管理内容
    - lynx2.9.2/ ディレクトリ配下のソースコード + 修正
    - PDCurses/ (git submodule)
    - ビルドスクリプト類
    - ドキュメント（docs/, README.md, CLAUDE.md, 01_requirements.md等）
    - 配布物（win64-dist/）
    - パッチファイル（記録用）
    - テストプログラム（test_*.c）
  - 修正済みファイル
    - CJK表示対応
      - src/LYCurses.c: LYstrExtent0でmk_wcwidth使用
      - src/GridText.c: PDCurses用にmk_wcwidthマクロ定義、UTFXTRA_ON_THIS_LINE文字幅計算修正
      - lynx_cfg.h: EXP_WCWIDTH_SUPPORT有効化
    - 日本語入力/IME対応
      - src/LYStrings.c: Unicode→UTF-8変換、バッファ拡張、UTF-8表示
      - src/LYEditmap.c: Unicode文字をLYE_CHARとして返す
    - Windows版SSL/TLS対応
      - configure_win64.sh: OpenSSL 3.4.0を使用してSSL有効化

- ブランチ戦略
  - PDCursesリポジトリ
    - main: 修正版の安定ブランチ
    - upstream-sync: 本家の更新取り込み用
    - feature/*: 機能追加・実験用（必要に応じて）
  - lynx-sdl2-winリポジトリ
    - main: 安定版（動作確認済み）
    - develop: 開発用メインブランチ
    - feature/*: 機能追加用（mainで直接作業しない）
    - hotfix/*: 緊急修正用

- コミット戦略
  - 原則
    - 機能単位でコミット（こまめにcommit）
    - コミットメッセージは英語（将来の公開・本家へのPR提出を想定）
    - 各コミットはビルド可能な状態を維持
    - ソースコード修正時は必ずビルド確認
  - コミットメッセージ例
    - "Fix IME hint timing in SDL2 backend"
    - "Add CJK character width handling in addch"
    - "Enable SSL/TLS support with OpenSSL 3.4.0"
  - コミット時の注意
    - 大きな修正の前に事前確認
    - ドキュメント更新を忘れない（特にREADME.md, 02_design.md）
    - 日付付きファイル・ディレクトリは一時的なので除外

- Git Submoduleの使い方
  - 基本動作
    - lynx-sdl2-winでコミットすると、その時点のPDCursesの特定コミットハッシュが記録される
    - バージョン間の依存関係が自動追跡される
    - ビルドの再現性が保証される
  - チェックアウト時の動作
    - lynx-sdl2-winの過去バージョンをcheckoutした後、`git submodule update`で対応するPDCursesがcheckoutされる
    - 自動的にはcheckoutされないため、明示的なコマンドが必要
  - 初回クローン時
    - オプション1: `git clone --recursive [URL]`
    - オプション2: `git clone [URL]` → `git submodule init` → `git submodule update`
  - Submoduleでの変更作業フロー（2段階）
    1. PDCurses側でコミット・プッシュ
      - cd PDCurses
      - (ファイル編集)
      - git add .
      - git commit -m "..."
      - git push origin main
      - cd ..
    2. lynx-sdl2-win側でsubmodule参照を更新
      - git status (modified: PDCurses が表示される)
      - git add PDCurses
      - git commit -m "Update PDCurses: ..."
      - git push origin main
  - 推奨ワークフロー
    - PDCursesリポジトリを別にクローンして作業
    - そこでコミット・プッシュ後、lynx-sdl2-winでsubmoduleを更新
    - submodule内での直接作業も可能だが、上記の方が管理しやすい

- 本家との同期方法
  - upstream設定
    - PDCursesリポジトリ: `git remote add upstream https://github.com/wmcbrine/PDCurses.git`
    - lynx-sdl2-winリポジトリ: 本家のGitリポジトリURLが判明次第設定
  - 定期的な確認手順
    1. 本家の更新をupstreamから取得: `git fetch upstream`
    2. CHANGELOG/リリースノートで変更内容確認
    3. 必要に応じてマージまたはcherry-pick
    4. 競合解決時は修正内容を優先しつつ本家の意図を尊重
    5. テスト＆ビルド確認
  - 記録の保持
    - どのバージョンをベースにしているか明記（上記「本家情報」参照）
    - パッチファイルを保持（差分確認用）
    - READMEに本家情報とフォーク理由を記載

- 将来の公開・PR戦略
  - 安定化フェーズ（現在private）
    - 十分なテスト実施
    - ドキュメント整備
    - コードクリーンアップ
    - ライセンス確認
  - 公開準備
    1. コミット履歴の整理（必要に応じてsquash）
    2. センシティブ情報の確認
    3. 本家との差分を明確化
    4. 貢献ガイドライン作成（必要に応じて）
  - 公開オプション
    - Option A: PR提出
      - 本家に機能提案
      - 修正を小さな単位に分割してPR
      - 受け入れられれば本家に統合
    - Option B: Public Fork
      - 独立したプロジェクトとして公開
      - 本家との違いを明記
      - 継続的メンテナンス
    - Option C: ハイブリッド
      - 一部をPR、一部を独立公開

- 初期セットアップ手順
  - Phase 1: PDCursesリポジトリ作成
    1. GitHub上でprivateリポジトリ作成（リポジトリ名: PDCurses）
    2. ローカルで新規ディレクトリ作成
    3. 本家からクローン: `git clone https://github.com/wmcbrine/PDCurses.git PDCurses-new`
    4. ベースコミットにチェックアウト: `git checkout 09cf16db`
    5. 新しいmainブランチ作成: `git checkout -b main`
    6. 現在の修正を適用（パッチまたは手動コピー）
    7. コミット: `git add .` → `git commit -m "Initial commit with SDL2 improvements"`
    8. GitHub上のリポジトリをremoteに設定: `git remote add origin [your-repo-url]`
    9. プッシュ: `git push -u origin main`
    10. upstream設定: `git remote add upstream https://github.com/wmcbrine/PDCurses.git`
  - Phase 2: lynx-sdl2-winリポジトリ作成
    1. GitHub上でprivateリポジトリ作成（リポジトリ名: lynx-sdl2-win）
    2. 現在の作業ディレクトリで初期化
      - cd /home/patakuti/CCROOT/lynx-sdl2
      - git init
    3. PDCursesディレクトリを一時的に退避
      - mv PDCurses PDCurses-backup
    4. .gitignore作成
    5. 初期コミット（PDCurses以外）
      - git add .
      - git commit -m "Initial commit: Lynx SDL2 Windows port"
    6. PDCursesをsubmoduleとして追加
      - git submodule add [your-PDCurses-repo-url] PDCurses
      - git commit -m "Add PDCurses as submodule"
    7. PDCurses-backupから修正を確認・マージ（必要に応じて）
    8. GitHub上のリポジトリをremoteに設定: `git remote add origin [your-repo-url]`
    9. プッシュ: `git push -u origin main`
    10. upstream設定（本家のGitリポジトリURLが判明次第）

- .gitignore設定（lynx-sdl2-win用）
  - ビルド生成物
    - *.o
    - *.obj
    - *.exe
    - *.lib
    - *.a
    - *.so
    - *.dll
  - ビルドディレクトリ
    - build/
    - obj/
  - エディタ・IDE設定
    - .vscode/
    - .idea/
    - *.swp
    - *~
  - OS固有ファイル
    - .DS_Store
    - Thumbs.db
  - 一時ファイル
    - 日付付きディレクトリ（例: 2601*/）
    - *.tmp
    - *.bak
  - 配布物（必要に応じて）
    - win64-dist/*.exe（バージョン管理する場合は除外しない）

- 注意事項
  - ライセンスファイルの保持
    - PDCursesとLynxの元のライセンスファイルを必ず含める
    - READMEに本家へのリンクと、これが修正版であることを明記
  - コミット履歴の管理
    - パッチファイルを適用した状態でコミット
    - コミットメッセージに本家のバージョン情報を記載
  - プッシュの順序
    - PDCursesの変更を先にプッシュ
    - その後、lynx-sdl2-winでsubmodule参照を更新してプッシュ
    - 順序を逆にすると、他の人がクローンしたときにエラーになる可能性
