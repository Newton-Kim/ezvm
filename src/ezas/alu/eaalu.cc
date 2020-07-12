#include "eaalu.h"
#include "eabool.h"
#include "eaint.h"
#include "eafloat.h"
#include "eacplx.h"
#include "eastring.h"

eaALU::eaALU() {
	m_state[EZ_VALUE_TYPE_BOOL] = new eaALUBool;
	m_state[EZ_VALUE_TYPE_INTEGER] = new eaALUInteger;
	m_state[EZ_VALUE_TYPE_FLOAT] = new eaALUFloat;
	m_state[EZ_VALUE_TYPE_COMPLEX] = new eaALUComplex;
	m_state[EZ_VALUE_TYPE_STRING] = new eaALUString;
}

eaALU::~eaALU() {
	for(size_t i = 0 ; i < EZ_VALUE_TYPE_MAX ; i++) {
		delete m_state[i];
	}
}

ezValue* eaALU::add(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->add((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::subtract(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->subtract((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::multiply(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->multiply((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::divide(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->divide((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::modulate(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->modulate((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::power(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->power((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::bitwise_and(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->bitwise_and((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::bitwise_or(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->bitwise_or((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::bitwise_xor(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->bitwise_xor((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::lsl(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->lsl((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::lsr(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->lsr((eaValue*)argl, (eaValue*)argr);
}

ezObject* eaALU::compare(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->compare((eaValue*)argl, (eaValue*)argr);
}

ezObject* eaALU::condition(ezValue* arg) {
	return m_state[((eaValue*)arg)->id]->condition((eaValue*)arg);
}

bool eaALU::is_equal(ezValue* argl, ezValue* argr) {
	int id = max_id((eaValue*)argl, (eaValue*)argr);
	return m_state[id]->is_equal((eaValue*)argl, (eaValue*)argr);
}

ezValue* eaALU::negate(ezValue* arg) {
	return m_state[((eaValue*)arg)->id]->negate((eaValue*)arg);
}

ezValue* eaALU::bitwise_not(ezValue* arg) {
	return m_state[((eaValue*)arg)->id]->bitwise_not((eaValue*)arg);
}


