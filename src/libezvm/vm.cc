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
}

ezASM& ezVM::assembler(void) {
	if(!m_pasm) m_pasm = new ezASM(m_entry, m_constants, m_globals);
	return *m_pasm;
}

ezArchive& ezVM::archive(void) {
	if(!m_parchive) m_parchive = new ezArchive();
	return *m_parchive;
}
