# RISC-V Porting Workflow

## Build using CMake

```
mkdir build
cd build
cmake .. \
      -DOMR_COMPILER=ON -DOMR_TEST_COMPILER=ON \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
make -j4
```

Notes:

 * Unlike "Santa Clara" port, this port uses CMake-based build. Autotools-based build is *not supported*.
 * *CMake* supports (requires?) out-of-tree builds, so build happen in `build` subdirectory. All artifacts are there.
 * You *must* return `cmake ...` when you add/remove/rename files (generally, when you change `CMakeLists.txt`).
 * `-DCMAKE_BUILD_TYPE=Debug` is necessary to produce debug symbols
 * `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` is to produce a `compile_commands.json` used by Clang-based tools. Should not harm.

 ## Running `compilertest`

 Unlike in "Santa Clara" port, the test program is `build/compilertest/compilertest` and uses Google Test (rather than sequence of `printf()` lines in "Santa Clara" port. A new testsuite called "MinimalTest" has been added (see `MinimalTest.hpp` and `MinimalTest.cpp`. This has been done in order to have better (and automatable!) way of ensuring that new changes have no regressions.

 To see what tests are in minimal testsuite run:

 ```
 ./compilertest --gtest_list_tests --gtest_filter=MinimalTest*
 ```

 To run specific test, say `MeaningOfLife`:

 ```
 ./compilertest --gtest_filter=MinimalTest.MeaningOfLife
 ```

 To run all tests in minimal test suite:

 ```
 ./compilertest --gtest_filter=MinimalTest.*
 ```



