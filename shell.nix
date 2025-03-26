{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
  name = "Silicon";
  packages = with pkgs; [
    vulkan-headers 
    libxkbcommon.dev
    gtest.dev
    (enableDebugging qt6)
    qtcreator
    cmake
    yosys
    gdb
    ddd
    valgrind
  ];
  hardeningDisable = [ "all" ];
}

