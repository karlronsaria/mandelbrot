#include "Save.h"

union convert_t {
	flt_t flt;
	unsigned char c_str[FLT_T_SIZE];
};

std::string to_hex_str(flt_t value, int_t size) {
	convert_t x;
	x.flt = value;
	std::stringstream buf;
	buf.fill('0');

	for (int i = 0; i < size; ++i)
		buf << std::setw(COUPLET_SIZE) << std::hex << (int)x.c_str[i];

	return buf.str();
}

flt_t to_float(const std::string& word) {
	convert_t u = { 0.F };
	int i = 0, j, k = 0;
	std::string couplet;

	while (i < word.length()) {
		couplet = "";
		j = 0;

		while (j < COUPLET_SIZE) {
			couplet += word[i];
			j = j + 1;
			i = i + 1;
		}

		u.c_str[k] = std::stoi(couplet, nullptr, 16);
		k = k + 1;
	}

	return u.flt;
}

void put_int(std::ostream& buf, int_t n) {
	if (n < 0)
		buf << 'n' << -n;
	else
		buf << n;
}

void put_flt(std::ostream& buf, flt_t n, int_t precision) {
	if (n < 0)
		buf << 'n' << to_hex_str(-n, precision);
	else
		buf << to_hex_str(n, precision);
}

int_t get_int(std::string str) {
	int_t n;

	if (str[0] == 'n') {
		n = -1;
		str = str.substr(1);
	}
	else {
		n = 1;
	}

	return n * std::stoll(str);
}

flt_t get_flt(std::string str) {
	flt_t n;

	if (str[0] == 'n') {
		n = -1.L;
		str = str.substr(1);
	}
	else {
		n = 1.L;
	}

	return n * to_float(str);
}

std::string GetDateTimeString() {
	std::time_t temp = std::time(nullptr);
	struct tm time;
	localtime_s(&time, &temp);

	std::string mon = std::to_string(time.tm_mon + 1);
	std::string day = std::to_string(time.tm_mday);
	std::string hour = std::to_string(time.tm_hour);
	std::string min = std::to_string(time.tm_min);
	std::string sec = std::to_string(time.tm_sec);

	mon = mon.length() < 2 ? "0" + mon : mon;
	day = day.length() < 2 ? "0" + day : day;
	hour = hour.length() < 2 ? "0" + hour : hour;
	min = min.length() < 2 ? "0" + min : min;
	sec = sec.length() < 2 ? "0" + sec : sec;

	std::string str =
		std::to_string(time.tm_year + 1900)
		+ "_"
		+ mon
		+ "_"
		+ day
		+ "_"
		+ hour + min + sec;

	return str;
}
