{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
  name = "Silicon";
  packages = with pkgs; [
    vulkan-headers 
    libxkbcommon.dev
    gtest.dev
    qt6.full
    qtcreator
    cmake
    yosys
    gdb
    ddd
    valgrind
  ];
  hardeningDisable = [ "all" ];
}

