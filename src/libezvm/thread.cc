#include "ezvm/ezthread.h"

ezThread::ezThread(ezAddress entry, vector< vector<size_t>* > globals, vector<ezValue*> constants):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals)
{
}

ezThreadState ezThread::step(void) {
	return EZ_THREAD_DONE;
}
