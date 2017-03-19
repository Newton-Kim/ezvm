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
#include <list>
#include <vector>
#define EZGC_THRESHOLD (20 * 1024)

using namespace std;

class ezGCClient {
public:
  virtual ~ezGCClient(){};
  virtual void on_mark(void) = 0;
};

class ezGCObject {
private:
  bool m_mark;

protected:
  size_t m_size;

public:
  ezGCObject() : m_mark(false) {}
  virtual ~ezGCObject() {}
  inline bool marked(void) { return m_mark; }
  inline void mark(void) { m_mark = true; }
  inline void unmark(void) { m_mark = false; }
  virtual size_t size(void) { return m_size; }
};

class ezGC {
private:
  vector<ezGCClient *> m_clients;
  list<ezGCObject *> m_memories;
  size_t m_size;
  size_t m_prev_size;
  bool m_pause;
  void collect(void);

public:
  ezGC();
  ~ezGC();
  void add(ezGCObject *v);
  void subscribe(ezGCClient *t);
  void force(void);
  void pause(void) { m_pause = true; }
  void resume(void) { m_pause = false; }
};
