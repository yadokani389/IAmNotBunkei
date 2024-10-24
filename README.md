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
もし実行できなかったら[公式チュートリアル](https://siv3d.github.io/ja-jp/download/ubuntu/)の必要な依存パッケージをインストールした後
```sh
$ nix develop .
$ mkdir build && cd build
$ cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
$ cd ..
$ cmake --build build
$ ./IAmNotBunkei
```

# Attention
formatしてからpushしてね
