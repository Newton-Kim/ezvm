#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <map>
#include <cstdint>

using namespace std;

enum ezValueType {
	EZ_VALUE_TYPE_NULL = 0,
	EZ_VALUE_TYPE_CONDITION,
	EZ_VALUE_TYPE_BOOL,
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
		const bool dynamic;
		ezValue(const ezValueType tp, const bool dyn);
		virtual ~ezValue();
		size_t reference(void);
		size_t release(void);
		virtual ezValue* duplicate(void);
		virtual void add(ezValue* v);
		virtual ezValue* condition(void);
};

class ezCondition: public ezValue {
	public:
		const bool zero;
		const bool negative;
		const bool overflow;
		const bool carry;
		ezCondition(const bool zr, const bool neg, const bool ovf, const bool cry, const bool dynamic = true);
};

class ezNull: public ezValue {
	public:
		ezNull();
		static ezNull* instance(void);
};

class ezBool: public ezValue {
	private:
		bool m_value;
	public:
		ezBool(bool val, const bool dynamic = true);
		const bool value(void);
		ezValue* condition(void);
};

class ezInteger : public ezValue {
	private:
		int m_value;
	public:
		ezInteger(int val, const bool dynamic = true);
		const int value(void);
		ezValue* duplicate(void);
		void add(ezValue* v);
		ezValue* condition(void);
};

class ezString : public ezValue {
	private:
		string m_value;
	public:
		ezString(const string val, const bool dynamic = true);
		const string value(void);
		ezValue* duplicate(void);
		void add(ezValue* v);
		ezValue* condition(void);
};

typedef	uint32_t ezInstruction;

class ezCarousel : public ezValue {
	public:
		uint8_t nargs;
		uint8_t nrets;
		size_t nmems;
		ezCarousel(uint8_t args, uint8_t rets, size_t mems, const bool dynamic = true);
		~ezCarousel();
		vector<ezInstruction> instruction;
		vector<ezInstruction> jmptbl;
		map<string, size_t> symtab;
};

class ezNativeCarousel : public ezValue {
	private:
	public:
		ezNativeCarousel(const bool dynamic = true);
		virtual void run(vector<ezValue*>& args, vector<ezValue*>& rets) = 0;
		
};
