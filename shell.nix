{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
  name = "Silicon";
  packages = with pkgs; [
    vulkan-headers 
    libxkbcommon.dev
    gtest.dev
    qt6.qtbase
    qt6.qtsvg
    ninja
    cmake
    clang-tools
    gdb
    ddd
    valgrind
    surelog
    yosys
  ];


  FONTCONFIG_FILE = pkgs.makeFontsConf {
    fontDirectories = with pkgs; [ quicksand ];
  };
  
  hardeningDisable = [ "all" ];
  NIX_LANG_CPP = "TRUE";
}

