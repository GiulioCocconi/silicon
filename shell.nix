{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
  name = "Silicon";
  packages = with pkgs; [ fltk14 gtest.dev cmake yosys gdb ddd ];
}

