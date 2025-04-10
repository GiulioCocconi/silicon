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
    clang-tools
    yosys
    gdb
    ddd
    valgrind
  ];
  hardeningDisable = [ "all" ];
  NIX_LANG_CPP = "TRUE";
}

