#include "ezio.h"
#include "ezvm/ezval.h"
#include <iostream>
#include <sstream>
#include <stdexcept>

class ezIoPrint : public ezNativeCarousel {
	public:
		ezIoPrint();
		void run(vector<ezValue*>& args, vector<ezValue*>& rets);
};

ezIoPrint::ezIoPrint() : ezNativeCarousel(false) { }

void ezIoPrint::run(vector<ezValue*>& args, vector<ezValue*>& rets) {
	rets.clear();
	stringstream ss;
	size_t len = args.size();
	if (args[0]->type != EZ_VALUE_TYPE_INTEGER) throw runtime_error("argument 1 is not a number");
	size_t ioidx = ((ezInteger*)args[0])->value();
	for(size_t i = 1 ; i < len ; i++) {
		ezValue* v = args[i];
		switch(v->type) {
			case EZ_VALUE_TYPE_INTEGER:
				ss << ((ezInteger*)v)->value();
				break;
			case EZ_VALUE_TYPE_STRING:
				ss << ((ezString*)v)->value();
				break;
			default:
				ss << hex << (void*)v << dec;
		}
	}
	ss << endl;
	switch(ioidx) {
		case 1:
			cout << ss.str();
			break;
		case 2:
			cerr << ss.str();
			break;
		default:
			throw runtime_error("invalid IO index");
			break;
	}
}

void ezIO::load(char*** symtab, ezValue*** constants){
	static ezIoPrint io_print;
	static const char* io_symtab[] = {"print", NULL};
	static ezValue* io_constants[] = {&io_print, NULL};
	*symtab = (char**)io_symtab;
	*constants = io_constants;
}
