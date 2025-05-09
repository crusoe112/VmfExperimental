/* =============================================================================
 * Vader Modular Fuzzer (VMF)
 * Copyright (c) 2021-2024 The Charles Stark Draper Laboratory, Inc.
 * <vmf@draper.com>
 *  
 * Effort sponsored by the U.S. Government under Other Transaction number
 * W9124P-19-9-0001 between AMTC and the Government. The U.S. Government
 * Is authorized to reproduce and distribute reprints for Governmental purposes
 * notwithstanding any copyright notation thereon.
 *  
 * The views and conclusions contained herein are those of the authors and
 * should not be interpreted as necessarily representing the official policies
 * or endorsements, either expressed or implied, of the U.S. Government.
 *  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 (only) as 
 * published by the Free Software Foundation.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *  
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *  
 * @license GPL-2.0-only <https://spdx.org/licenses/GPL-2.0-only.html>
 * ===========================================================================*/

#include "gtest/gtest.h"
#include "ModuleTestHelper.hpp"
#include "SimpleStorage.hpp"
#include "RadamsaTreeMutatorBase.hpp"
#include "RuntimeException.hpp"

using vmf::StorageModule;
using vmf::StorageRegistry;
using vmf::ModuleTestHelper;
using vmf::TestConfigInterface;
using vmf::SimpleStorage;
using vmf::StorageEntry;
using vmf::RadamsaTreeMutatorBase;
using vmf::BaseException;
using vmf::RuntimeException;
// using RadamsaTreeMutatorBase::Tree;

class RadamsaTreeMutatorBaseTest : public ::testing::Test {
  protected:
    RadamsaTreeMutatorBase::Tree tr;

    RadamsaTreeMutatorBaseTest() {}
    ~RadamsaTreeMutatorBaseTest() {}
};
/*
TEST_F(RadamsaTreeMutatorBaseTest, NoRoot)
{   
    std::string treeStr = "(G)";

    try{
        tr = RadamsaTreeMutatorBase::Tree(treeStr);
        ADD_FAILURE() << "No exception thrown";
    }
    catch (RuntimeException e)
    {
        EXPECT_EQ(e.getErrorCode(), e.UNEXPECTED_ERROR);
    }
    catch (BaseException e)
    {
        FAIL() << "Unexpected Exception thrown: " << e.getReason();
    }
}

TEST_F(RadamsaTreeMutatorBaseTest, DanglingCloseBracket)
{   
    std::string treeStr = "G)";

    try{
        tr = RadamsaTreeMutatorBase::Tree(treeStr);
        ADD_FAILURE() << "No exception thrown";
    }
    catch (RuntimeException e)
    {
        EXPECT_EQ(e.getErrorCode(), e.UNEXPECTED_ERROR);
    }
    catch (BaseException e)
    {
        FAIL() << "Unexpected Exception thrown: " << e.getReason();
    }
}

TEST_F(RadamsaTreeMutatorBaseTest, NonAlphaNumericValue)
{   
    std::string treeStr = "$";

    try{
        tr = RadamsaTreeMutatorBase::Tree(treeStr);
        ADD_FAILURE() << "No exception thrown";
    }
    catch (RuntimeException e)
    {
        EXPECT_EQ(e.getErrorCode(), e.UNEXPECTED_ERROR);
    }
    catch (BaseException e)
    {
        FAIL() << "Unexpected Exception thrown: " << e.getReason();
    }
}

TEST_F(RadamsaTreeMutatorBaseTest, UnmatchedOpenBracket)
{   
    std::string treeStr = "G(";

    try{
        tr = RadamsaTreeMutatorBase::Tree(treeStr);
        ADD_FAILURE() << "No exception thrown";
    }
    catch (RuntimeException e)
    {
        EXPECT_EQ(e.getErrorCode(), e.UNEXPECTED_ERROR);
    }
    catch (BaseException e)
    {
        FAIL() << "Unexpected Exception thrown: " << e.getReason();
    }
}*/

TEST_F(RadamsaTreeMutatorBaseTest, JustRoot)
{   
    const std::string treeStr = "G";

    try{
        // tr = std::move(RadamsaTreeMutatorBase::Tree(treeStr));
        RadamsaTreeMutatorBase::Tree tr;
        tr = RadamsaTreeMutatorBase::Tree(treeStr);
        // throw std::runtime_error("after tr instanciation");      //      TODO deleteme
    }
    catch (BaseException e)
    {
        FAIL() << "Exception thrown: " << e.getReason();
    }

    // throw std::runtime_error("root not created");      //      TODO deleteme
    // throw std::runtime_error(tr.root->value);      //      TODO deleteme
    // std::stringstream ss; ss << tr.root; throw std::runtime_error("root addr: " + ss.str());      //      TODO deleteme

    EXPECT_EQ(treeStr, tr.toString(tr.root));
}

