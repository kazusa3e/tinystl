add_library(tinystl INTERFACE)
target_include_directories(tinystl INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_features(tinystl INTERFACE cxx_std_17)