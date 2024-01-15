# libmonoAtomic
Core audio library in c++

# Build tests
```
cd tests/build
CC=clang CXX=clang++ cmake .. && cmake --build .
```

The output executables will be under `build/bin`

- `bin/test_maAudioFileWave ../assets/splash.wav`