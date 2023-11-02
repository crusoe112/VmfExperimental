/* =============================================================================
 * Vader Modular Fuzzer (VMF)
 * Copyright (c) 2021-2023 The Charles Stark Draper Laboratory, Inc.
 * <vader@draper.com>
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
/*****
 * The following includes code copied from the LibAFL_Legacy repository.
 * 
 *       american fuzzy lop++ - fuzzer header
 *  ------------------------------------
 *  Originally written by Michal Zalewski
 *  Now maintained by Marc Heuse <mh@mh-sec.de>,
 *                    Heiko Eißfeldt <heiko.eissfeldt@hexco.de>,
 *                    Andrea Fioraldi <andreafioraldi@gmail.com>,
 *                    Dominik Maier <mail@dmnk.co>
 *  Copyright 2016, 2017 Google Inc. All rights reserved.
 *  Copyright 2019-2020 AFLplusplus Project. All rights reserved.
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at:
 *    http://www.apache.org/licenses/LICENSE-2.0
 *  This is the Library based on AFL++ which can be used to build
 *  customized fuzzers for a specific target while taking advantage of
 *  a lot of features that AFL++ already provides.
 */

#include "AFLOverwriteCopyMutator.hpp"
#include "AFLDeleteMutator.hpp" //For static choose_block_len method
#include "RuntimeException.hpp"
#include <random>
#include <algorithm>

using namespace vader;

#include "ModuleFactory.hpp"
REGISTER_MODULE(AFLOverwriteCopyMutator);

/**
 * @brief Builder method to support the ModuleFactory
 * Constructs an instance of this class
 * @return Module* 
 */
Module* AFLOverwriteCopyMutator::build(std::string name)
{
    return new AFLOverwriteCopyMutator(name);
}

/**
 * @brief Initialization method
 * 
 * @param config 
 */
void AFLOverwriteCopyMutator::init(ConfigInterface& config)
{

}

/**
 * @brief Construct a new AFLOverwriteCopyMutator::AFLOverwriteCopyMutator object
 * 
 * @param name the name of the module
 */
AFLOverwriteCopyMutator::AFLOverwriteCopyMutator(std::string name) :
    MutatorModule(name)
{
    afl_rand_init(&rand);
}

/**
 * @brief Destroy the AFLOverwriteCopyMutator::AFLOverwriteCopyMutator object
 * 
 */
AFLOverwriteCopyMutator::~AFLOverwriteCopyMutator()
{

}

/**
 * @brief Registers storage needs
 * This class uses only the "TEST_CASE" key
 * 
 * @param registry 
 */
void AFLOverwriteCopyMutator::registerStorageNeeds(StorageRegistry& registry)
{
    testCaseKey = registry.registerKey("TEST_CASE", StorageRegistry::BUFFER, StorageRegistry::READ_WRITE);
}
 
StorageEntry* AFLOverwriteCopyMutator::createTestCase(StorageModule& storage, StorageEntry* baseEntry)
{

    int size = baseEntry->getBufferSize(testCaseKey);
    char* buffer = baseEntry->getBufferPointer(testCaseKey);

    if(size <= 0)
    {
        throw RuntimeException("AFLOverwriteCopyMutator mutate called with zero sized buffer", RuntimeException::USAGE_ERROR);
    }

    StorageEntry* newEntry = storage.createNewEntry();
    char* newBuff = newEntry->allocateBuffer(testCaseKey, size);
    // Copy base entry to new entry
    memcpy((void*)newBuff, (void*)buffer, size);

    if (size < 2) {
        return newEntry;
    }

    u32 copy_len = AFLDeleteMutator::choose_block_len(&rand, size - 1);
    u32 copy_from = afl_rand_below(&rand, size - copy_len + 1);
    u32 copy_to = afl_rand_below(&rand, size - copy_len + 1);

    if (copy_from != copy_to) {
        memmove(newBuff + copy_to, newBuff + copy_from, copy_len);
    }

    return newEntry;
}