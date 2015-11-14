#pragma once
#include <cstddef>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

enum ezValueType {
	EZ_VALUE_TYPE_NULL,
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
};

class ezNull: public ezValue {
	public:
		ezNull();
		static ezNull* instance(void);
};

class ezBool: public ezValue {
	private:
		const bool m_value;
	public:
		ezBool(bool val, const bool dynamic = true);
		const bool value(void);
};

class ezInteger : public ezValue {
	private:
		const int m_value;
	public:
		ezInteger(int val, const bool dynamic = true);
		const int value(void);
};

class ezString : public ezValue {
	private:
		const string m_value;
	public:
		ezString(const string val, const bool dynamic = true);
		const string value(void);
};

typedef	uint32_t ezInstruction;

class ezCarousel : public ezValue {
	public:
		uint8_t nargs;
		uint8_t nrets;
		ezCarousel(uint8_t args, uint8_t rets, const bool dynamic = true);
		~ezCarousel();
		vector<ezInstruction> instruction;
};

class ezNativeCarousel : public ezValue {
	private:
	public:
		ezNativeCarousel(const bool dynamic = true);
		virtual void run(vector<ezValue*>& args, vector<ezValue*>& rets) = 0;
		
};
