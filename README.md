# IAmNotBunkei

文化祭の出し物です
## Setup
install [nix](https://nixos.org/)

``$``は無視してください
```sh
$ mkdir ~/.config/nix -p
$ cat - > ~/.config/nix/nix.conf

```

IDEに設定を読み込ませたかったら
```sh
$ nix develop .
$ mkdir build && cd build
$ cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
$ cd ..
$ mv build/compile_commands.json .
$ rm build -r
```
この``compile_commands.json``をIDEを読み込ませる

## Build
```sh
$ nix build .
```
実行するには
```sh
$ nix run .
```
もし実行できなかったら``nix develop .``したあとに[公式チュートリアル](https://siv3d.github.io/ja-jp/download/ubuntu/#3-siv3d-%E3%82%92%E3%83%93%E3%83%AB%E3%83%89%E3%81%99%E3%82%8B)に従ってください

# Attention
formatしてからpushしてね
