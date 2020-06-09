/**
 * Appframework impl
 */
#include "appframework.h"

#include <list>

#include "common.h"
#include "crtlib.h"

struct appsys_t
{
	const char* parent;
	const char* name;
	bool loaded;
	bool init;
	struct mod_t* module;
	void* pinterface;
};

struct mod_t
{
	const char* filename;
	void* handle;
	pfnCreateInterface pCreateInterface;
	pfnGetInterfaces  pGetInterfaces;
};

#ifdef _POSIX
#include <dlfcn.h>

void* GetProcAddress(void* hmod, const char* sym)
{
	return dlsym(hmod, sym);
}

void* LoadLibraryA(const char* lib)
{
	return dlopen(lib, RTLD_LAZY);
}

void FreeLibrary(void* lib)
{
	dlclose(lib);
}

#else
#include <windows.h>
#endif

std::list<appsys_t> g_appsystems;
std::list<mod_t*> g_loadedmods;

bool AppFramework::AddInterface(const char *module, const char *iface)
{
	mod_t *_module;
	/* Load the lib */
	for(auto mod : g_loadedmods)
	{
		if(mod->filename && Q_strcmp(mod->filename, module) == 0)
		{
			_module = mod;
			break;
		}
	}

	/* Module not found :( */
	if(!_module)
	{
		_module = new mod_t();
		_module->handle = LoadLibraryA(module);

		if(!_module->handle)
		{
			delete _module;
			return false;
		}

		_module->filename = module;
		_module->pCreateInterface = (pfnCreateInterface)GetProcAddress(_module->handle, "CreateInterface");
		_module->pGetInterfaces = (pfnGetInterfaces)GetProcAddress(_module->handle, "GetInterfaces");

		/* Functions not exported?? */
		if(!_module->pGetInterfaces || !_module->pCreateInterface)
		{
			FreeLibrary(_module->handle);
			delete _module;
			return false;
		}

		/* Add it to the list */
		g_loadedmods.push_back(_module);
	}

	/* Get the module info ptr */
	int outnum = 0;
	iface_t* iface_list = _module->pGetInterfaces(&outnum);

	/* No interfaces? */
	if(outnum <= 0 || iface_list == nullptr)
	{
		return false;
	}

	/* Loop through and check if the name matches with the passed iface */
	for(int i = 0; i < outnum; i++)
	{
		if(Q_strcmp(iface_list[i].name, iface) == 0)
		{
			appsys_t appsys;
			appsys.module = _module;
			appsys.name = iface;
			appsys.loaded = false;
			appsys.init = false;
			appsys.pinterface = 0;
			appsys.parent = iface_list[i].parent;
			g_appsystems.push_back(appsys);
			return true;
		}
	}

	/* Nein? That sucks */
	return false;
}

bool AppFramework::LoadInterfaces()
{
	/* Loop through each interface and check which hasn't been loaded */
	for(auto& appsys : g_appsystems)
	{
		/* Not loaded yet!! */
		if(!appsys.loaded)
		{
			/* Call CreateInterface for it */
			int status = 0;
			appsys.pinterface = appsys.module->pCreateInterface(appsys.name, &status);

			/* Failure! */
			if(status == EIfaceStatus::FAILED)
				return false;

			/* Cast to the base class and call preinit, and fail if it breaks */
			if(!((IAppInterface*)appsys.pinterface)->PreInit())
			{
				return false;
			}

			appsys.loaded = true;
		}
	}

	/* Pass #2. Call init on each interface */
	for(auto& appsys : g_appsystems)
	{
		/* Not initialized yet! */
		if(!appsys.init)
		{
			/* pinterface should be valid here */
			if(!((IAppInterface*)appsys.pinterface)->Init())
			{
				return false;
			}

			appsys.init = true;
		}
	}

	return true;
}

void *AppFramework::FindInterface(const char *iface)
{
	for(auto x : g_appsystems)
	{
		if(Q_strcmp(x.parent, iface) == 0)
		{
			return x.pinterface;
		}
	}
	return nullptr;
}

void AppFramework::UnloadInterfaces()
{
	for(auto& appsys : g_appsystems)
	{
		if(appsys.loaded)
		{
			((IAppInterface*)appsys.pinterface)->Shutdown();
			appsys.loaded = appsys.init = false;
		}
	}
}
