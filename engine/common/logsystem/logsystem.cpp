
#include "public/containers/list.h"
#include "engine/log_int.h"

#define CLOGGINGSYSTEM_NAME "CLoggingSystem001"

struct log_channel_t
{
	char* name;
	int verbosity;
	color24 color;
	LoggingChannel_t id;
};

static List<log_channel_t> g_Channels;
static LoggingChannel_t g_CurrentIndex = 1;


class CLoggingSystem : public ILogSystem
{
private:
	FILE* _stdin;
	FILE* _stdout;
public:
	virtual const char* GetParentInterface();
	virtual const char* GetName();
	virtual bool PreInit();
	virtual bool Init();
	virtual void Shutdown();



	/* Return true to support remote logging features */
	virtual bool SupportsRemoteLogging();

	/* If this supports remote logging, connect stdout/stdin with the port */
	virtual void ConnectPort(int port);

	/* Disconnect remote logging from the specified port */
	virtual void DisconnectPort(int port);

	virtual LoggingChannel_t CreateLoggingChannel(const char* name, int verbosity, color24 color);
	virtual LoggingChannel_t GetLoggingChannelForName(const char* name);


	virtual void Log(LoggingChannel_t channel, int v, const char* fmt, ...);
	virtual void Warn(LoggingChannel_t channel, int v, const char* fmt, ...);
	virtual void Error(LoggingChannel_t channel, int v, const char* fmt, ...);

	/* Sets the standard file streams */
	virtual void SetStdin(FILE* stream);
	virtual void SetStdout(FILE* stream);
};

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

LoggingChannel_t CLoggingSystem::CreateLoggingChannel(const char *name, int verbosity, color24 color)
{
	log_channel_t chan;
	chan.name = strdup(name);
	chan.verbosity = verbosity;
	chan.color = color;
	chan.id = g_CurrentIndex++;

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



void CLoggingSystem::Log(LoggingChannel_t channel, int v, const char *fmt, ...)
{

}

void CLoggingSystem::Warn(LoggingChannel_t channel, int v, const char *fmt, ...)
{

}

void CLoggingSystem::Error(LoggingChannel_t channel, int v, const char *fmt, ...)
{

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

EXPOSE_INTERFACE(CLoggingSystem);