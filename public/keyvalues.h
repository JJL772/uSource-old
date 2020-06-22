/**
 *
 * keyvalues.h
 *
 * Parser for KeyValues files
 *
 * TODO: Add better memory management. Right now keys are all strduped and freed when the key is removed.
 * 		 this will probably lead to memory fragmentation.
 * TODO: Improve the error handling
 * TODO: Add support for conditionals
 */ 
#pragma once

#undef min
#undef max
#include <vector>
#include <stdio.h>

class KeyValues
{
public:
	struct key_t
	{
		char* key;
		char* value;
		enum class ELastCached
		{
			NONE = 0,
			INT,
			FLOAT,
			BOOL,
		} cached;

		union {
			long int ival;
			double fval;
			bool bval;
		} cachedv;

                bool quoted;

		key_t() { cached = ELastCached::NONE; };

		inline bool ReadBool(bool &ok);
		inline long int ReadInt(bool &ok);
		inline double ReadFloat(bool &ok);

	};
	char* name;
	bool good;
        bool quoted;
public:

	explicit KeyValues(const char* name);
	KeyValues();

	~KeyValues();

	/* Getters */
	bool 		GetBool(const char* key, bool _default = false);
	int 		GetInt(const char* key, int _default = -1);
	float 		GetFloat(const char* key, float _default = 0.0f);
	const char*     GetString(const char* key, const char* _default = nullptr);
	double 		GetDouble(const char* key, double _default = 0.0);
	KeyValues*      GetChild(const char* name);

	bool 		HasKey(const char* key);

	/* Setters */
	void 		SetBool(const char* key, bool v);
	void 		SetInt(const char* key, int v);
	void 		SetFloat(const char* key, float v);
	void 		SetString(const char* key, const char* v);

	/* Parse from a file */
	void 		ParseFile(const char* file, bool use_escape_codes = false);
	void 		ParseString(const char* string, bool use_escape_codes = false, long long len = -1);

	/* Clears a key's value setting it to "" */
	void 		ClearKey(const char* key);

	/* Completely removes a key */
	void 		RemoveKey(const char* key);

	/* Dumps this to stdout */
	void 		DumpToStream(FILE* fs);
	void 		DumpToStreamInternal(FILE* fs, int indent);

	/* Set Good bit is set if parsing went OK */
	bool        IsGood() const { return this->good; };

	enum class EError
	{
		NONE = 0,
		UNEXPECTED_EOF,
		MISSING_BRACKET,
		MISSING_QUOTE,
		UNNAMED_SECTION,
		UNTERMINATED_SECTION,
	};

	typedef void(*pfnErrorCallback_t)(int,int,EError);

	/* Sets the error reporting callback */
	void SetErrorCallback(pfnErrorCallback_t callback);

	/* Array of child sections */
	std::vector<KeyValues*> child_sections;

	/* Array of keys */
	std::vector<key_t> keys;
private:
	void ReportError(int line, int _char, EError err);
	pfnErrorCallback_t pCallback;
};
