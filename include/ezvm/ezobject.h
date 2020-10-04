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
#include "ezgc.h"

enum ezObjectType {
  EZ_OBJECT_TYPE_NULL = 0,
  EZ_OBJECT_TYPE_CONDITION,
  EZ_OBJECT_TYPE_FUNCTION,
  EZ_OBJECT_TYPE_USER_DEFINED_FUNCTION,
  EZ_OBJECT_TYPE_HANDLE,
  EZ_OBJECT_TYPE_ARRAY,
  EZ_OBJECT_TYPE_VALUE,
  EZ_OBJECT_TYPE_MAX
};

class ezObject : public ezGCObject {
public:
  const ezObjectType type;
  const char* type_string(void);
  ezObject(ezObjectType type);
  virtual ~ezObject(){};
  virtual void dump(ezFile &sink);
};

class ezNull : public ezObject {
public:
  ezNull();
  static ezNull *instance(void);
  void dump(ezFile &sink);
};

class ezHandle : public ezObject {
public:
  unsigned int id;
  ezHandle(unsigned int identifier);
  virtual ~ezHandle(){};
  void dump(ezFile &sink);
};

class ezArray : public ezObject {
public:
  vector<ezObject *> data;
  ezArray();
  ~ezArray(){};
  void dump(ezFile &sink);
};