/*

TEST_F(RadamsaTreeMutatorBaseTest, OneNode)
{   
    GTEST_SKIP();
    std::string buffString = "4";

    StorageEntry* baseEntry = storage->createNewEntry();
    StorageEntry* modEntry = storage->createNewEntry();
    char* modBuff;

    const size_t buff_len = buffString.length();
    char* buff = baseEntry->allocateBuffer(testCaseKey, buff_len);
    for(size_t i{0}; i < buff_len; ++i) {
        buff[i] = buffString[i];
    }

    try{
        theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
        modBuff = modEntry->getBufferPointer(testCaseKey);
    } 
    catch (BaseException e)
    {
        FAIL() << "Exception thrown: " << e.getReason();
    }

    size_t modBuff_len = modEntry->getBufferSize(testCaseKey);
    std::string modString = std::string(modBuff);

    // test buff len
    EXPECT_TRUE(
        modBuff_len == buff_len + 1 - 1 ||
        modBuff_len == buff_len + 1 - 3
    );
    // test buff contents
    EXPECT_EQ(modString[0], '\0');
}

TEST_F(RadamsaTreeMutatorBaseTest, OneChild)
{   
    GTEST_SKIP();
    std::string buffString = "43(12)";

    StorageEntry* baseEntry = storage->createNewEntry();
    StorageEntry* modEntry = storage->createNewEntry();
    char* modBuff;

    const size_t buff_len = buffString.length();
    char* buff = baseEntry->allocateBuffer(testCaseKey, buff_len);
    for(size_t i{0}; i < buff_len; ++i) {
        buff[i] = buffString[i];
    }

    try{
        theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
        modBuff = modEntry->getBufferPointer(testCaseKey);
    } 
    catch (BaseException e)
    {
        FAIL() << "Exception thrown: " << e.getReason();
    }

    size_t modBuff_len = modEntry->getBufferSize(testCaseKey);
    std::string modString = std::string(modBuff);

    // test buff len
    EXPECT_TRUE(
        modBuff_len == buff_len + 1 - 2 ||
        modBuff_len == buff_len + 1 - 4
    );
    // test buff contents
    EXPECT_TRUE(
        modString == "43" ||
        modString == "12"
    );
}

TEST_F(RadamsaTreeMutatorBaseTest, TwoChildren)
{   
    GTEST_SKIP();
    std::string buffString = "43(12)(56)";

    StorageEntry* baseEntry = storage->createNewEntry();
    StorageEntry* modEntry = storage->createNewEntry();
    char* modBuff;

    const size_t buff_len = buffString.length();
    char* buff = baseEntry->allocateBuffer(testCaseKey, buff_len);
    for(size_t i{0}; i < buff_len; ++i) {
        buff[i] = buffString[i];
    }

    try{
        theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
        modBuff = modEntry->getBufferPointer(testCaseKey);
    } 
    catch (BaseException e)
    {
        FAIL() << "Exception thrown: " << e.getReason();
    }

    size_t modBuff_len = modEntry->getBufferSize(testCaseKey);
    std::string modString = std::string(modBuff);

    // test buff len
    EXPECT_TRUE(
        modBuff_len == buff_len + 1 - 2 ||
        modBuff_len == buff_len + 1 - 4
    );
    // test buff contents
    EXPECT_TRUE(
        modString == "43()(56)" ||    // left child delete
        modString == "43(12)" ||    // right child delete
        modString == "56(12)"       // root delete
    );
}

TEST_F(RadamsaTreeMutatorBaseTest, TwoChildren_OneGrandchild)
{   
    GTEST_SKIP();
    std::string buffString = "43(12)(56(50))";

    StorageEntry* baseEntry = storage->createNewEntry();
    StorageEntry* modEntry = storage->createNewEntry();
    char* modBuff;

    const size_t buff_len = buffString.length();
    char* buff = baseEntry->allocateBuffer(testCaseKey, buff_len);
    for(size_t i{0}; i < buff_len; ++i) {
        buff[i] = buffString[i];
    }

    try{
        theMutator->mutateTestCase(*storage, baseEntry, modEntry, testCaseKey);
        modBuff = modEntry->getBufferPointer(testCaseKey);
    } 
    catch (BaseException e)
    {
        FAIL() << "Exception thrown: " << e.getReason();
    }

    size_t modBuff_len = modEntry->getBufferSize(testCaseKey);
    std::string modString = std::string(modBuff);

    // test buff len
    EXPECT_TRUE(
        modBuff_len == buff_len + 1 - 2 ||
        modBuff_len == buff_len + 1 - 4
    );
    // test buff contents
    EXPECT_TRUE(
        modString == "43()(56(50))" ||  // left child delete
        modString == "43(12)(50)" ||    // right child delete
        modString == "43(12)(56)" ||    // grandchild delete
        modString == "50(12)(56)"       // root delete
    );
}*/