![Silicon](./resources/banner.png)
An Open Source Suite for simulating Circuits, Finite State Machines and Microcontrollers (WIP)

## TODOs
Since it's a pre-alpha product, there are quite a lot of things to be done:

_Common_

- [ ] GUI with QT6

_Logic circuits (Silicon LogiFlow)_

- [ ] Multiplexers & demultiplexers
- [ ] Timed simulation
- [ ] Flip flops & synchronous components
- [ ] Wires-to-bus & bus-to-wires
- [ ] INPUTS & OUTPUTS!!!!
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

## Compiling develop edition
SILICON uses [Nix](https://nixos.org) and [CMAKE](https://cmake.org) in order to manage dependencies. It's recomended to use [Ninja Build](https://ninja-build.org) as a generator. Run the commands below to compile the develop edition of SILICON:

```
git clone github.com/GiulioCocconi/silicon
cd silicon
git checkout develop
nix-shell
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
ninja
```

Clangd uses the file `compile_commands.json`, that should be placed in the project root. CMAKE generates it in the build subdirectory so you need to symlink it:

```
cd <project-root>
ln -s ./build/compile_commands.json compile_commands.json
```
