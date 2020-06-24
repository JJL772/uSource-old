
#include "public/containers/list.h"
#include "engine/log_int.h"
#include "engine/common/common.h"

#define CLOGGINGSYSTEM_NAME "CLoggingSystem001"

struct log_channel_t
{
	char* name;
	int verbosity;
	char color[3];
	LoggingChannel_t id;
};

static List<log_channel_t> g_Channels;
static LoggingChannel_t g_CurrentIndex = 1;


class CLoggingSystem : public ILogSystem
{
private:
	FILE* _stdin;
	FILE* _stdout;
	List<FILE*> m_stdout;
	List<FILE*> m_stdin;
public:
	virtual const char* GetParentInterface();
	virtual const char* GetName();
	virtual bool PreInit();
	virtual bool Init();
	virtual void Shutdown();

	CLoggingSystem();

	/* Return true to support remote logging features */
	virtual bool SupportsRemoteLogging();

	/* If this supports remote logging, connect stdout/stdin with the port */
	virtual void ConnectPort(int port);

	/* Disconnect remote logging from the specified port */
	virtual void DisconnectPort(int port);

	virtual LoggingChannel_t CreateLoggingChannel(const char* name, int verbosity, char color[3]);
	virtual LoggingChannel_t CreateLoggingChannel(LoggingChannel_t id, const char* name, int verbosity, char color[3]);
	virtual LoggingChannel_t GetLoggingChannelForName(const char* name);


	virtual void Log(LoggingChannel_t channel, int v, const char* fmt);
	virtual void Warn(LoggingChannel_t channel, int v, const char* fmt);
	virtual void Error(LoggingChannel_t channel, int v, const char* fmt);

	/* Sets the standard file streams */
	virtual void SetStdin(FILE* stream);
	virtual void SetStdout(FILE* stream);

	virtual void AddStdin(FILE* stream);
	virtual void AddStdout(FILE* stream);
	virtual void RemoveStdin(FILE* stream);
	virtual void RemoveStdout(FILE* steam);
};

CLoggingSystem::CLoggingSystem()
{
	this->_stdout = nullptr;
	this->_stdin = nullptr;
}

/**
 * For Now we aren't supporting remote logging. This is a simple baseline logger
 */
bool CLoggingSystem::SupportsRemoteLogging()
{
	return false;
}

void CLoggingSystem::ConnectPort(int port)
{
	/* Stub for now */ 
}

void CLoggingSystem::DisconnectPort(int port)
{
	/* Stub for now */
}

LoggingChannel_t CLoggingSystem::CreateLoggingChannel(const char *name, int verbosity, char color[3])
{
	log_channel_t chan;
	chan.name = strdup(name);
	chan.verbosity = verbosity;
	chan.color[0] = color[0];
	chan.color[1] = color[1];
	chan.color[2] = color[2];
	chan.id = g_CurrentIndex;
	g_CurrentIndex++;

	/* First check if it's already registered */
	for(auto x : g_Channels)
	{
		if(Q_strcmp(x.name, name) == 0) return 0;
	}

	/* Cool, add it */
	g_Channels.push_back(chan);

	return chan.id;
}

LoggingChannel_t CLoggingSystem::GetLoggingChannelForName(const char *name)
{
	for(auto x : g_Channels)
	{
		if(Q_strcmp(name, x.name) == 0)
			return x.id;
	}
	return 0;
}

void SetColor(FILE* fs, char r, char g, char b)
{
#ifdef _POSIX
	fprintf(fs, "\e[38;5;%u;%u;%um", r, g, b);
#endif
}

log_channel_t GetChan(LoggingChannel_t chan)
{
	for(auto x : g_Channels)
	{
		if(x.id == chan) return x;
	}
	return {};
}

void CLoggingSystem::Log(LoggingChannel_t channel, int v, const char *fmt)
{
	static thread_local char buf[8192];

	/* Only for stdout */
	if(this->_stdout)
	{
		auto chan = GetChan(channel); 
		SetColor(this->_stdout, chan.color[0], chan.color[1], chan.color[2]);
		fprintf(this->_stdout, fmt);
	}

	for(auto stream : this->m_stdout)
		fprintf(stream, fmt);
	
}

void CLoggingSystem::Warn(LoggingChannel_t channel, int v, const char *fmt)
{
	static thread_local char buf[8192];

	/* Only for stdout */
	if(this->_stdout)
	{
		auto chan = GetChan(channel);
		SetColor(this->_stdout, chan.color[0], chan.color[1], chan.color[2]);
		fprintf(this->_stdout, fmt);
	}
	
	for(auto stream : this->m_stdout)
		fprintf(stream, fmt);

}

void CLoggingSystem::Error(LoggingChannel_t channel, int v, const char *fmt)
{
	static thread_local char buf[8192];
	/* Only for stdout */
	if(this->_stdout)
	{
		auto chan = GetChan(channel);
		SetColor(this->_stdout, chan.color[0], chan.color[1], chan.color[2]);
		fprintf(this->_stdout, fmt);
	}
	
	for(auto stream : this->m_stdout)
		fprintf(stream, fmt);
}


LoggingChannel_t CLoggingSystem::CreateLoggingChannel(LoggingChannel_t id, const char* name, int verbosity, char color[3])
{
	for(auto& chan : g_Channels)
	{
		/* Channel already created */
		if(chan.id == id) return id;
	}

	this->CreateLoggingChannel(name, verbosity, color);
	return id;
}

const char *CLoggingSystem::GetParentInterface()
{
	return ILOGSYSTEM_INTERFACE;
}

const char *CLoggingSystem::GetName()
{
	return CLOGGINGSYSTEM_NAME;
}

bool CLoggingSystem::PreInit()
{
	this->_stdin = stdin;
	this->_stdout = stdout;
	return true;
}

bool CLoggingSystem::Init()
{
	/* Create the default logging channel */
	char white[3] = {
		(char)255, (char)255, (char)255
	};
	this->CreateLoggingChannel("General", 0, white);
	return true;
}

void CLoggingSystem::Shutdown()
{

}

void CLoggingSystem::SetStdin(FILE *stream)
{
	this->_stdin = stream;
}

void CLoggingSystem::SetStdout(FILE *stream)
{
	this->_stdout = stream;
}

void CLoggingSystem::AddStdin(FILE* stream)
{
	this->m_stdin.push_back(stream);
}

void CLoggingSystem::AddStdout(FILE* stream)
{
	this->m_stdout.push_back(stream);
}

void CLoggingSystem::RemoveStdin(FILE* stream)
{
	this->m_stdin.remove(stream);
}

void CLoggingSystem::RemoveStdout(FILE* stream)
{
	this->m_stdout.remove(stream);
}


EXPOSE_INTERFACE(CLoggingSystem);