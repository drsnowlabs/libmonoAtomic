cmake_minimum_required(VERSION 3.16)
project("monoAtomic"  VERSION 0.1 LANGUAGES CXX)


include(GNUInstallDirs)

add_library(monoAtomic INTERFACE)

target_include_directories(
  monoAtomic
  INTERFACE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
            $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)


target_compile_features(monoAtomic INTERFACE cxx_std_17)


install(TARGETS monoAtomic
        EXPORT monoAtomic_Targets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file("monoAtomicConfigVersion.cmake"
                                 VERSION ${PROJECT_VERSION}
                                 COMPATIBILITY SameMajorVersion)

configure_package_config_file(
    "${PROJECT_SOURCE_DIR}/cmake/monoAtomicConfig.cmake.in"
    "${PROJECT_BINARY_DIR}/monoAtomicConfig.cmake"
    INSTALL_DESTINATION
    ${CMAKE_INSTALL_DATAROOTDIR}/monoAtomic/cmake)

install(EXPORT monoAtomic_Targets
     FILE monoAtomicTargets.cmake
     NAMESPACE monoAtomic::
     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/monoAtomic/cmake)

install(FILES "${PROJECT_BINARY_DIR}/monoAtomicConfig.cmake"
           "${PROJECT_BINARY_DIR}/monoAtomicConfigVersion.cmake"
     DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/monoAtomic/cmake)

install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/monoAtomic DESTINATION include)
