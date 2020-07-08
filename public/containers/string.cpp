#include "string.h"

#include "port.h"
#include "xash3d_types.h"
#include "const.h"
#include <math.h>
#include <stdarg.h>
#include <ctype.h>
#include <time.h>
#include "stdio.h"
#include "crtlib.h"
#include <string.h>
#include <stdlib.h>

String::String()
{
	this->_length = 0;
	this->_string = nullptr;
}

String::String(const String &other) 
{
	this->_length = other._length;
	this->_string = strdup(other._string);
}

String::String(String &&other) 
{
	this->_length = other._length;
	this->_string = other._string;
	other._string = nullptr;
	other._length = 0;
}

String::String(const char *str) 
{
	this->_length = strlen(str);
	this->_string = strdup(str);
}

String::~String() 
{
	if(this->_string) free(_string);
}

const char *String::c_str() const 
{
	return this->_string;
}

size_t String::length() const 
{
	return this->_length;
}

bool String::equals(const String &other) const 
{
	if(!this->_string || !other._string) return false;
	return Q_strcmp(this->_string, other._string) == 0;
}

bool String::equals(const char *other) const 
{
	if(!this->_string || !other) return false;
	return Q_strcmp(other, this->_string) == 0;
}

bool String::iequals(const String &other) const 
{
	if(!this->_string || !other._string) return false;
	return Q_strcasecmp(other._string, this->_string);
}

bool String::iequals(const char *other) const 
{
	if(!this->_string || !other) return false;
	return Q_strcasecmp(other, this->_string);
}

void String::to_lower() 
{
	if(!this->_string) return;
	for(int i = 0; i < this->_length; i++)
		this->_string[i] = Q_tolower(this->_string[i]);
}

void String::to_upper() 
{
	if(!this->_string) return;
	for(int i = 0; i < this->_length; i++)
		this->_string[i] = Q_toupper(this->_string[i]);
}

size_t String::replace(char c, char n, size_t max = 0) 
{
	if(!this->_string) return 0;
	size_t num = 0;
	max = max > 0 ? max : SIZE_MAX;
	for(size_t i = 0; i < this->_length && max > 0; i++)
	{
		if(this->_string[i] == c) 
		{
			this->_string[i] = n;
			max--, num++;
		}
	}
	return num;
}

bool String::empty() const  
{
	return (!this->_string) && (this->_length == 0);
}

String String::substr(size_t start, size_t end) 
{
	return String();
}

String::operator const char *() const 
{
	return this->_string;
}

String::operator char *() 
{
	return this->_string;
}

char String::operator[](size_t i) const 
{
	if(i < this->_length) return this->_string[i];
	return 0;
}

String &String::operator=(const String &other) 
{
	if(this->_string) free(this->_string);
	this->_string = strdup(other._string);
	this->_length = other._length;
	return *this;
}

String &String::operator=(String &&other) 
{
	this->_string = other._string;
	this->_length = other._length;
	return *this;
}

bool String::operator==(const String &other) const 
{
	return this->equals(other);
}

bool String::operator!=(const String &other) const 
{
	return !this->equals(other);
}
