cmake_minimum_required(VERSION 3.16)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)
set( CMAKE_CXX_FLAGS "-O3")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

project("monoAtomic_tests"  VERSION 0.1 LANGUAGES CXX)

add_executable(test_maAudioFileWave test_maAudioFileWave.cpp)
add_executable(test_maAudioDevicePortAudio test_maAudioDevicePortAudio.cpp)
add_subdirectory("../" monoAtomic)
add_subdirectory("../include/monoAtomic/portaudio" portaudio)

target_link_libraries(test_maAudioFileWave PRIVATE monoAtomic)
target_link_libraries(test_maAudioDevicePortAudio PRIVATE monoAtomic PortAudio)
