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

#include "mutationBase.hpp"

namespace vmf
{
/**
 *
 */
class RadamsaLineMutatorBase: public MutationBase
{
public:
    struct Line
    {
        Line() = default;
        ~Line() = default;

        Line(Line&&) = default;
        Line(const Line&) = default;

        Line& operator=(Line&&) = default;
        Line& operator=(const Line&) = default;

        bool operator==(const Line &other) const { 
            return (IsValid == other.IsValid && 
                    StartIndex == other.StartIndex && 
                    Size == other.Size); 
        }
        bool operator!=(const Line &other) const { return !(*this == other); } // we may want to change this to test for equality instead of identity

        bool IsValid{false};
        size_t StartIndex{0u};
        size_t Size{0u};
    };

    // struct LineVector

    // struct LineList

    RadamsaLineMutatorBase() = delete;
    virtual ~RadamsaLineMutatorBase() = default;

    // line mutator helper functions go here
};
}