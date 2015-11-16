#include "ezintrinsic.h"
#include "ezio.h"

void ezIntrinsic::load(const string name, char*** symtab, ezValue*** constants) {
	*symtab = NULL;
	*constants = NULL;
	if(name == "io") ezIO::load(symtab, constants);
}

