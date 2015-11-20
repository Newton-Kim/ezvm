#include "ezvm/ezdump.h"
#include "ezinstruction.h"

using namespace std;

ezDump::ezDump(ezAddress& entry, vector<ezValue*>& constants, vector< vector<ezValue*>* >& globals, ezASM* pasm):
		m_entry(entry),
		m_constants(constants),
		m_globals(globals),
		m_asm(pasm) {
}

void ezDump::dump(ezFile& sink, const ezValue* v) {
	switch(v->type) {
		case EZ_VALUE_TYPE_NULL:
			sink.print("(nil)");
			break;
		case EZ_VALUE_TYPE_BOOL:
			if(((ezBool*)v)->value() == true) sink.print("true");
			else sink.print("false");
			break;
		case EZ_VALUE_TYPE_INTEGER:
			sink.print("%d", ((ezInteger*)v)->value());
			break;
		case EZ_VALUE_TYPE_STRING:
			sink.print("\"%s\"", ((ezString*)v)->value().c_str());
			break;
		case EZ_VALUE_TYPE_CAROUSEL:
			{
				ezCarousel* crsl = (ezCarousel*)v;
				sink.print("(%d) %d\n", crsl->nargs, crsl->nrets);
				sink.print("      .memsize: %lu\n", crsl->nmems);
				ezInstDecoder decoder;
				ezAddress addr;
				ezOpCode op;
				uint8_t arg[3];
				size_t len = crsl->instruction.size();
				size_t pc = 0;
				while(pc < len) {
					decoder.opcode(crsl->instruction[pc++], op, arg[0], arg[1], arg[2]);
					sink.print("      %s", decoder.opstr(op));
					sink.print(":%d:%d:%d", arg[0], arg[1], arg[2]);
					if(op == EZ_OP_CALL) {
						decoder.argument(crsl->instruction[pc++], addr);
						sink.print("[%d:%u]", addr.segment, addr.offset);
					}
					for(size_t c = 0 ; c < 3 ; c++) {
						if(!arg[c]) continue;
						sink.print(",");
						for(size_t i = 0 ; i < arg[c] ; i++) {
							decoder.argument(crsl->instruction[pc++], addr);
							switch(addr.segment) {
								case EZ_ASM_SEGMENT_CONSTANT:
									sink.print(" c");
									break;
								case EZ_ASM_SEGMENT_LOCAL:
									sink.print(" l");
									break;
								case EZ_ASM_SEGMENT_PARENT:
									sink.print(" p");
									break;
								case EZ_ASM_SEGMENT_GLOBAL:
									sink.print(" g");
									break;
								default:
									sink.print(" %d", addr.segment);
							}
							sink.print(":%u", addr.offset);
						}
					}
					sink.print("\n");
				}
			}
			break;
		case EZ_VALUE_TYPE_NATIVE_CAROUSEL:
			sink.print("(native)");
			break;
	}
	sink.print("\n");
}

void ezDump::dump(const string path) {
	ezFile sink(path, "wb");
	sink.print(".entry: %d:%u\n", m_entry.segment, m_entry.offset);
	sink.print(".global:\n");
	for(size_t i = 0 ; i < m_globals.size() ; i++) {
		sink.print("  segment %lu:\n", i);
		vector<ezValue*>* global = m_globals[i];
		for(size_t j = 0 ; j < global->size() ; j++) {
			sink.print("    [%lu]=", j);
			dump(sink, (*global)[j]);
		}
	}
	sink.print(".constant:\n");
	for(size_t i = 0 ; i < m_constants.size() ; i++) {
		sink.print("  [%lu]=", i);
		dump(sink, m_constants[i]);
	}
	if(m_asm) m_asm->dump(sink);
}
