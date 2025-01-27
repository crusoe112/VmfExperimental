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

    struct LineVector
    {
        ~LineVector() = default;
        LineVector() = default;

        LineVector(
            const char* const buffer,
            const Line& lineData)
        {
            Data = std::make_unique<char[]>(lineData.Size);
            memcpy(Data.get(), &buffer[lineData.StartIndex], lineData.Size);

            Size = lineData.Size;
        }

        LineVector(const LineVector &other) noexcept
        {
            // Copy Data

            const size_t& size{other.Size};

            Data = std::make_unique<char[]>(size);
            memcpy(Data.get(), other.Data.get(), size);

            Size = size;
        }

        LineVector(LineVector&& other) noexcept
        {
            // Copy Data

            const size_t& size{other.Size};

            Data = std::make_unique<char[]>(size);
            memcpy(Data.get(), other.Data.get(), size);

            Size = size;

            // Release ownership

            other.Data.release();
            other.Size = 0u;
        }

        LineVector& operator=(const LineVector& other)
        {
            // Copy Data

            const size_t& size{other.Size};

            Data = std::make_unique<char[]>(size);
            memcpy(Data.get(), other.Data.get(), size);

            Size = size;

            return *this;
        }

        LineVector& operator=(LineVector&& other)
        {
            // Copy Data

            const size_t& size{other.Size};

            Data = std::make_unique<char[]>(size);
            memcpy(Data.get(), other.Data.get(), size);

            Size = size;

            // Release ownership

            other.Data.release();
            other.Size = 0u;

            return *this;
        }

        bool operator==(const LineVector &other) const { 
            return (Size == other.Size && 
                    (memcmp(Data.get(), other.Data.get(), other.Size) == 0)); 
        }

        bool operator!=(const LineVector &other) const { return !(*this == other); } // we may want to change this to test for equality instead of identity

        std::unique_ptr<char[]> Data{nullptr};
        size_t Size{0u};
    };

    struct LineList
    {
        ~LineList() = default;
        LineList() = default;

        LineList(
            const char* const buffer,
            const std::vector<Line>& lineData) noexcept
        {
            const size_t numberOfElements{lineData.size()};

            auto data{std::make_unique<LineVector[]>(numberOfElements)};

            for(size_t it{0u}; it < numberOfElements; ++it)
            {
                const Line& line{lineData.at(it)};

                data[it] = std::move(LineVector{buffer,line});

                Capacity += line.Size;
            }

            NumberOfElements = numberOfElements;
            Data = std::move(data);
        }

        LineList(const LineList& other) noexcept
        {
            // Copy Data

            const size_t& numberOfElements{other.NumberOfElements};

            NumberOfElements = numberOfElements;
            Capacity = other.Capacity;

            Data = std::make_unique<LineVector[]>(numberOfElements);

            for (size_t it{0u}; it < numberOfElements; ++it)
            {
                Data[it] = other.Data[it];
            }
        }

        LineList(LineList&& other) noexcept
        {
            // Copy Data

            const size_t& numberOfElements{other.NumberOfElements};

            NumberOfElements = numberOfElements;
            Capacity = other.Capacity;

            Data = std::make_unique<LineVector[]>(numberOfElements);
            //memcpy(Data.get(), other.Data.get(), numberOfElements); //not legal
            int count = numberOfElements;
            for(int i=0; i<count; i++)
            {
                Data[i] = other.Data[i];
            }


            // Release ownership

            other.Data.release();
            other.Capacity = 0u;
            other.NumberOfElements = 0u;
        }

        LineList& operator=(const LineList& other)
        {
            // Copy Data

            const size_t& numberOfElements{other.NumberOfElements};

            NumberOfElements = numberOfElements;
            Capacity = other.Capacity;

            Data = std::make_unique<LineVector[]>(numberOfElements);
            //memcpy(Data.get(), other.Data.get(), numberOfElements); //not legal
            int count = numberOfElements;
            for(int i=0; i<count; i++)
            {
                Data[i] = other.Data[i];
            }

            return *this;
        }

        LineList& operator=(LineList&& other)
        {
            // Copy Data

            const size_t& numberOfElements{other.NumberOfElements};

            NumberOfElements = numberOfElements;
            Capacity = other.Capacity;

            Data = std::make_unique<LineVector[]>(numberOfElements);
            //memcpy(Data.get(), other.Data.get(), numberOfElements); //not legal
            int count = numberOfElements;
            for(int i=0; i<count; i++)
            {
                Data[i] = other.Data[i];
            }

            // Release ownership

            other.Data.release();
            other.Capacity = 0u;
            other.NumberOfElements = 0u;

            return *this;
        }

        bool operator==(const LineList& other) const
        {
            auto compareLineVectors{
                [&](const LineVector* const lhs, const LineVector* const rhs) -> bool
                {
                    if (lhs != nullptr && rhs != nullptr)
                    {
                        for (size_t it{0u}; it < lhs->Size; ++it)
                            if (lhs->Data[it] != rhs->Data[it])
                                return false;

                        return true;
                    }
                    else if (lhs == nullptr && rhs == nullptr)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
            };

            return ((NumberOfElements == other.NumberOfElements) &&
                    (Capacity == other.Capacity) &&
                    (compareLineVectors(Data.get(), other.Data.get())));
        }

        bool operator!=(const LineList& other) const { return !(*this == other); }

        std::unique_ptr<LineVector[]> Data{nullptr};
        size_t NumberOfElements{0u};
        size_t Capacity{0u};
    };

    RadamsaLineMutatorBase() = default;
    virtual ~RadamsaLineMutatorBase() = default;

    // line mutator helper functions go here
};
}