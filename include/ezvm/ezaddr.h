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

#define EZ_ASM_SEGMENT_CONSTANT ((int8_t) - 1)
#define EZ_ASM_SEGMENT_LOCAL ((int8_t) - 2)
#define EZ_ASM_SEGMENT_PARENT ((int8_t) - 3)
#define EZ_ASM_SEGMENT_GLOBAL ((int8_t) 0)

class ezAddress {
 public:
  ezAddress() : segment(EZ_ASM_SEGMENT_CONSTANT), offset(0) {}
  ezAddress(const ezAddress& addr)
      : segment(addr.segment), offset(addr.offset) {}
  ezAddress(int8_t seg, size_t off) : segment(seg), offset(off) {}
  int8_t segment;
  size_t offset;
};
