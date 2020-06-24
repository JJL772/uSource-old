
#pragma once

#include "appframework.h"

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef uint32_t LoggingChannel_t;

#define ILOGSYSTEM_INTERFACE "ILogSystem001"

class ILogSystem : public IAppInterface
{
public:
	/* Return true to support remote logging features */
	virtual bool SupportsRemoteLogging() = 0;

	/* If this supports remote logging, connect stdout/stdin with the port */
	virtual void ConnectPort(int port) = 0;

	/* Disconnect remote logging from the specified port */
	virtual void DisconnectPort(int port) = 0;

	/* verbosity is the minimum verbosity a message must have in order to be shown */
	virtual LoggingChannel_t CreateLoggingChannel(const char* name, int verbosity, char color[3]) = 0;
	virtual LoggingChannel_t CreateLoggingChannel(LoggingChannel_t id, const char* name, int verbo, char color[3]) = 0;
	virtual LoggingChannel_t GetLoggingChannelForName(const char* name) = 0;


	virtual void Log(LoggingChannel_t channel, int verbosity, const char* fmt) = 0;
	virtual void Warn(LoggingChannel_t channel, int verbosity, const char* fmt) = 0;
	virtual void Error(LoggingChannel_t channel, int verbosity, const char* fmt) = 0;

	/* Sets the standard file streams */
	virtual void SetStdin(FILE* stream) = 0;
	virtual void SetStdout(FILE* stream) = 0;

	/* Adds or removes streams */
	virtual void AddStdin(FILE* stream) = 0;
	virtual void AddStdout(FILE* stream) = 0;
	virtual void RemoveStdin(FILE* stream) = 0;
	virtual void RemoveStdout(FILE* steam) = 0;
};

/*
=======================

 Default logging channels 

=======================
*/
#define LOGCHAN_GENERAL 0 /* General info */
#define LOGCHAN_WARN	1 /* General warns */
#define LOGCHAN_ERROR	2 /* General errors */
#define LOGCHAN_FATAL	3 /* General fatal errors */

#define LOGVERBO_LOWEST 0

/*
=======================

 Common structures and other definitions

=======================
*/
enum class ELogLevel : int
{
	INVALID = -1,
	INFO,
	WARNING,
	ERROR,
	FATAL,
};

/*
=======================

 LOGGING PROTOCOL DEFINITIONS

 Some notes:
  - This isn't intended to be a secure protocol by any means.
	the most security we take here is literally just "dont send the password in plaintext"
        you should never use remote logging on public networks. Keep it confined to your
        LAN, or use ssh tunneling.

=======================
*/

/* Log message types */
#define LOG_MSG_REGISTER_CHANNEL (1<<0)
#define LOG_MSG_LOG (1<<1)
#define LOG_MSG_STDIN (1<<2)

/* Everything here is tightly packed */
#pragma pack(1)

struct log_hdr_t
{
	uint32_t type;

	/* md5 hash of the password */
	/* This is unset for logging to STDOUT */
	/* Any commands coming in over STDIN will need the password set IF */
	/*  secure mode is enabled on the server */
	uint64_t passhash[2];

};

/* LOG_MSG_REGISTER_CHANNEL */
struct log_msg_register_channel_t
{
	log_hdr_t hdr;
	LoggingChannel_t channel; /* Reference ID for the channel */
	unsigned char color[3]; /* color[0] = r, color[1] = g, color[2] = b */
	char name[64]; /* NULL terminated string representing the name. */
	int verbosity; /* Verbosity level */
};

/* LOG_MSG_LOG */
/* The actual message logging message */
struct log_msg_log_t
{
	log_hdr_t hdr;
	LoggingChannel_t channel; /* The logging channel for this message */
	uint32_t length; /* Length of the message */
	/* Following this message, there will be length bytes of UTF8 encoded text */
	/* The string is not NULL terminated! */
};

/* LOG_MSG_STDIN */
/* This is the standard input message */
/* Password is usually required for this */
/* This message will also be received by the server only */
struct log_msg_stdin_t
{
	log_hdr_t hdr;
	uint32_t length;
	/* Same as the normal message type. length bytes of UTF8 encoded string data will follow */
	/* It is not null terminated */
};




#pragma pack()
