#include "ezvm/ezvm.h"

void ezVM::run(void){
}

ezASM& ezVM::assembler(void) {
	return *m_pasm;
}

ezArchive& ezVM::archive(void) {
	return *m_parchvie;
}
