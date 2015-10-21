#include "ezvm/ezvm.h"
#include <stdexcept>

using namespace std;

ezVM::ezVM() : m_pasm(NULL), m_parchive(NULL) {}

ezVM::~ezVM() {
	if(m_pasm) delete m_pasm;
	if(m_parchive) delete m_parchive;
	for(vector<ezValue*>::iterator it = m_constants.begin() ; it != m_constants.end() ; it++)
		delete *it;
}

void ezVM::run(void){
	if(m_entry.empty()) throw runtime_error("error : no entry point has been defined.");
}

ezASM& ezVM::assembler(void) {
	if(!m_pasm) m_pasm = new ezASM(m_entry);
	return *m_pasm;
}

ezArchive& ezVM::archive(void) {
	if(!m_parchive) m_parchive = new ezArchive();
	return *m_parchive;
}
