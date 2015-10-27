#include "ezio.h"
#include "ezvm/ezval.h"
#include <iostream>

class ezIoPrint : public ezNativeCarousel {
	public:
		ezIoPrint();
		void run(vector<ezValue*>& args, vector<ezValue*>& rets);
};

ezIoPrint::ezIoPrint() : ezNativeCarousel(false) { }

void ezIoPrint::run(vector<ezValue*>& args, vector<ezValue*>& rets) {
	rets.clear();
	for(vector<ezValue*>::iterator it = args.begin() ; it != args.end() ; it++) {
		ezValue* v = *it;
		switch(v->type) {
			case EZ_VALUE_TYPE_INTEGER:
				cout << ((ezInteger*)v)->value();
				break;
			case EZ_VALUE_TYPE_STRING:
				cout << ((ezString*)v)->value();
				break;
			default:
				cout << hex << (void*)v << dec;
		}
	}
	cout << endl;
}

void ezIO::load(char*** symtab, ezValue*** constants){
	static ezIoPrint io_print;
	static const char* io_symtab[] = {"print", NULL};
	static ezValue* io_constants[] = {&io_print, NULL};
	*symtab = (char**)io_symtab;
	*constants = io_constants;
}
