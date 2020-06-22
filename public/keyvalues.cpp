/**
 *
 * Key Values parser
 *
 */ 
#include "keyvalues.h"

#include <memory.h>
#include <stdlib.h>
#include <ctype.h>
#include <stack>
#include <stdio.h>
#include <errno.h>

inline bool _internal_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

KeyValues::KeyValues(const char* name) :
	pCallback(NULL)
{
	this->name = strdup(name);
	this->keys.reserve(10);
        this->quoted = false;
}

KeyValues::KeyValues() :
	pCallback(NULL)
{

}

KeyValues::~KeyValues()
{
	if(this->name) free(name);

	/* Free the keys */
	for(auto key : this->keys)
	{
		if(key.key) free(key.key);
		if(key.value) free(key.value);
	}

	/* Free child sections */
	for(auto section : this->child_sections)
		delete section;
}

void KeyValues::ParseFile(const char *file, bool use_escape_codes)
{
	FILE* fs = fopen(file, "r");
	if(!fs)
	{
		this->good = false;
		return;
	}

	/* Get file size */
	fseek(fs, 0, SEEK_END);
	long int size = ftell(fs);

	/* Read the entire file */
	char* buffer = static_cast<char*>(malloc(size + 1));
	fseek(fs, 0, SEEK_SET);
	fread(buffer, size, 1, fs);
	fclose(fs);
	buffer[size] = 0;
	
	this->ParseString(buffer, use_escape_codes, size);

	/* Free the allocated buffer */
	free(buffer);
}


void KeyValues::ParseString(const char* string, bool escape, long long len)
{
	int nline = 0, nchar = 0, bracket_level = 0;
	bool inquote = false, incomment = false, parsed_key = false;
	char buf[512];
	int bufpos = 0;

	size_t nlen = 0;
	if(len < 0) nlen = strlen(string);
	else nlen = len;


	KeyValues* RootKV = this;
	KeyValues* CurrentKV = this;
	key_t CurrentKey;

	/* Replaces the std::stack calls */
	int stackpos = 0;
	KeyValues* stack[512];
	stack[stackpos] = this;
	//stackpos++;

	std::stack<KeyValues*> SectionStack;
	SectionStack.push(this);

	this->good = true;

	char c, nc, pc;
	for(int i = 0; i < nlen; i++, nchar++)
	{
		c = string[i];
		if(i > 0) pc = string[i-1];
		if(i < nlen-1) nc = string[i+1];

		if(c == '\n')
		{
			/* Check for errors */
			if(inquote) this->ReportError(nline, nchar, EError::MISSING_QUOTE);

			/* Save any tokens we might have at the end of the line */
			if(bufpos > 0)
			{
				buf[bufpos] = 0;
				if(parsed_key)
				{
					parsed_key = false;
					CurrentKey.value = strdup(buf);
					CurrentKV->keys.push_back(CurrentKey);
					CurrentKey.key = CurrentKey.value = NULL;
				}
				else
				{
					CurrentKey.key = strdup(buf);
					parsed_key = true;
				}
				bufpos = 0;
			}
			incomment = false;
			inquote = false;

			nline++;
			nchar = 0;

			continue;
		}

		if(c == '/' && (pc == '/' || nc == '/') && !inquote)
		{
			incomment = true;
			continue;
		}

		if(incomment) continue;

		/* Handle exit/enter quote */
		if(c == '"')
		{
			if(inquote)
			{
				inquote = false;
				buf[bufpos] = 0;
				if(parsed_key)
				{
					parsed_key = false;
					CurrentKey.value = strdup(buf);
                                        CurrentKey.quoted = true;
					CurrentKV->keys.push_back(CurrentKey);
					CurrentKey.key = CurrentKey.value = NULL;
				}
				else
				{
                                        CurrentKey.quoted = true;
					CurrentKey.key = strdup(buf);
					parsed_key = true;
				}
				bufpos = 0;
			}
			else
			{
				inquote = true;
			}
			continue;
		}

		/* Enter scope */
		if(!inquote && c == '{')
		{
			KeyValues* pKV;
			if(parsed_key)
			{
				pKV = new KeyValues(CurrentKey.key);
				free(CurrentKey.key);
				CurrentKey.key = 0;
			}
			else if(bufpos > 0)
			{
				buf[bufpos] = 0;
				bufpos = 0;
				pKV = new KeyValues(buf);
			}
			/* In the event that buf is empty and we've not yet parsed a key, issue an error about an unnamed section */
			else
			{
				pKV = new KeyValues();
				this->ReportError(nline, nchar, EError::UNNAMED_SECTION);
			}
			if(CurrentKey.quoted) pKV->quoted = true;
                        parsed_key = false;
			CurrentKV->child_sections.push_back(pKV);
			CurrentKV = pKV;
			stack[++stackpos] = pKV;
			bracket_level++;
			continue;
		}
		/* Exit scope */
		else if(!inquote && c == '}')
		{
			CurrentKV = stack[--stackpos];
			bracket_level--;
			continue;
		}

		/* Eat anything that isn't space into the buffer */
		if(!_internal_isspace(c) || (inquote))
		{
			buf[bufpos++] = c;
			continue;
		}

		/* Finally, handle cases where we've encountered a space and we are not in a quote */
		if(_internal_isspace(c) && !inquote && bufpos > 0)
		{
			inquote = false;
			buf[bufpos] = 0;
			if(parsed_key)
			{
				parsed_key = false;
				CurrentKey.value = strdup(buf);
				CurrentKey.quoted = false;
                                CurrentKV->keys.push_back(CurrentKey);
				CurrentKey.key = CurrentKey.value = NULL;
			}
			else
			{
                                CurrentKey.quoted = false;
				CurrentKey.key = strdup(buf);
				parsed_key = true;
			}
			bufpos = 0;
		}
                for(; i < nlen-1 && _internal_isspace(string[i + 1]); i++);
	}

	/* Verify that the parsing completed fine */
	if(inquote) this->ReportError(-1, 0, EError::MISSING_QUOTE);
	if(bracket_level > 0) this->ReportError(-1, 0, EError::UNTERMINATED_SECTION);
}
void KeyValues::ReportError(int line, int _char, EError err)
{
	if(pCallback) pCallback(line, _char, err);
	this->good = false;
}


