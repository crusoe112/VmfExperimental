/* =============================================================================
 * Vader Modular Fuzzer
 * Copyright (c) 2021-2023 The Charles Stark Draper Laboratory, Inc.
 * <vmf@draper.com>
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

#pragma once

#include "RadamsaMutatorBase.hpp"

namespace vmf
{
/**
 *
 */
class RadamsaByteMutatorBase: public RadamsaMutatorBase
{
public:
    static size_t GetRandomRepetitionLength(VmfRand* rand) noexcept
    {
        constexpr size_t MINIMUM_UPPER_LIMIT{0x2u};
        constexpr size_t MAXIMUM_UPPER_LIMIT{0x20000u};

        size_t randomStop{rand->randBetween(0u, MINIMUM_UPPER_LIMIT)};
        size_t randomUpperLimit{MINIMUM_UPPER_LIMIT};

        while(randomStop != 0u)
        {
            if(randomUpperLimit == MAXIMUM_UPPER_LIMIT)
                break;

            randomUpperLimit <<= 1u;
            randomStop = rand->randBetween(0u, MINIMUM_UPPER_LIMIT);
        }

        return rand->randBetween(0u, randomUpperLimit) + 1u; // We add one to the return value in order to account for the case where the random upper value is zero.
    }

    std::vector<uint8_t> encode_utf8(char32_t cp) {
        // encodes 21-bit character code points into UTF-8 values of 1 to 4 bytes

        std::vector<uint8_t> result;
        if (cp <= 0x7F) {           // 1B case
            result.push_back(cp);       
        } 
        else if (cp <= 0x7FF) {     // 2B case
            result.push_back(0xC0 | (cp >> 6));     // 110xxxxx, top 5b
            result.push_back(0x80 | (cp & 0x3F));   // 10xxxxxx, bottom 6b
        } else if (cp <= 0xFFFF) {  // 3B case
            result.push_back(0xE0 | (cp >> 12));            // 1110xxxx, top 4b
            result.push_back(0x80 | ((cp >> 6) & 0x3F));    // 10xxxxxx, next 6b
            result.push_back(0x80 | (cp & 0x3F));
        } else {                    // 4B case
            result.push_back(0xF0 | (cp >> 18));            // 11110xxx, top 3b
            result.push_back(0x80 | ((cp >> 12) & 0x3F));   // 10xxxxxx, next 6b
            result.push_back(0x80 | ((cp >> 6) & 0x3F));
            result.push_back(0x80 | (cp & 0x3F));
        }
        return result;
    }
};
}