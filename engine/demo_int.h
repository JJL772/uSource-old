#pragma once

#include "appframework.h"
#include "demo_api.h"

#define IDEMOAPI_001 "IEngineDemo001"
#define IDEMOAPI_INTERFACE IDEMOAPI_001

class IEngineDemo : public IAppInterface
{
public:
	virtual int	IsRecording( void ) = 0;
	virtual int	IsPlayingback( void ) = 0;
	virtual int	IsTimeDemo( void ) = 0;
	virtual void	WriteBuffer( int size, unsigned char *buffer ) = 0;
};