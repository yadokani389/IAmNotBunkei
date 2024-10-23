{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/release-24.05";
    OpenSiv3D.url = "github:yadokani389/OpenSiv3D";
  };
  outputs = { nixpkgs, OpenSiv3D, ... }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      IAmNotBunkei = with pkgs;
        stdenv.mkDerivation {
          pname = "IAmNotBunkei";

          version = "0.0.1";

          src = ./.;

          installPhase = ''
            runHook preInstall

            mkdir -p $out/bin
            cd ..
            cp IAmNotBunkei $out/bin
            cp -r ${OpenSiv3D}/Linux/App/resources $out/bin

            runHook postInstall
          '';

          cmakeFlags = [ "-GNinja" "-DCMAKE_BUILD_TYPE=RelWithDebInfo" ];

          nativeBuildInputs = [ cmake ninja pkgconf ];

          buildInputs = [
            OpenSiv3D.packages."x86_64-linux".default
            zlib
            alsa-lib
            ffmpeg
            boost175
            giflib
            mesa
            libmpg123
            opencv
            opusfile
            soundtouch
            libtiff
            libwebp
            util-linux
            xorg.libXft
            xorg.libXdmcp
            xorg.libXtst
            libGLU
            libvorbis
            harfbuzz
            glib
            gtk3
            curl
            pcre
            pcre2
            libselinux
            libthai
            libsepol
            libdatrie
            libxkbcommon
            libepoxy
          ];
        };
    in { defaultPackage."x86_64-linux" = IAmNotBunkei; };
}
