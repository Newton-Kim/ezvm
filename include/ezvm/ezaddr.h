/* Copyright (C) 2015 Newton Kim
*
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditiong:
*
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*
*/
#pragma once
#include <cstddef>
#include <cstdint>

/**
* @brief Constant segment: A value is set on compiling and cannot be altered.
*/
#define EZ_ASM_SEGMENT_CONSTANT 1
/**
* @brief Local segment: A value which is accessible within a procedure only.
*/
#define EZ_ASM_SEGMENT_LOCAL 2
/**
* @brief Parent segment: A value which on the scope of a caller.
*/
#define EZ_ASM_SEGMENT_PARENT 3
/**
* @brief Global segment: A value can be altered and is accessible from all
* scope.
*/
#define EZ_ASM_SEGMENT_GLOBAL 0

/**
* @brief ezAddress is a class that describes an address of ezVM.
*/
class ezAddress {
public:
  /**
  * @brief A default constructor: It points the first slot of the constant
  * segment.
  */
  ezAddress() : segment(EZ_ASM_SEGMENT_CONSTANT), offset(0) {}
  /**
  * @brief A copy constructor.
  *
  * @param addr is an adress descriptor.
  */
  ezAddress(const ezAddress &addr)
      : segment(addr.segment), offset(addr.offset) {}
  /**
  * @brief A constructor
  *
  * @param seg is a segment index.
  * @param off is an offset in the segment.
  */
  ezAddress(uint8_t seg, size_t off) : segment(seg), offset(off) {}
  /**
  * @brief A segment.
  */
  uint8_t segment;
  /**
  * @brief An offset of the segment.
  */
  size_t offset;
};
