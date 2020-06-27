#pragma once

#include "appframework.h"
#include "net_api.h"

#define INETAPI_001 "IEngineNetwork001"
#define INETAPI_INTERFACE INETAPI_001

class IEngineNetAPI : public IAppInterface
{
public:
	virtual void		InitNetworking( void ) = 0;
	virtual void		Status ( struct net_status_s *status ) = 0;
	virtual void		SendRequest( int context, int request, int flags, double timeout, struct netadr_s *remote_address, net_api_response_func_t response ) = 0;
	virtual void		CancelRequest( int context ) = 0;
	virtual void		CancelAllRequests( void ) = 0;
	virtual char		*AdrToString( struct netadr_s *a ) = 0;
	virtual int		CompareAdr( struct netadr_s *a, struct netadr_s *b ) = 0;
	virtual int		StringToAdr( char *s, struct netadr_s *a ) = 0;
	virtual const char	*ValueForKey( const char *s, const char *key ) = 0;
	virtual void		RemoveKey( char *s, const char *key ) = 0;
	virtual void		SetValueForKey( char *s, const char *key, const char *value, int maxsize ) = 0;
};