#pragma once

#include "ezvm/ezval.h"

class ezIO {
	public:
		static void load(char*** symtab, ezValue*** constants);
};
