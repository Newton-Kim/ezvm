#include "ezvm/ezvm.h"
#include <stdexcept>

using namespace std;

ezVM::ezVM() : m_pasm(NULL), m_parchive(NULL) {
	vector<size_t>* dummy = new vector<size_t>;
	m_globals.push_back(dummy);
}

ezVM::~ezVM() {
	if(m_pasm) delete m_pasm;
	if(m_parchive) delete m_parchive;
	for(vector<ezValue*>::iterator it = m_constants.begin() ; it != m_constants.end() ; it++)
		if((*it)->dynamic) delete *it;
	for(vector< vector<size_t>* >::iterator it = m_globals.begin() ; it != m_globals.end() ; it++)
		if(*it) delete *it;
}

void ezVM::run(void){
	ezThread* thread = new ezThread(m_entry, m_globals, m_constants);
	m_threads.push_back(thread);
	while(!m_threads.empty()) {
		size_t idx = 0;
		for (idx = 0 ; idx < m_threads.size() ; idx++) {
			ezThread* thd = m_threads[idx];
			if(thd == NULL) break;
			switch(thd->step()) {
				EZ_THREAD_DONE:
					delete thd;
					for(size_t i = idx ; i < m_threads.size() - 1 ; i++)
						m_threads[i] = m_threads[i + 1];
					break;
			}
		}
	}
}

ezASM& ezVM::assembler(void) {
	if(!m_pasm) m_pasm = new ezASM(m_entry, m_constants, m_globals);
	return *m_pasm;
}

ezArchive& ezVM::archive(void) {
	if(!m_parchive) m_parchive = new ezArchive();
	return *m_parchive;
}
