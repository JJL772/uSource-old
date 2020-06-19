/**
 *
 * mod_gltf.cpp
 *
 * Model loader for glTf models
 *
 */
#include "common.h"
#include "mod_local.h"
#include "imagelib/imagelib.h"

/* cgltf includes */
#define CGLTF_IMPLEMENTATION
#include "cgltf/cgltf.h"

/* glTf extensions */
const char* g_gltf_extensions[] = {
	"glb",
	"gltf",
};

class CGlTfLoader : public IModelLoader
{
	CGlTfLoader();
	virtual ~CGlTfLoader() {};

	virtual bool LoadModel(model_t* mod, const void* buffer, size_t length);

	virtual bool CheckBuffer(const void* buf, size_t len) { return true; }

	/* Singleton */
	static CGlTfLoader g_glTfLoader;
};

CGlTfLoader::CGlTfLoader() :
	IModelLoader(g_gltf_extensions, ARRAYSIZE(g_gltf_extensions))
{

}

bool CGlTfLoader::LoadModel(model_t *mod, const void *buffer, size_t length)
{
	cgltf_options options = {};
	cgltf_data* data = NULL;
	cgltf_result result = cgltf_parse(&options, buffer, 0, &data);

	if(result != cgltf_result_success)
		return false;

	/* Just loads the internal buffers */
	cgltf_load_buffers(&options, data, mod->name);




	cgltf_free(data);

	return false;
}
