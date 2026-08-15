cmake_minimum_required(VERSION 3.20)
project(SpellShuffler LANGUAGES CXX)

add_library(SpellShuffler SHARED
    src/dllmain.cpp
	src/hook.cpp
	src/PlayerIns.cpp
	src/GameDataMan.cpp
	src/MagicParam.cpp
)

# RTTIHook requires C++17 or newer.
target_compile_features(SpellShuffler PRIVATE cxx_std_17)

target_include_directories(SpellShuffler PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
