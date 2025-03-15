{ pkgs ? import <nixpkgs> {}, ...}:
pkgs.mkShell {
	name = "Silicon";
	packages = with pkgs; [ fmt.dev cmake yosys gdb ddd ];
}

