/* =============================================================================
 * Vader Modular Fuzzer
 * Copyright (c) 2021-2023 The Charles Stark Draper Laboratory, Inc.
 * <vader@draper.com>
 *
 * Effort sponsored by the U.S. Government under Other Transaction number
 * W9124P-19-9-0001 between AMTC and the Government. The U.S. Government
 * is authorized to reproduce and distribute reprints for Governmental purposes
 * notwithstanding any copyright notation thereon.
 *
 * The views and conclusions contained herein are those of the authors and
 * should not be interpreted as necessarily representing the official policies
 * or endorsements, either expressed or implied, of the U.S. Government.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * @license GPL-3.0-or-later <https://spdx.org/licenses/GPL-3.0-or-later>
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

#include "byteSequenceMutations.hpp"


void vader::radamsa::mutations::ByteSequenceMutations::RepeatByteSequence(
                                                    StorageEntry* newEntry,
                                                    const size_t originalSize,
                                                    const char* originalBuffer,
                                                    const size_t minimumSeedIndex,
                                                    const int testCaseKey)
{
    // Consume the original buffer by repeating a sequence of bytes a random number of times and appending a null-terminator to the end.

    constexpr size_t minimumSize{2u};

    if (originalSize < minimumSize)
        throw RuntimeException{"The buffer's minimum size must be greater than or equal to 2", RuntimeException::USAGE_ERROR};

    if (minimumSeedIndex > originalSize - 2u)
        throw RuntimeException{"Minimum seed index is out of bounds", RuntimeException::INDEX_OUT_OF_RANGE};

    if (originalBuffer == nullptr)
        throw RuntimeException{"Input buffer is null", RuntimeException::UNEXPECTED_ERROR};

    // The new buffer size will contain a random number of additional elements since we are repeating a random byte.
    // Furthermore, it will contain one more element since we are appending a null-terminator to the end.

    const size_t numberOfRandomByteSequenceRepetitions{GetRandomByteSequenceRepetitionLength()};
    const size_t lower{0u};
    const size_t upper{originalSize - 1u};
    const size_t maximumRandomIndexValue{originalSize - minimumSeedIndex};

    // Select a random index from which the new bytes will be repeated.
    const size_t randomByteSequenceStartIndex{
                                    std::clamp(
                                            GetRandomValueWithinBounds(
                                                                    lower,
                                                                    maximumRandomIndexValue) + minimumSeedIndex,
                                                                    lower,
                                                                    upper)};
    // Get random end index
    const size_t randomByteSequenceEndIndex{
                                    std::clamp(
                                            GetRandomValueWithinBounds(
                                                                    lower + randomByteSequenceStartIndex + 1u,
                                                                    maximumRandomIndexValue) + minimumSeedIndex,
                                                                    lower,
                                                                    upper)};
    const size_t sequenceLength{randomByteSequenceEndIndex - randomByteSequenceStartIndex};
    const size_t newBufferSize{originalSize + (numberOfRandomByteSequenceRepetitions * sequenceLength) + 1u};

    // Allocate the new buffer and set it's elements to zero.

    char* newBuffer{newEntry->allocateBuffer(testCaseKey, static_cast<int>(newBufferSize))};
    memset(newBuffer, 0u, newBufferSize);

    // Copy data from the original buffer into the new buffer, but repeat the target byte sequence.
    // The last element in the new buffer is skipped since it was implicitly set to zero during allocation.

    for (size_t sourceIndex{0u}, destinationIndex{0u}; sourceIndex < originalSize; ++sourceIndex)
    {
        if (sourceIndex == randomByteSequenceEndIndex)
        {
            // Each sequence repetition
            for (size_t repetitionIndex{0u}; repetitionIndex < numberOfRandomByteSequenceRepetitions; ++repetitionIndex)
            {
                // Copy byte sequence
                memcpy(
                        &newBuffer[destinationIndex],
                        &originalBuffer[randomByteSequenceStartIndex],
                        sequenceLength);
                ++destinationIndex; 
            }
        }
        else
        {
            // Copy original byte
            newBuffer[destinationIndex] = originalBuffer[sourceIndex];

            ++destinationIndex;
        }
    }
}