#include "eaval.h"
#include <stdexcept>

using std::runtime_error;

eaValue::eaValue(const unsigned int id) : ezValue(id) {}

bool eaValue::to_bool(void) {
  throw runtime_error("unable to cast to bool");
}

int eaValue::to_int(void) {
  throw runtime_error("unable to cast to int");
}

double eaValue::to_float(void) {
  throw runtime_error("unable to cast to float");
}

complex<double> eaValue::to_complex(void) {
  throw runtime_error("unable to cast to complex");
}

string eaValue::to_string(void) {
  throw runtime_error("unable to cast to string");
}

