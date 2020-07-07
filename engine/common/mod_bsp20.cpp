/**
 * 
 * mod_bsp20.cpp
 * 	Support for BSP version 20 files (commonly used in source games)
 * 
 * Notes:
 * 	- The loader manages a global state, so this is not thread-safe and should only be called from the main thread 
 */
#include "common.h"
#include "mod_local.h"
#include "imagelib/imagelib.h"
#include "bsp20file.h"
#include "public/keyvalues.h"

const char* g_bsp20_extensions[] = {
	"bsp",
	"bsp20", /* we can explicitly specify the map version using this */
};;

static model_t* g_bsp20_model = nullptr;

class CBsp20Loader : public IMapLoader
{
public:
	CBsp20Loader();
	~CBsp20Loader() {};

	/* Check the version of the map */
	bool CheckBuffer(const void* buf, size_t len) override;
	bool LoadMap(model_t* mod, const void* buffer, size_t length) override;

	void AddPointToVolume(vec3_t point);

	vec3_t mins;
	vec3_t maxs;

	/* Lump loading functions */
	void LoadEntityLump   (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 0 */
	void LoadPlaneLump    (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 1 */
	void LoadTexDatLump   (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 2 */
	void LoadVertexLump   (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 3 */
	void LoadVisLump      (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 4 */
	void LoadNodeLump     (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 5 */
	void LoadTexInfoLump  (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 6 */
	void LoadFaceLump     (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 7 */
	void LoadLightingLump (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 8 */
	void LoadOcclusionLump(bsp20::lump_t lump, const void* buf, size_t length);  /* lump 9 */
	void LoadLeafLump     (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 10 */
	void LoadFaceIdLump   (bsp20::lump_t lump, const void* buf, size_t length){};/* lump 11, unsupported */
	void LoadEdgeLump     (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 12 */
	void LoadSurfEdgeLump (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 13 */
	void LoadModelLump    (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 14 */
	void LoadWLightLump   (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 15 */
	void LoadLeafFaceLump (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 16 */
	void LoadLeafBrushLump(bsp20::lump_t lump, const void* buf, size_t length);  /* lump 17 */
	void LoadBrushLump    (bsp20::lump_t lump, const void* buf, size_t length);  /* lump 18 */
};

static CBsp20Loader g_Bsp20Loader;

CBsp20Loader::CBsp20Loader() :
	IMapLoader(g_bsp20_extensions, ARRAYSIZE(g_bsp20_extensions))
{

}

bool CBsp20Loader::CheckBuffer(const void* buf, size_t len)
{
	if(len < sizeof(bsp20::dheader_t) || !buf) return false;
	bsp20::dheader_t* hdr = (bsp20::dheader_t*)buf;
	return hdr->ident == bsp20::BSP_HEADER && hdr->version == 20;
}

bool CBsp20Loader::LoadMap(model_t* mod, const void* buffer, size_t length)
{
	g_bsp20_model = mod;
	bsp20::dheader_t* hdr = (bsp20::dheader_t*)buffer;

	Con_Printf("CBsp20Loader::LoadModel: Loading map.\n");
	Host_Error("CBsp20Loader::Unsupported Map Version\n");

	VectorClear(mins);
	VectorClear(maxs);

	/* Load the lumps */
	this->LoadEntityLump(hdr->lumps[bsp20::_LUMP_ENTITIES], buffer, length);
	this->LoadPlaneLump(hdr->lumps[bsp20::_LUMP_PLANES], buffer, length);
	this->LoadTexDatLump(hdr->lumps[bsp20::_LUMP_TEXDATA], buffer, length);
	this->LoadVertexLump(hdr->lumps[bsp20::_LUMP_VERTEXES], buffer, length);
	return true;
}

void CBsp20Loader::LoadEntityLump   (bsp20::lump_t lump, const void* buf, size_t length)
{
	/* Entity data is stored in a key-values format */
	KeyValues* pEnts = new KeyValues();
	pEnts->ParseString((const char*)((uintptr_t)buf + lump.fileofs), false, lump.filelen);
}

void CBsp20Loader::LoadPlaneLump    (bsp20::lump_t lump, const void* buf, size_t length)
{
	bsp20::dplane_t* planes = (bsp20::dplane_t*)((uintptr_t)buf + lump.fileofs); 

	/* Compute number of planes the BSP has and allocate space for it in the correct mempool */
	size_t num_planes = lump.filelen / sizeof(bsp20::dplane_t);
	g_bsp20_model->numplanes = num_planes;
	g_bsp20_model->planes = (mplane_t*)Mem_Malloc(g_bsp20_model->mempool, num_planes * sizeof(mplane_t));

	for(size_t i = 0; i < num_planes; i++)
	{
		bsp20::dplane_t plane = planes[i];
		mplane_t* dst = &g_bsp20_model->planes[i];

		dst->dist = plane.dist;
		VectorCopy(plane.normal, dst->normal);
		dst->type = plane.type;
		/* compute sign bits */
		/* sign = signx + (signy<<1) + (signz<<1) */
		union {
			float v[3];
			unsigned int i[3];
		} _sign;
		VectorCopy(plane.normal, _sign.v);
		dst->signbits = (_sign.i[0] & BIT(31)) + ((_sign.i[1] & BIT(31)) << 1) + ((_sign.i[2] & BIT(31)) << 1);
	}
}

void CBsp20Loader::LoadTexDatLump   (bsp20::lump_t lump, const void* buf, size_t length)
{
}

void CBsp20Loader::LoadVertexLump   (bsp20::lump_t lump, const void* buf, size_t length)
{
	vec3_t* verticies = (vec3_t*)((uintptr_t)buf + lump.fileofs);
	
	size_t num_verts = lump.filelen / sizeof(vec3_t);
	g_bsp20_model->numvertexes = num_verts;
	g_bsp20_model->vertexes = (mvertex_t*)Mem_Malloc(g_bsp20_model->mempool, num_verts * sizeof(mvertex_t));

	for(size_t i = 0; i < num_verts; i++)
		VectorCopy(verticies[i], g_bsp20_model->vertexes[i].position);
}

void CBsp20Loader::LoadVisLump      (bsp20::lump_t lump, const void* buf, size_t length)
{
	
}

void CBsp20Loader::LoadNodeLump     (bsp20::lump_t lump, const void* buf, size_t length)
{
	bsp20::dnode_t* nodes = (bsp20::dnode_t*)((uintptr_t)buf + lump.fileofs);

	size_t num_nodes = lump.filelen / sizeof(bsp20::dnode_t);
	g_bsp20_model->nodes = (mnode_t*)Mem_Malloc(g_bsp20_model->mempool, num_nodes * sizeof(mnode_t));
	g_bsp20_model->numnodes = num_nodes;

	for(size_t i = 0; i < num_nodes; i++)
	{
		bsp20::dnode_t node = nodes[i];
		mnode_t* dst = &g_bsp20_model->nodes[i];
		
		dst->plane = &g_bsp20_model->planes[node.planenum];
		memcpy(&dst->minmaxs[0], node.mins, sizeof(float) * 3);
		memcpy(&dst->minmaxs[3], node.maxs, sizeof(float) * 3);

		/* Resolve child and parent nodes */
		dst->children[0] = &g_bsp20_model->nodes[node.children[0]];
		dst->children[1] = &g_bsp20_model->nodes[node.children[1]];
		dst->children[0]->parent = dst;
		dst->children[1]->parent = dst;
	
		dst->numsurfaces = node.numfaces;
		dst->firstsurface = node.firstface;

		dst->contents = 0;
		dst->plane = &g_bsp20_model->planes[node.planenum];
		dst->visframe = 0; /* ?????????? */
	}
}

void CBsp20Loader::LoadTexInfoLump  (bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadFaceLump     (bsp20::lump_t lump, const void* buf, size_t length)
{
	
}

void CBsp20Loader::LoadLightingLump (bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadOcclusionLump(bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadLeafLump     (bsp20::lump_t lump, const void* buf, size_t length)
{
	bsp20::dleaf_t* leafs = (bsp20::dleaf_t*)((uintptr_t)buf + lump.fileofs);

	size_t num_leafs = lump.filelen / sizeof(bsp20::dleaf_t);
	g_bsp20_model->numleafs = num_leafs;
	g_bsp20_model->leafs = (mleaf_t*)Mem_Malloc(g_bsp20_model->mempool, num_leafs * sizeof(mleaf_t));

	for(size_t i = 0; i < num_leafs; i++)
	{
		mleaf_t* dst = &g_bsp20_model->leafs[i];
		bsp20::dleaf_t leaf = leafs[i];

		
	}
}

void CBsp20Loader::LoadEdgeLump     (bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadSurfEdgeLump (bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadModelLump    (bsp20::lump_t lump, const void* buf, size_t length)
{
	bsp20::dmodel_t* models = (bsp20::dmodel_t*)((uintptr_t)buf + lump.fileofs);

	size_t num_models = lump.filelen / sizeof(bsp20::dmodel_t);
	g_bsp20_model->numsubmodels = num_models;
	g_bsp20_model->submodels = (dmodel_t*)Mem_Malloc(g_bsp20_model->mempool, sizeof(dmodel_t) * num_models);
	
	for(size_t i = 0; i < num_models; i++)
	{
		dmodel_t* dst = &g_bsp20_model->submodels[i];
		bsp20::dmodel_t model = models[i];

		dst->firstface = model.firstface;
		//dst->headnode = model.headnode;
		VectorCopy(model.mins, dst->mins);
		VectorCopy(model.maxs, dst->maxs);
		dst->numfaces = model.numfaces;
		VectorCopy(model.origin, dst->origin);
		dst->visleafs = 0; /* ??????????? */
	}
}

void CBsp20Loader::LoadWLightLump   (bsp20::lump_t lump, const void* buf, size_t length)
{
	
}

void CBsp20Loader::LoadLeafFaceLump (bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadLeafBrushLump(bsp20::lump_t lump, const void* buf, size_t length)
{

}

void CBsp20Loader::LoadBrushLump    (bsp20::lump_t lump, const void* buf, size_t length)
{
	
}

void CBsp20Loader::AddPointToVolume(vec3_t pt)
{
	if(pt[0] > maxs[0]) maxs[0] = pt[0];
	if(pt[1] > maxs[1]) maxs[1] = pt[1];
	if(pt[2] > maxs[2]) maxs[2] = pt[2];
	if(pt[0] < mins[0]) mins[0] = pt[0];
	if(pt[1] < mins[1]) mins[1] = pt[1];
	if(pt[2] < mins[2]) mins[2] = pt[2];
}