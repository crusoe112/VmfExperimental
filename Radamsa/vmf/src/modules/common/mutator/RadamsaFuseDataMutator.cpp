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
 /**
  *
  */
#include "RadamsaFuseDataMutator.hpp"
#include "RuntimeException.hpp"
#include <random>
#include <algorithm>
#include <set>

using namespace vmf;
using std::pair;
using std::set;

#include "ModuleFactory.hpp"
REGISTER_MODULE(RadamsaFuseDataMutator);

/**
 * @brief Builder method to support the ModuleFactory
 * Constructs an instance of this class
 * @return Module* - Pointer to the newly created instance
 */
Module* RadamsaFuseDataMutator::build(std::string name)
{
    return new RadamsaFuseDataMutator(name);
}

/**
 * @brief Initialization method
 *
 * @param config - Configuration object
 */
void RadamsaFuseDataMutator::init(ConfigInterface& config)
{

}

/**
 * @brief Construct a new RadamsaFuseDataMutator::RadamsaFuseDataMutator object
 *
 * @param name The of the name module
 */
RadamsaFuseDataMutator::RadamsaFuseDataMutator(std::string name) : MutatorModule(name)
{
    // rand->randInit();
}

/**
 * @brief Destroy the RadamsaFuseDataMutator::RadamsaFuseDataMutator object
 *
 */
RadamsaFuseDataMutator::~RadamsaFuseDataMutator()
{

}

/**
 * @brief Register the storage needs for this module
 *
 * @param registry - StorageRegistry object
 */
void RadamsaFuseDataMutator::registerStorageNeeds(StorageRegistry& registry)
{
    // This module does not register for a test case buffer key, because mutators are told which buffer to write in storage
    // by the input generator that calls them
}

template<typename T>
pair<T*, T*> randomPair(vector<T>& data, VmfRand* rand) {
    T* elemA = &data[rand->randBetween(0, data.size() - 1)];
    T* elemB = &data[rand->randBetween(0, data.size() - 1)];

    return pair<T*, T*>(elemA, elemB);
}

template<typename T>
pair<vector<vector<T>>, vector<vector<T>>> getSuffixes(vector<T>& data, VmfRand* rand) {
    vector<vector<T>> new_listA, new_listB;
    vector<T> sliceA, sliceB;

    for (size_t i = 0; i < data.size(); ++i) {
        if (rand->randBetween(0, 1) == 0) {
            sliceA = vector<T>(data.begin() + i, data.end());

            if (!sliceA.empty()) {
                new_listB.push_back(sliceB);
                sliceB.clear();
            }
        }
        else {
            sliceB = vector<T>(data.begin() + i, data.end());

            if (!sliceB.empty()) {
                new_listA.push_back(sliceB);
                sliceA.clear();
            }
        }
    }

    return {new_listA, new_listB};
}

template<typename T> 
pair<vector<vector<T>>, vector<vector<T>>> splitPrefixes(const vector<vector<T>>& prefixes, const vector<vector<T>>& suffixes, VmfRand* rand) {
    vector<vector<T>> new_prefixes;
    vector<vector<T>> suffixes_copy = suffixes;

    set<T> charSuffix;          // tracks unique first elements in prefixes
    set<vector<T>> hashSuffix;  // collects suffixes shorter than len

    // assuming prefixes is sorted by length
    for (const vector<T> prefix : prefixes) {
        if (prefix.empty()) continue;

        const T key = prefix[0];
        if (charSuffix.find(key) == charSuffix.end()) { // if key wasn't found...
            new_prefixes.push_back(prefix);
            charSuffix.insert(key);
            
            // keep suffixes with length >= len, send others to hashSuffix
            size_t len = prefix.size() - 1;
            vector<vector<T>> keptSuffixes;
            for (vector<T> suffix : suffixes_copy) {
                if (suffix.size() < len) {
                    hashSuffix.insert(suffix);
                }
                else {
                    keptSuffixes.push_back(suffix);
                }
            }
            suffixes_copy = std::move(keptSuffixes);
        }
    }

    vector<vector<T>> new_suffixes(hashSuffix.begin(), hashSuffix.end());

    return {new_prefixes, new_suffixes};
}

template<typename T>
pair<vector<T>, vector<T>> findJumpPoints(vector<T>& data, VmfRand* rand) {
    size_t fuel = 100000;
    
    vector<vector<T>> suffixesA, suffixesB;
    std::tie(suffixesA, suffixesB) = getSuffixes(data, rand);
    //TODO error checking here?

    vector<T> from, to;
    std::tie(from, to) = randomPair(data, rand);
    

    while (true) {
        if (fuel < 0 || rand->randBetween(0, 8) == 0) {
            break;
        }
        // TODO
    }
}

void RadamsaFuseDataMutator::mutateTestCase(StorageModule& storage, StorageEntry* baseEntry, StorageEntry* newEntry, int testCaseKey)
{
    // TODO: Add the mutator desc here

    const size_t minimumSize{1u};
    const size_t minimumSeedIndex{0u};
    const size_t originalSize = baseEntry->getBufferSize(testCaseKey);
    char* originalBuffer = baseEntry->getBufferPointer(testCaseKey);

    if (originalSize < minimumSize) {
        throw RuntimeException{"The buffer's minimum size must be greater than or equal to 1", RuntimeException::USAGE_ERROR};
    }
    if (minimumSeedIndex > originalSize - 1u) {
        throw RuntimeException{"Minimum seed index is out of bounds", RuntimeException::INDEX_OUT_OF_RANGE};
    }
    if (originalBuffer == nullptr) {
        throw RuntimeException{"Input buffer is null", RuntimeException::UNEXPECTED_ERROR};
    }


}