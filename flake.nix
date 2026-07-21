{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };
  outputs = {nixpkgs, ...}: let
    system = "x86_64-linux";
    pkgs = nixpkgs.legacyPackages.${system};
    runtimeLibs = with pkgs; [
        wayland
        wayland-protocols
        libdecor
        libxkbcommon
        libGL
      ];
  in {
    devShells.${system}.default = pkgs.gcc16Stdenv.mkDerivation {
      name = "devshell";
      nativeBuildInputs = [
        # tools
        pkgs.cmake
        pkgs.meson
        pkgs.ninja
        pkgs.pkg-config
        pkgs.wayland-scanner

        # libraries
        pkgs.wayland
        pkgs.wayland-protocols
        pkgs.libdecor
        pkgs.libxkbcommon
        pkgs.libffi
      ];
    shellHook = ''
          export LD_LIBRARY_PATH="${pkgs.lib.makeLibraryPath runtimeLibs}:$LD_LIBRARY_PATH"
        '';
    };
  };
}
