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
#include "ezfile.h"
#include <cstddef>
#include <cstdint>

/**
 * @brief Global segment: Values are accessed globally. They are altered any
 * time.
 * scope.
 */
#define EZ_ASM_SEGMENT_GLOBAL 0
/**
 * @brief Constant segment: Constant values are set on compiling and cannot be
 * altered after.
 */
#define EZ_ASM_SEGMENT_CONSTANT 1
/**
 * @brief Local segment: This segment is available within a procedure only.
 */
#define EZ_ASM_SEGMENT_LOCAL 2
/**
 * @brief temporary segment: This segment is for temporary variables.
 */
#define EZ_ASM_SEGMENT_TEMPORARY 3
/**
 * @brief Parent segment: The scope of a caller.
 */
#define EZ_ASM_SEGMENT_SCOPE 4

/**
 * @brief ezAddress describes a memory address.
 */
class ezAddress {
public:
  /**
   * @brief A default constructor.
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
  void dump(ezFile &sink) {
    switch (segment) {
    case EZ_ASM_SEGMENT_CONSTANT:
      sink.print(" c");
      break;
    case EZ_ASM_SEGMENT_LOCAL:
      sink.print(" r");
      break;
    case EZ_ASM_SEGMENT_TEMPORARY:
      sink.print(" t");
      break;
    case EZ_ASM_SEGMENT_SCOPE:
      sink.print(" s");
      break;
    case EZ_ASM_SEGMENT_GLOBAL:
      sink.print(" g");
      break;
    default:
      sink.print(" %d", segment);
    }
    sink.print("%u", offset);
  }
};