void KeyValues::DumpToStream(FILE* fs)
{
	this->DumpToStreamInternal(fs, 0);
}

void KeyValues::DumpToStreamInternal(FILE* fs, int indent)
{
	/* Ensure indent < 128 or fail */
	if(!fs || indent >= 127) return;

	char buf[128];
	for(int i = 0; i < indent; i++) buf[i] = '\t';
	buf[indent] = 0;
	if(this->name)
	{
		if(this->quoted)
                        fprintf(fs, "%s\"%s\"\n%s{\n", buf, this->name, buf);
                else
                        fprintf(fs, "%s%s\n%s{\n", buf, this->name, buf);
                buf[indent] = '\t';
		buf[indent+1] = 0;
	}

	for(auto key : this->keys)
        {
	        if(key.quoted)
                        fprintf(fs, "%s\"%s\" \"%s\"\n", buf, key.key, key.value); 
                else
                        fprintf(fs, "%s%s \"%s\"\n", buf, key.key, key.value);
        }
	for(auto section : this->child_sections)
	{
		section->DumpToStreamInternal(fs, indent+1);
	}

	buf[indent] = 0;
	if(this->name) fprintf(fs, "%s}\n", buf);


}

bool KeyValues::GetBool(const char* key, bool _default)
{
	for(auto _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			bool ok = false;
			bool b = _key.ReadBool(ok);
			if(ok) return b;
			return _default;
		}
	}
	return _default;
}

int KeyValues::GetInt(const char* key, int _default)
{
	for(auto _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			bool ok = false;
			int i = (int)_key.ReadInt(ok);
			if(ok) return i;
			return _default;
		}
	}
	return _default;
}

float KeyValues::GetFloat(const char* key, float _default)
{
	for(auto _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			bool ok = false;
			float f = (float)_key.ReadFloat(ok);
			if(ok) return f;
			return _default;
		}
	}
	return _default;
}

double KeyValues::GetDouble(const char* key, double _default)
{
	for(auto _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			bool ok = false;
			double f = _key.ReadFloat(ok);
			if(ok) return f;
			return _default;
		}
	}
	return _default;
}

