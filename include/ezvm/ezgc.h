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

/**
* @brief ezGCClient is an abtract object which contains ezGCObject.
*/
class ezGCClient {
public:
  virtual ~ezGCClient(){};
  virtual void on_mark(void) = 0;
};

/**
* @brief ezGCObject is an abstract object which occupies an allocated heap
* memory.
*/
class ezGCObject {
private:
  bool m_mark;

protected:
  size_t m_size;

public:
  ezGCObject() : m_mark(false) {}
  virtual ~ezGCObject() {}
  /**
  * @brief returns current state of the mark.
  *
  * @return boolean value which implies wheather it is marked.
  */
  inline bool marked(void) { return m_mark; }
  /**
  * @brief sets the mark true.
  */
  inline void mark(void) { m_mark = true; }
  /**
  * @brief sets the mark false.
  */
  inline void unmark(void) { m_mark = false; }
  /**
  * @brief returns the size of the object.
  *
  * @return is the size of the object.
  */
  inline size_t size(void) { return m_size; }
};

/**
* @brief ezGC is a garbage collector.
*/
class ezGC {
private:
  /**
  * @brief m_clients is an array of ezGCClients.
  */
  vector<ezGCClient *> m_clients;
  /**
  * @brief m+memories is an array of ezGCObjects.
  */
  list<ezGCObject *> m_memories;
  size_t m_size;
  size_t m_prev_size;
  bool m_pause;
  void collect(void);

public:
  ezGC();
  ~ezGC();
  /**
  * @brief adds an instance of ezGCObject.
  *
  * @param v is an instance of ezGCObject.
  */
  void add(ezGCObject *v);
  /**
  * @brief adds an instance of ezGCClient.
  *
  * @param t is an instance of ezGCClient.
  */
  void subscribe(ezGCClient *t);
  /**
  * @brief forces to collect the garbage.
  */
  void force(void);
  /**
  * @brief pends the garbage collection.
  */
  void pause(void) { m_pause = true; }
  /**
  * @brief sets the garbage collection back to normal state.
  */
  void resume(void) { m_pause = false; }
  static ezGC &instance(void);
};
