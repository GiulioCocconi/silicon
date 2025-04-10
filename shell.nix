{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
  name = "Silicon";
  packages = with pkgs; [
    vulkan-headers 
    libxkbcommon.dev
    gtest.dev
    qt6.full
    ninja
    cmake
    yosys
    gdb
    ddd
    valgrind
  ];
  hardeningDisable = [ "all" ];
}

