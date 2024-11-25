#===============================================================================
# Vader Modular Fuzzer (VMF) Test Configuration
#===============================================================================

# Specify the path to the GoogleTest built by the original repository
#set(GTEST_ROOT ${VMF_INSTALL})

# Find GoogleTest
#find_package(GTest REQUIRED PATHS ${GTEST_ROOT} NO_DEFAULT_PATH)

# Add the test source files
add_executable(radamsaMutatorTest
  radamsaMutatorTestNew.cpp
)

# Link the test executable with the necessary libraries
target_link_libraries(radamsaMutatorTest
  PUBLIC
    VMFFramework
    CoreModules
    gtest_main
)

# Build-time dependencies for Radamsa
target_include_directories(radamsaMutatorTest PRIVATE
  ${VMF_INSTALL}/include
  ${VMF_INSTALL}/include/vmf
  ${VMF_INSTALL}/include/plog
  ${PROJECT_SOURCE_DIR}/src/module
)

# Include GoogleTest
include(GoogleTest)
gtest_add_tests(TARGET radamsaMutatorTest)