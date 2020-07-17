#include "ezvm/ezsegment.h"

#include "ezvm/ezlog.h"

ezMemSegment::ezMemSegment(size_t size) {
  m_size = sizeof(*this);
  for (size_t i = 0; i < size; i++)
    m_memory.push_back(ezNull::instance());
}

ezMemSegment::~ezMemSegment() {
  // Each element in m_memory should be registered in ezGC.
}

void ezMemSegment::on_mark(void) {
  EZ_INFO("m_memory size: %d", m_memory.size());
  for (vector<ezObject *>::iterator it = m_memory.begin(); it != m_memory.end();
       it++) {
    ezObject *obj = *it;
    EZ_INFO("visiting 0x%x", obj);
    if (obj)
      obj->mark();
  }
}
