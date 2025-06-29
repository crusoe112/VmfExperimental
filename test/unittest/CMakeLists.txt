#===============================================================================
# Vader Modular Fuzzer (VMF)
# Copyright (c) 2021-2024 The Charles Stark Draper Laboratory, Inc.
# <vmf@draper.com>
#  
# Effort sponsored by the U.S. Government under Other Transaction number
# W9124P-19-9-0001 between AMTC and the Government. The U.S. Government
# Is authorized to reproduce and distribute reprints for Governmental purposes
# notwithstanding any copyright notation thereon.
#  
# The views and conclusions contained herein are those of the authors and
# should not be interpreted as necessarily representing the official policies
# or endorsements, either expressed or implied, of the U.S. Government.
#  
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 (only) as 
# published by the Free Software Foundation.
#  
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#  
# @license GPL-2.0-only <https://spdx.org/licenses/GPL-2.0-only.html>
#===============================================================================

# required by gtest
find_package(Threads REQUIRED)

SET(TEST_SRCS
  ${CMAKE_INSTALL_PREFIX}/../../test/unittest/ModuleTestHelper.cpp
  ${CMAKE_INSTALL_PREFIX}/../../test/unittest/TestConfigInterface.cpp
  ../../Radamsa/test/RadamsaDecrementByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaDropByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaFlipByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaIncrementByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaInsertByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaPermuteByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaRandomizeByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaRepeatByteMutatorTest.cpp
  ../../Radamsa/test/RadamsaDeleteLineMutatorTest.cpp
  ../../Radamsa/test/RadamsaDeleteSequentialLinesMutatorTest.cpp
  ../../Radamsa/test/RadamsaDuplicateLineMutatorTest.cpp
  ../../Radamsa/test/RadamsaCopyLineCloseByMutatorTest.cpp
  ../../Radamsa/test/RadamsaRepeatLineMutatorTest.cpp
  ../../Radamsa/test/RadamsaSwapLineMutatorTest.cpp
  ../../Radamsa/test/RadamsaRepeatByteSequenceMutatorTest.cpp
  ../../Radamsa/test/RadamsaDeleteByteSequenceMutatorTest.cpp
  ../../Radamsa/test/RadamsaPermuteLinesMutatorTest.cpp
  ../../Radamsa/test/RadamsaInsertLineMutatorTest.cpp
  ../../Radamsa/test/RadamsaReplaceLineMutatorTest.cpp
  ../../Radamsa/test/RadamsaTreeMutatorBaseTest.cpp
  ../../Radamsa/test/RadamsaDeleteNodeMutatorTest.cpp
  ../../Radamsa/test/RadamsaDuplicateNodeMutatorTest.cpp
  ../../Radamsa/test/RadamsaReplaceNodeMutatorTest.cpp
  ../../Radamsa/test/RadamsaSwapNodesMutatorTest.cpp
  ../../Radamsa/test/RadamsaRepeatPathMutatorTest.cpp
  ../../Radamsa/test/RadamsaWidenCodePointMutatorTest.cpp
  ../../Radamsa/test/RadamsaInsertUnicodeMutatorTest.cpp
  ../../Radamsa/test/RadamsaModifyTextNumberMutatorTest.cpp
  ../../Radamsa/test/RadamsaFuse_helpers.cpp
  ../../Radamsa/test/RadamsaFuseThisMutatorTest.cpp
  ../../Radamsa/test/RadamsaFuseNextMutatorTest.cpp
  ../../Radamsa/test/RadamsaFuseOldMutatorTest.cpp
  ../../Radamsa/test/RadamsaAsciiBadMutatorTest.cpp
)

add_executable(VmfTest ${TEST_SRCS})

set_target_properties(VmfTest PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(VmfTest PUBLIC
  ${CMAKE_INSTALL_PREFIX}/include
  ${CMAKE_INSTALL_PREFIX}/../../vmf
  ${CMAKE_INSTALL_PREFIX}/../../vmf/dependencies/googletest/googletest/include
  ${CMAKE_INSTALL_PREFIX}/../../test/unittest
  ${CMAKE_INSTALL_PREFIX}/../../vmf/src/framework/baseclasses
  ${CMAKE_INSTALL_PREFIX}/../../vmf/src/framework/util
  ../../Radamsa/vmf/src/modules/common/mutator
)

target_link_directories(VmfTest PUBLIC
  ${CMAKE_INSTALL_PREFIX}/lib
  ${CMAKE_INSTALL_PREFIX}/plugins
  ${CMAKE_INSTALL_PREFIX}/../lib
)

target_link_libraries(VmfTest
  PUBLIC
  gtest
  gtest_main
  VMFFramework
  CoreModules
  Threads::Threads
  Radamsa
  yaml-cpp
)
gtest_discover_tests(VmfTest)
