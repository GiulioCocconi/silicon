![Silicon](./resources/banner.png)
An Open Source Suite for simulating Circuits, Finite State Machines and Microcontrollers (WIP)

## TODOs

Since it's a pre-alpha product, there are quite a lot of things to be done:

_Common_

- [ ] GUI with QT6
- [ ] Deployment (setup packages for Win & Mac). See [here](https://www.qt.io/blog/cmake-deployment-api).

_Logic circuits (Silicon LogiFlow)_

- [ ] Multiplexers & demultiplexers
- [ ] Timed simulation
- [ ] Flip flops & synchronous components
- [ ] Wires-to-bus & bus-to-wires
- [x] INPUTS & OUTPUTS!!!!
- [ ] Bus display
- [ ] 7-segment display
- [ ] Verilog support
    * [ ] [Slang](https://github.com/MikePopoloski/slang) for parsing
    * [ ] [Inja](https://github.com/pantor/inja) for reverse parsing (needs template file)
- [ ] File format
    * [ ] Implement [Quine-McCluskey method](https://en.wikipedia.org/wiki/Quine%E2%80%93McCluskey_algorithm) to minimize the combinatorial logic net
    * [ ] Using verilog & Slang?

_FSMs_

- [ ] TBD

_Microcontrollers_

- [ ] TBD

## Using clangd

This project is currently being developed using EMACS. The following packages will be useful:

- [lsp-mode](https://emacs-lsp.github.io/) for clangd integration.
- [dir-config.el](https://github.com/jamescherti/dir-config.el) to load this project specific config.

## Compiling develop edition

SILICON uses [Nix](https://nixos.org) and [CMAKE](https://cmake.org) in order to manage dependencies. It's recomended to use [Ninja Build](https://ninja-build.org) as a generator. Run the commands below to compile the develop edition of SILICON:

```shell
git clone https://github.com/GiulioCocconi/silicon
cd silicon
git checkout develop
nix-shell
mkdir build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug -Bbuild
ninja -C build
```

Clangd uses the file `compile_commands.json`, that should be placed in the project root. CMAKE generates it in the build subdirectory so you need to symlink it:

```
cd <project-root>
ln -s ./build/compile_commands.json compile_commands.json
```

In order to be able to use the `git hooks` written for this project you need to set a specific config value (it's local to the project so you shouldn't need to worry):

```shell
git config core.hooksPath hooks/
```