const char* KeyValues::GetString(const char* key, const char* _default)
{
	for(auto _key : this->keys)
	{
		if(_key.key && strcmp(key, _key.key) == 0)
			return _key.value;
	}
	return _default;
}

bool KeyValues::HasKey(const char* key)
{
	for(auto _key : this->keys)
	{
		if(_key.key && strcmp(key, _key.key) == 0) return true;
	}
	return false;
}

bool KeyValues::key_t::ReadBool(bool& ok)
{
	ok = true;
	if(this->cached == ELastCached::BOOL)
	{
		return this->cachedv.bval;
	}
	/* If the value is not cached, parse it from a string */
	if(!this->value)
	{
		ok = false;
		return false;
	}

	/* For bool, check if we've got TRUE or FALSE */
	if(strcasecmp(this->value, "true") == 0 || strcmp(this->value, "1") == 0)
	{
		this->cachedv.bval = true;
		this->cached = ELastCached::BOOL;
		return true;
	}
	else if(strcasecmp(this->value, "false") == 0 || strcmp(this->value, "0") == 0)
	{
		this->cachedv.bval = false;
		this->cached =  ELastCached::BOOL;
		return false;
	}
	ok = false;
	return false;
}

long int KeyValues::key_t::ReadInt(bool& ok)
{
	ok = true;
	if(this->cached == ELastCached::INT)
	{
		return this->cachedv.ival;
	}

	/* Check that value is not null */
	if(!this->value)
	{
		ok = false;
		return 0;
	}

	long int v = strtol(this->value, NULL, 10);
	if(errno == 0)
	{
		this->cached = ELastCached ::INT;
		this->cachedv.ival = v;
		return v;
	}
	ok = false;
	return 0;
}

double KeyValues::key_t::ReadFloat(bool& ok)
{
	ok = true;
	if(this->cached == ELastCached::FLOAT)
	{
		return this->cachedv.fval;
	}

	/* Check if value is null */
	if(!this->value)
	{
		ok = false;
		return 0.0f;
	}

	double f = strtod(this->value, NULL);
	if(errno == 0)
	{
		this->cached = ELastCached::FLOAT;
		this->cachedv.fval = f;
		return f;
	}
	ok = false;
	return 0.0f;
}

KeyValues* KeyValues::GetChild(const char *name)
{
	for(auto _child : this->child_sections)
	{
		if(_child->name && strcmp(name, _child->name) == 0)
		{
			return _child;
		}
	}
	return nullptr;
}

void KeyValues::SetBool(const char *key, bool v)
{
	for(auto& _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			_key.cached = KeyValues::key_t::ELastCached::BOOL;
			_key.cachedv.bval = v;
			return;
		}
	}
}

void KeyValues::SetInt(const char *key, int v)
{
	for(auto& _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			_key.cached = KeyValues::key_t::ELastCached::INT;
			_key.cachedv.ival = v;
			return;
		}
	}
}

void KeyValues::SetFloat(const char *key, float v)
{
	for(auto& _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			_key.cached = KeyValues::key_t::ELastCached::FLOAT;
			_key.cachedv.fval = v;
			return;
		}
	}
}

void KeyValues::SetString(const char *key, const char *v)
{
	for(auto& _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			_key.cached = KeyValues::key_t::ELastCached::NONE;
			if(_key.value) free(_key.value);
			_key.value = strdup(v);
			return;
		}
	}
}

void KeyValues::ClearKey(const char *key)
{
	for(auto& _key : this->keys)
	{
		if(_key.key && strcmp(_key.key, key) == 0)
		{
			free(_key.value);
			_key.value = strdup("");
			_key.cached = KeyValues::key_t::ELastCached::NONE;
			return;
		}
	}
}

void KeyValues::RemoveKey(const char *key)
{
	for(auto it = this->keys.begin(); it != this->keys.end(); it++)
	{
		if(it->value && strcmp(key, it->value) == 0)
		{
			this->keys.erase(it);
			return;
		}
	}
}

void KeyValues::SetErrorCallback(KeyValues::pfnErrorCallback_t callback)
{
	this->pCallback = callback;
}
