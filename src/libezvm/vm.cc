#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include <stdexcept>

using namespace std;

ezVM::ezVM() : m_pasm(NULL), m_parchive(NULL) {
	vector<ezValue*>* dummy = new vector<ezValue*>;
	m_globals.push_back(dummy);
}

ezVM::~ezVM() {
	if(m_pasm) delete m_pasm;
	if(m_parchive) delete m_parchive;
	for(vector<ezValue*>::iterator it = m_constants.begin() ; it != m_constants.end() ; it++)
		if((*it)->dynamic) delete *it;
	for(vector< vector<ezValue*>* >::iterator it = m_globals.begin() ; it != m_globals.end() ; it++) {
		vector<ezValue*>* vct = *it;
		for(vector<ezValue*>::iterator subit = vct->begin() ; subit != vct->end() ; subit++)
			if((*subit)->dynamic) delete *subit;
		if(*it) delete *it;
	}
	for(vector<ezThread*>::iterator it = m_threads.begin() ; it != m_threads.end() ; it++)
		if(*it) delete *it;
}

void ezVM::run(void){
	ezFile& log = ezLog::logger();
	log.print("%s", __PRETTY_FUNCTION__);
	ezThread* thread = new ezThread(m_entry, m_globals, m_constants);
	m_threads.push_back(thread);
	log.print("m_threads is %lu", m_threads.size());
	while(!m_threads.empty() && m_threads[0]) {
		size_t idx = 0, sz = m_threads.size();
		for (idx = 0 ; idx < sz ; idx++) {
			log.print("m_thread[%lu](%p) gets turn", idx, m_threads[idx]);
			ezThread* thd = m_threads[idx];
			if(thd == NULL) break;
			switch(thd->step()) {
				case EZ_STEP_DONE:
					delete thd;
					m_threads[idx] = NULL;
					for(size_t i = idx + 1 ; i < sz ; i++) {
						log.print("m_threads[%lu] <- m_threads[%lu]", i - 1, i);
						if(!m_threads[i]) break;
						m_threads[i] = m_threads[i + 1];
					}
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
