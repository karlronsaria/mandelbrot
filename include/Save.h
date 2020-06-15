#pragma once
#include "types.h"
#include <iomanip>
#include <sstream>
#include <string>
#include <time.h>

constexpr int FLT_T_SIZE = sizeof(flt_t);
const int COUPLET_SIZE = 2;
// const int J_COORD_PRECISION = 4;

std::string to_hex_str(flt_t value, int_t size = FLT_T_SIZE);
flt_t to_float(const std::string& word);
void put_int(std::ostream& buf, int_t n);
void put_flt(std::ostream& buf, flt_t n, int_t precision = FLT_T_SIZE);
int_t get_int(std::string str);
flt_t get_flt(std::string str);
std::string GetDateTimeString();
