#pragma once
#include <cstddef>
#include <string>
#include <vector>

using namespace std;

enum ezValueType {
	EZ_VALUE_TYPE_INTEGER,
	EZ_VALUE_TYPE_STRING,
	EZ_VALUE_TYPE_CAROUSEL,
	EZ_VALUE_TYPE_NATIVE_CAROUSEL,
	EZ_VALUE_TYPE_MAX
};

class ezValue {
	private:
		size_t m_reference;
	public:
		const ezValueType type;
		ezValue(ezValueType tp): type(tp) {};
		virtual ~ezValue(){};
		void reference(void) { m_reference++; }
		void release(void) { m_reference--; }
};

class ezInteger : ezValue {
	private:
		const int m_value;
	public:
		ezInteger(int val);
		const int value(void);
};

class ezString : ezValue {
	private:
		const string m_value;
	public:
		ezString(const string val);
		const string value(void);
};

typedef unsigned int ezInstruction;
class ezCarousel : ezValue {
	private:
		vector<ezInstruction> m_instruction;
	public:
		ezCarousel();
};

class ezNativeCarousel : ezValue {
	private:
	public:
		ezNativeCarousel();
		void run(vector<ezValue*>& args, vector<ezValue*>& rets);
		
};
