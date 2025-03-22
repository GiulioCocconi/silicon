{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
  name = "Silicon";
  packages = with pkgs; [ gtest.dev cmake yosys gdb ddd valgrind qt6.full ];
  hardeningDisable = [ "all" ];
}

