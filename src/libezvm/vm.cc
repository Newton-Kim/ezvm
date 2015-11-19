#include "ezvm/ezvm.h"
#include "ezvm/ezlog.h"
#include <stdexcept>
#include <iostream>

using namespace std;

ezVM::ezVM() : m_pasm(NULL), m_parchive(NULL) {
	vector<ezValue*>* dummy = new vector<ezValue*>;
	m_globals.push_back(dummy);
}

ezVM::~ezVM() {
	if(m_pasm) delete m_pasm;
	if(m_parchive) delete m_parchive;
	for(vector< vector<ezValue*>* >::iterator it = m_globals.begin() ; it != m_globals.end() ; it++) {
		vector<ezValue*>* vct = *it;
		for(vector<ezValue*>::iterator subit = vct->begin() ; subit != vct->end() ; subit++) {
			ezValue* v = *subit;
			if(v->dynamic) {
				size_t ref = v->release();
				if(ref == 0) delete v;
			}
		}
		if(vct) delete vct;
	}
	for(vector<ezValue*>::iterator it = m_constants.begin() ; it != m_constants.end() ; it++) {
		ezValue* v = *it;
		if(v->dynamic) {
			size_t ref = v->release();
			if(ref == 0) delete v;
		}
	}
	for(vector<ezThread*>::iterator it = m_threads.begin() ; it != m_threads.end() ; it++)
		if(*it) delete *it;
}

void ezVM::run(void){
	ezLog& log = ezLog::instance();
	log.verbose("%s", __PRETTY_FUNCTION__);
	ezThread* thread = new ezThread(m_entry, m_globals, m_constants);
	m_threads.push_back(thread);
	log.debug("m_threads is %lu", m_threads.size());
	while(!m_threads.empty() && m_threads[0]) {
		size_t idx = 0, sz = m_threads.size();
		for (idx = 0 ; idx < sz ; idx++) {
			log.verbose("m_thread[%lu](%p) gets turn", idx, m_threads[idx]);
			ezThread* thd = m_threads[idx];
			if(thd == NULL) break;
			switch(thd->step()) {
				case EZ_STEP_DONE:
					delete thd;
					m_threads[idx] = NULL;
					for(size_t i = idx + 1 ; i < sz ; i++) {
						log.debug("m_threads[%lu] <- m_threads[%lu]", i - 1, i);
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

ezDump& ezVM::dump(void) {
	if(!m_pdump) m_pdump = new ezDump(m_entry, m_constants, m_globals);
	return *m_pdump;
}
