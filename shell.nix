{
  pkgs ? import <nixpkgs> { },
}:
pkgs.llvmPackages_22.stdenv.mkDerivation {
  name = "devshell";
  nativeBuildInputs = [
    pkgs.meson
    pkgs.cmake
    pkgs.ninja
    pkgs.wayland-scanner
    pkgs.pkg-config
    pkgs.wayland
    pkgs.libxkbcommon
    pkgs.libffi
  ];
}
