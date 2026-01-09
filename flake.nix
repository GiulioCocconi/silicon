{
  description = "Silicon - Open Source Suite for simulating Circuits, FSMs and Microcontrollers";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
      in
      {
        devShells.default = pkgs.mkShell {
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

          hardeningDisable = [ "all" ];
          NIX_LANG_CPP = "TRUE";

          shellHook = ''
            echo "Silicon development environment"
            echo "CMake version: $(cmake --version | head -n1)"
            echo "Ninja version: $(ninja --version)"
            echo ""
            echo "To build:"
            echo "  mkdir -p build"
            echo "  cmake -G Ninja -Bbuild"
            echo "  ninja -C build"
          '';
        };

        packages.default = pkgs.stdenv.mkDerivation {
          pname = "silicon";
          version = "0.1.0-pre-alpha";

          src = ./.;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            qt6.wrapQtAppsHook
          ];

          buildInputs = with pkgs; [
            gtest.dev
            qt6.qtbase
            qt6.qtsvg
          ];

          cmakeFlags = [
            "-DSILICON_USE_VCPKG=OFF"
          ];

          doCheck = true;

          meta = with pkgs.lib; {
            description = "Open Source Suite for simulating Circuits, FSMs and Microcontrollers";
            homepage = "https://github.com/GiulioCocconi/silicon";
            license = licenses.gpl3;
            platforms = platforms.linux;
          };
        };
      }
    );
}