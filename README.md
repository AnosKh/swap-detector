# swapped-arg-checker

Module that checks for swapped arguments in function calls.

## Prerequisites
* CMake 3.10
* GCC 7 or Visual Studio 2017
* If enabling Clang plugin support: Clang 10 source

## Getting Started
### Setup for building Clang
* Check out Clang from git.
```bash
git clone https://github.com/llvm/llvm-project.git
git checkout llvmorg-10.0.0
```
* Build and install LLVM.
```bash
mkdir llvm-build; pushd llvm-build
cmake -G Ninja -DCMAKE_BUILD_TYPE=RelWithDebInfo -DLLVM_ENABLE_ASSERTIONS=1 -DLLVM_TARGETS_TO_BUILD=X86 -DLLVM_INSTALL_UTILS=1 -DLLVM_ENABLE_PROJECTS=clang -DLLVM_TOOL_CLANG_BUILD=1 -DCMAKE_INSTALL_PREFIX=$PWD/../llvm-install ../llvm-src
cmake --build . --target check-clang
cmake --build . --target install
popd
```
* [Install lit](https://pypi.org/project/lit/). Optional but required to be able to run tests.
```bash
pip install --user lit
export PATH=$PATH:$HOME/.local/bin
```
* Use cmake to build the plugin:
```bash
mkdir build; cd build
cmake -G Ninja -DLLVM_EXTERNAL_LIT=$(which lit) -DSWAPPED_ARGS_BUILD_CLANG_PLUGIN=ON -DCMAKE_PREFIX_PATH=$PWD/../../llvm-install/lib/cmake ~/path/to/swapped-arg-checker
cmake --build . --target check-all
```

#### Notes

If you wish to install Clang globally, omit`-DCMAKE_INSTALL_PREFIX` and `-DCMAKE_PREFIX_PATH`.

If you don't have Ninja installed, omit `-G Ninja` from the cmake invocations to generate makefiles instead. These are somewhat slower.

There are build errors relating to binding ‘llvm::json::Object’ lvalue to ‘llvm::json::Object&&’ with older versions of gcc (e.g. 4.8). This can be worked around by using clang as the compiler via `-DCMAKE_C_COMPILER` and `-DCMAKE_CXX_COMPILER`.

### Configuration Options
Option | Description
------ | -----------
`SWAPPED_ARGS_BUILD_CLANG_PLUGIN` | Enables building the Clang plugin. Default: ON
`SWAPPED_ARGS_BUILD_TESTS` | Enables building tests. Default: ON

### Automatic Downloads
As part of the CMake configuration, the latest master branch of [googletest]
(https://github.com/google/googletest) is downloaded and built if testing
functionality is enabled.

### Testing
To run the C++ unit tests, ensure that `SWAPPED_ARGS_BUILD_TESTS` is not
disabled when configuring the cmake project. The `TestSwappedArgsCpp` executable
will be generated on successful build and can be run to perform unit testing.
