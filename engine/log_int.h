

#include "appframework.h"

#include <stdio.h>

class ILogSystem : public IAppInterface
{
public:

	/* Functions for changing of the IO streams */
	virtual void SetStdout(FILE* handle) = 0;
	virtual void SetStderr(FILE* handle) = 0;
	virtual void SetStdin(FILE* handle) = 0;

	virtual FILE* GetStdout() = 0;
	virtual FILE* GetStderr() = 0;
	virtual FILE* GetStdin() = 0;


	/* Remote logging functions */

	/**
	 * @brief Connects the specified IO stream to the specified port
	 * @param port
	 */
	virtual void ConnectStdout(int port) = 0;
	virtual void ConnectStderr(int port) = 0;
	virtual void ConnectStdin(int port) = 0;

	/**
	 * @brief Disconnects the specified stream from the port
	 * @param port
	 */
	virtual void DisconnectStdout(int port) = 0;
	virtual void DisconnectStderr(int port) = 0;
	virtual void DisconnectStdin(int port) = 0;

};

#define ILOGSYSTEM_INTERFACE "ILogSystem001"