/*

cli_utl.h

Utils for command line interfaces

*/
#pragma once

#include <initializer_list>
#include <vector>
#include <type_traits>
#include <string>

class CmdParser
{
private:

public:
	CmdParser(std::initializer_list<class CmdOptionBase> list);
};

enum class ECmdOptionFlags
{
	NONE = 0,
	REQUIRED = 1,
	OPTIONAL = 2,
};

enum class ECmdOptionType
{
	STRING = 0,
	INT = 1,
	FLOAT = 2,
	BOOL = 3,
};

class CmdOptionBase
{
protected:
	ECmdOptionFlags flags;
	const char* help;
	std::vector<const char*> aliases;
	ECmdOptionType type;
public:
	CmdOptionBase(const char* name, const char* help, ECmdOptionFlags flags);
};

template<class T>
class CmdOption : public CmdOptionBase
{
private:
	T val;
public:
	CmdOption(const char* name, T initial_val, const char* help, ECmdOptionFlags flags)
	: CmdOptionBase(name, help, flags)
	{
		initial_val = val;
		if(std::is_integral<T>::value)
			this->type = ECmdOptionType::INT;
		else if(std::is_floating_point<T>::value)
			this->type = ECmdOptionType::FLOAT;
		else if(std::is_same<T, bool>::value)
			this->type = ECmdOptionType::BOOL;
		else if(std::is_same<T, const char*>::value)
			this->type = ECmdOptionType::STRING;
		else if(std::is_same<T, char*>::value)
			this->type = ECmdOptionType::STRING;
		else if(std::is_same<T, std::string>::value)
			this->type = ECmdOptionType::STRING;
	}

	CmdOption(std::initializer_list<const char*> aliases, T initial_val, const char* help, ECmdOptionFlags flags)
	: CmdOptionBase(name, help, flags)
	{
		
	}
};