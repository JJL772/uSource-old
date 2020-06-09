
#pragma once

/**
 * AppFramework and how it works
 *
 * This is a system that's heavily modeled off of Source 1's AppFramework system.
 * There are some pretty major differences, however.
 *
 * The base class IModuleInterface should be the parent of any exported interface.
 * A module can export an infinite number of interfaces.
 *
 * Each interface has a couple of properties:
 * 	- Version number
 * 	- Parent interface
 *
 * The parent interface is the interface in which it implements. E.g. IFilesystem001
 * Version numbers are rather abstract, and they're a part of the interface parent name itself (e.g. 001 in the above example)
 *
 * Interfaces are loaded into interface "slots". Basically you have an interface implementation, such as IEngineFilesystem001, which
 * is an implementation of IFilesystem001. So, when you load IEngineFilesystem001, it's loaded into the IFilesystem001 slot. When other modules lookup interfaces,
 * they refer to the "slot" name, in this case IFilesystem001, and the IEngineFilesystem001 implementation is returned.
 */

class IAppInterface
{
public:
	virtual const char* GetParentInterface() = 0;

	virtual const char* GetName() = 0;

	virtual bool PreInit() = 0;

	virtual bool Init() = 0;

	virtual void Shutdown() = 0;

};

typedef struct
{
	const char* name;
	const char* parent;
} iface_t;

/* Returns a list of interfaces exported by this module */
/* The single int parameter should be set to the number of interfaces in the returned array */
typedef iface_t*(*pfnGetInterfaces)(int*);

/* Returns a new pointer to the specified interface */
/* First parameter is the interface name (not the parent!) and the second is an out parameter for the status code */
typedef void*(*pfnCreateInterface)(const char*, int*);

/* Used for the iface status parameter in pfnCreateInterface */
enum EIfaceStatus
{
	OK = 0,
	FAILED = 1,
};

/*

 API usage:

 AppFramework_AddInterface("engine.so", "IFilesystem001");
 AppFramework_AddInterface("engine.so", "IEngineTrace001");

 AppFramework_LoadInterfaces();

 void* ptr = AppFramework_FindInterface("IEngineTrace001");

 ...

 */

namespace AppFramework
{
	/**
	 * @brief Adds an interface to the load list
	 * @param module shared object or DLL this comes from
	 * @param iface Interface name
	 * @return True if added, false otherwise
	 */
	bool AddInterface(const char *module, const char *iface);

	/**
	 * @brief Called to load all interfaces queued for load
	 * @return True if OK, false if something failed
	 */
	bool LoadInterfaces();

	/**
	 * @brief Finds a pointer to a loaded interface
	 * @param iface Name of the interface
	 * @return Pointer to the iface or NULL if not found
	 */
	void * FindInterface(const char *iface);

	/**
	 * @brief Unloads all added interfaces
	 */
	void UnloadInterfaces();
}