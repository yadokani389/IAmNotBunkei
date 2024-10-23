# IAmNotBunkei

文化祭の出し物です
## Setup
install [nix](https://nixos.org/)

install [cachix](https://nixos.org/) (Optional 入れると無駄にビルドする必要が無くなります)

もしcachix入れたなら
```sh
$ cachix use yadokani389
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

# Attention
formatしてからpushしてね
