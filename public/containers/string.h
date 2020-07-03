/**
 * 
 * string.h
 * 
 * Custom string class
 * 
 */ 
#pragma once

#include <stddef.h>
#include <stdint.h>

class String 
{
private:
	char* _string;
	unsigned long long _length;

public:
	String();
	String(const String& other);
	String(String&& other);
	String(const char* str);

	~String();

	const char* c_str() const;

	size_t length() const;

	bool empty() const;

	bool equals(const String& other) const;
	bool equals(const char* other) const;

	bool iequals(const String& other) const;
	bool iequals(const char* other) const;

	void to_lower();
	void to_upper();
	size_t replace(char c, char n, size_t max = 0);

	String substr(size_t start, size_t end); 

	operator const char*() const;
	explicit operator char*();
	char operator[](size_t i) const;
	String& operator=(const String& other);
	String& operator=(String&& other);

	bool operator==(const String& other) const;
	bool operator!=(const String& other) const;
};