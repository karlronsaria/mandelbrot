#pragma once
#include <list>
#include <vcruntime.h>

template <typename T>
class History {
public:
	typedef void (*const_binary_void)(const History&, const T&);
private:
	std::list<T> _list;
	typename std::list<T>::iterator _current;
	void drop_tail();
public:
	History();
	History(const T&);
	History(const History&) = default;
	virtual ~History() = default;

	const T& current() const;
	T& current();

	bool next();
	bool prev();
	void add();
	void add(const T&);

	size_t size() const;
	void foreach(const_binary_void function) const;

#ifdef DEBUG
	std::string to_string(const char*) const;
#endif
};

template <typename T>
History<T>::History() {
	_list.push_back(std::move(T()));
	_current = _list.begin();
}

template <typename T>
History<T>::History(const T& payload) {
	_list.push_back(payload);
	_current = _list.begin();
}

template <typename T>
void History<T>::drop_tail() {
	auto temp = _current;
	temp++;
	_list.erase(temp, _list.end());
}

template <typename T>
const T& History<T>::current() const {
	return *_current;
}

template <typename T>
T& History<T>::current() {
	return *_current;
}

template <typename T>
size_t History<T>::size() const {
	return _list.size();
}

template <typename T>
bool History<T>::next() {
	auto temp = _current;

	if (++temp == _list.end())
		return false;

	++_current;
	return true;
}

template <typename T>
bool History<T>::prev() {
	if (_current != _list.begin()) {
		--_current;
		return true;
	}

	return false;
}

template <typename T>
void History<T>::add() {
	add(std::move(T()));
}

template <typename T>
void History<T>::add(const T& payload) {
	drop_tail();
	_list.push_back(payload);
	next();
}

template <typename T>
void History<T>::foreach(const_binary_void function) const {
	for (const auto& temp : _list)
		function(*this, temp);
}

#ifdef DEBUG
template <typename T>
std::string History<T>::to_string(const char* delim) const {
	std::ostringstream buf;

	for (const auto& temp : _list)
		buf << temp << delim;

	return buf.str();
}
#endif
