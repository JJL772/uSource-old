/**
 * 
 * Bsp version 20 definitions
 * 
 */ 
#pragma once

//#include "xash3d_types.h"

/* Everything needs to be tightly packed ! */
#pragma pack(1)


/* Undefine all the crap from the other bspfile.h */
#undef BSP_HEADER						
#undef MAX_BRUSH_LIGHTMAP_DIM_WITHOUT_BORDER 			
#undef MAX_BRUSH_LIGHTMAP_DIM_INCLUDING_BORDER			
#undef MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER			
#undef MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER			
#undef MAX_LIGHTMAP_DIM_WITHOUT_BORDER				
#undef MAX_LIGHTMAP_DIM_INCLUDING_BORDER				
#undef MAX_LIGHTSTYLES						
#undef MIN_MAP_DISP_POWER					
#undef MAX_MAP_DISP_POWER					
#undef MAX_DISP_CORNER_NEIGHBORS							
#undef MAX_MAP_MODELS						
#undef MAX_MAP_BRUSHES						
#undef MAX_MAP_ENTITIES						
#undef MAX_MAP_TEXINFO						
#undef MAX_MAP_TEXDATA						
#undef MAX_MAP_DISPINFO						
#undef MAX_MAP_DISP_VERTS					
#undef MAX_MAP_DISP_TRIS						
#undef MAX_DISPVERTS						
#undef MAX_DISPTRIS						
#undef MAX_MAP_AREAS						
#undef MAX_MAP_AREA_BYTES					
#undef MAX_MAP_AREAPORTALS					
#undef MAX_MAP_PLANES						
#undef MAX_MAP_NODES						
#undef MAX_MAP_BRUSHSIDES					
#undef MAX_MAP_LEAFS						
#undef MAX_MAP_VERTS						
#undef MAX_MAP_VERTNORMALS					
#undef MAX_MAP_VERTNORMALINDICES					
#undef MAX_MAP_FACES						
#undef MAX_MAP_LEAFFACES						
#undef MAX_MAP_LEAFBRUSHES 					
#undef MAX_MAP_PORTALS						
#undef MAX_MAP_CLUSTERS						
#undef MAX_MAP_LEAFWATERDATA					
#undef MAX_MAP_PORTALVERTS					
#undef MAX_MAP_EDGES						
#undef MAX_MAP_SURFEDGES						
#undef MAX_MAP_LIGHTING						
#undef MAX_MAP_VISIBILITY					
#undef MAX_MAP_TEXTURES						
#undef MAX_MAP_WORLDLIGHTS					
#undef MAX_MAP_CUBEMAPSAMPLES					
#undef MAX_MAP_OVERLAYS						
#undef MAX_MAP_WATEROVERLAYS					
#undef MAX_MAP_TEXDATA_STRING_DATA				
#undef MAX_MAP_TEXDATA_STRING_TABLE				
#undef MAX_MAP_PRIMITIVES					
#undef MAX_MAP_PRIMVERTS						
#undef MAX_MAP_PRIMINDICES					
#undef HEADER_LUMPS

/* Wrap it all in a namespace */
namespace bsp20
{
static constexpr unsigned int BSP_HEADER						= (('P'<<24)+('S'<<16)+('B'<<8)+'V');
static constexpr unsigned int MAX_BRUSH_LIGHTMAP_DIM_WITHOUT_BORDER 			= 32;
static constexpr unsigned int MAX_BRUSH_LIGHTMAP_DIM_INCLUDING_BORDER			= 35;
static constexpr unsigned int MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER			= 125;
static constexpr unsigned int MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER			= 128;
static constexpr unsigned int MAX_LIGHTMAP_DIM_WITHOUT_BORDER				= MAX_DISP_LIGHTMAP_DIM_WITHOUT_BORDER;
static constexpr unsigned int MAX_LIGHTMAP_DIM_INCLUDING_BORDER				= MAX_DISP_LIGHTMAP_DIM_INCLUDING_BORDER;
static constexpr unsigned int MAX_LIGHTSTYLES						= 64;
static constexpr unsigned int MIN_MAP_DISP_POWER					= 2;
static constexpr unsigned int MAX_MAP_DISP_POWER					= 4	;
static constexpr unsigned int MAX_DISP_CORNER_NEIGHBORS					= 4;
static constexpr unsigned int NUM_DISP_POWER_VERTS(unsigned int power)			{ return ( ((1 << (power)) + 1) * ((1 << (power)) + 1) ); };
static constexpr unsigned int NUM_DISP_POWER_TRIS(unsigned int power)			{ return ( (1 << (power)) * (1 << (power)) * 2 ); };
static constexpr unsigned int MAX_MAP_MODELS						= 1024;
static constexpr unsigned int MAX_MAP_BRUSHES						= 8192;
static constexpr unsigned int MAX_MAP_ENTITIES						= 8192;
static constexpr unsigned int MAX_MAP_TEXINFO						= 12288;
static constexpr unsigned int MAX_MAP_TEXDATA						= 2048;
static constexpr unsigned int MAX_MAP_DISPINFO						= 2048;
static constexpr unsigned int MAX_MAP_DISP_VERTS					= ( MAX_MAP_DISPINFO * ((1<<MAX_MAP_DISP_POWER)+1) * ((1<<MAX_MAP_DISP_POWER)+1) );
static constexpr unsigned int MAX_MAP_DISP_TRIS						= ( (1 << MAX_MAP_DISP_POWER) * (1 << MAX_MAP_DISP_POWER) * 2 );
static constexpr unsigned int MAX_DISPVERTS						= NUM_DISP_POWER_VERTS( MAX_MAP_DISP_POWER );
static constexpr unsigned int MAX_DISPTRIS						= NUM_DISP_POWER_TRIS( MAX_MAP_DISP_POWER );
static constexpr unsigned int MAX_MAP_AREAS						= 256;
static constexpr unsigned int MAX_MAP_AREA_BYTES					= (MAX_MAP_AREAS/8);
static constexpr unsigned int MAX_MAP_AREAPORTALS					= 1024;
static constexpr unsigned int MAX_MAP_PLANES						= 65536;
static constexpr unsigned int MAX_MAP_NODES						= 65536;
static constexpr unsigned int MAX_MAP_BRUSHSIDES					= 65536;
static constexpr unsigned int MAX_MAP_LEAFS						= 65536;
static constexpr unsigned int MAX_MAP_VERTS						= 65536;
static constexpr unsigned int MAX_MAP_VERTNORMALS					= 256000;
static constexpr unsigned int MAX_MAP_VERTNORMALINDICES					= 256000;
static constexpr unsigned int MAX_MAP_FACES						= 65536;
static constexpr unsigned int MAX_MAP_LEAFFACES						= 65536;
static constexpr unsigned int MAX_MAP_LEAFBRUSHES 					= 65536;
static constexpr unsigned int MAX_MAP_PORTALS						= 65536;
static constexpr unsigned int MAX_MAP_CLUSTERS						= 65536;
static constexpr unsigned int MAX_MAP_LEAFWATERDATA					= 32768;
static constexpr unsigned int MAX_MAP_PORTALVERTS					= 128000;
static constexpr unsigned int MAX_MAP_EDGES						= 256000;
static constexpr unsigned int MAX_MAP_SURFEDGES						= 512000;
static constexpr unsigned int MAX_MAP_LIGHTING						= 0x1000000;
static constexpr unsigned int MAX_MAP_VISIBILITY					= 0x1000000;
static constexpr unsigned int MAX_MAP_TEXTURES						= 1024;
static constexpr unsigned int MAX_MAP_WORLDLIGHTS					= 8192;
static constexpr unsigned int MAX_MAP_CUBEMAPSAMPLES					= 1024;
static constexpr unsigned int MAX_MAP_OVERLAYS						= 512 ;
static constexpr unsigned int MAX_MAP_WATEROVERLAYS					= 16384;
static constexpr unsigned int MAX_MAP_TEXDATA_STRING_DATA				= 256000;
static constexpr unsigned int MAX_MAP_TEXDATA_STRING_TABLE				= 65536;
static constexpr unsigned int MAX_MAP_PRIMITIVES					= 32768;
static constexpr unsigned int MAX_MAP_PRIMVERTS						= 65536;
static constexpr unsigned int MAX_MAP_PRIMINDICES					= 65536;

enum
{
	CHILDNODE_UPPER_RIGHT=0,
	CHILDNODE_UPPER_LEFT=1,
	CHILDNODE_LOWER_LEFT=2,
	CHILDNODE_LOWER_RIGHT=3
};

enum
{
	CORNER_LOWER_LEFT=0,
	CORNER_UPPER_LEFT=1,
	CORNER_UPPER_RIGHT=2,
	CORNER_LOWER_RIGHT=3
};

enum
{
	NEIGHBOREDGE_LEFT=0,
	NEIGHBOREDGE_TOP=1,
	NEIGHBOREDGE_RIGHT=2,
	NEIGHBOREDGE_BOTTOM=3
};

typedef enum
{
	CORNER_TO_CORNER=0,
	CORNER_TO_MIDPOINT=1,
	MIDPOINT_TO_CORNER=2
};

typedef enum
{
	ORIENTATION_CCW_0=0,
	ORIENTATION_CCW_90=1,
	ORIENTATION_CCW_180=2,
	ORIENTATION_CCW_270=3
};

enum
{
	_LUMP_ENTITIES					= 0,
	_LUMP_PLANES					= 1,
	_LUMP_TEXDATA					= 2,
	_LUMP_VERTEXES					= 3,
	_LUMP_VISIBILITY				= 4,
	_LUMP_NODES					= 5,
	_LUMP_TEXINFO					= 6,
	_LUMP_FACES					= 7,
	_LUMP_LIGHTING					= 8,
	_LUMP_OCCLUSION					= 9,
	_LUMP_LEAFS					= 10,
	_LUMP_FACEIDS					= 11,
	_LUMP_EDGES					= 12,	
	_LUMP_SURFEDGES					= 13,	
	_LUMP_MODELS					= 14,	
	_LUMP_WORLDLIGHTS				= 15,
	_LUMP_LEAFFACES					= 16,	
	_LUMP_LEAFBRUSHES				= 17,	
	_LUMP_BRUSHES					= 18,	
	_LUMP_BRUSHSIDES				= 19,	
	_LUMP_AREAS					= 20,	
	_LUMP_AREAPORTALS				= 21,	
	_LUMP_UNUSED0					= 22,
	_LUMP_UNUSED1					= 23,
	_LUMP_UNUSED2					= 24,
	_LUMP_UNUSED3					= 25,
	_LUMP_DISPINFO					= 26,
	_LUMP_ORIGINALFACES				= 27,
	_LUMP_PHYSDISP					= 28,
	_LUMP_PHYSCOLLIDE				= 29,
	_LUMP_VERTNORMALS				= 30,
	_LUMP_VERTNORMALINDICES				= 31,
	_LUMP_DISP_LIGHTMAP_ALPHAS			= 32,
	_LUMP_DISP_VERTS				= 33,
	_LUMP_DISP_LIGHTMAP_SAMPLE_POSITIONS 		= 34,
	_LUMP_GAME_LUMP					= 35,
	_LUMP_LEAFWATERDATA				= 36,
	_LUMP_PRIMITIVES				= 37,
	_LUMP_PRIMVERTS					= 38,
	_LUMP_PRIMINDICES				= 39,
	_LUMP_PAKFILE					= 40,
	_LUMP_CLIPPORTALVERTS				= 41,
	_LUMP_CUBEMAPS					= 42,
	_LUMP_TEXDATA_STRING_DATA			= 43,
	_LUMP_TEXDATA_STRING_TABLE			= 44,
	_LUMP_OVERLAYS					= 45,
	_LUMP_LEAFMINDISTTOWATER			= 46,
	_LUMP_FACE_MACRO_TEXTURE_INFO			= 47,
	_LUMP_DISP_TRIS					= 48,
	_LUMP_PHYSCOLLIDESURFACE			= 49,
	_LUMP_WATEROVERLAYS              		= 50,
	_LUMP_LEAF_AMBIENT_INDEX_HDR			= 51,
	_LUMP_LEAF_AMBIENT_INDEX         		= 52,
	_LUMP_LIGHTING_HDR				= 53,
	_LUMP_WORLDLIGHTS_HDR				= 54,
	_LUMP_LEAF_AMBIENT_LIGHTING_HDR			= 55,
	_LUMP_LEAF_AMBIENT_LIGHTING			= 56,
	_LUMP_XZIPPAKFILE				= 57,
	_LUMP_FACES_HDR					= 58,
	_LUMP_MAP_FLAGS                  		= 59,
	_LUMP_OVERLAY_FADES				= 60,
};

enum class EBsp20LumpVersions
{
	LUMP_LIGHTING_VERSION          			= 1,
	LUMP_FACES_VERSION             			= 1,
	LUMP_OCCLUSION_VERSION         			= 2,
	LUMP_LEAFS_VERSION			   	= 1,
	LUMP_LEAF_AMBIENT_LIGHTING_VERSION 		= 1,
};

static constexpr unsigned int HEADER_LUMPS = 64;

struct lump_t
{
	int	fileofs;	/* lump offset in BSP */
	int	filelen;	/* lump length */
	int	version;	/* lump ver */
	char	fourCC[4];	/* Lump identification */
};

struct dheader_t
{
	int	ident;                /* = BSP_HEADER */
	int	version;              /* BSP version */
	lump_t	lumps[HEADER_LUMPS];  /* lump array */
	int	mapRevision;          /* Map version */
};

struct dplane_t
{
	vec3_t	normal;
	float	dist;	
	int	type;	/* Plane axis */
};

struct dedge_t
{
	unsigned short	v[2];	/* indicies of the verticies */
};

struct dbrush_t
{
	int	firstside;	/* first brushside */
	int	numsides;	/* Number of brishsides */
	int	contents;	/* what the brush contains */
};

struct dbrushside_t
{
	unsigned short	planenum;	/* facing out of the leaf */
	short		texinfo;	/* texture info */
	short		dispinfo;	/* disp info */
	short		bevel;		/* is the side a bevel plane? */
};

struct dnode_t
{
	int		planenum;	// index into plane array
	int		children[2];	// negative numbers are -(leafs + 1), not nodes
	short		mins[3];	// for frustum culling
	short		maxs[3];
	unsigned short	firstface;	// index into face array
	unsigned short	numfaces;	// counting both sides
	short		area;		// If all leaves below this node are in the same area, then
					// this is the area index. If not, this is -1.
	short		paddding;	// pad to 32 bytes length
};

struct dleaf_t
{
	int			contents;		// OR of all brushes (not needed?)
	short			cluster;		// cluster this leaf is in
	short			area	:9;		// area this leaf is in
	short			flags	:7;		// flags
	short			mins[3];		// for frustum culling
	short			maxs[3];
	unsigned short		firstleafface;		// index into leaffaces
	unsigned short		numleaffaces;
	unsigned short		firstleafbrush;		// index into leafbrushes
	unsigned short		numleafbrushes;
	short			leafWaterDataID;	// -1 for not in water
};

/* Flags for texinfo_t::flags */
enum
{
	SURF_LIGHT = 0x1,
	SURF_SKY2D = 0x2,
	SURF_SKY = 0x4,
	SURF_WARP = 0x8,
	SURF_TRANS = 0x10,
	SURF_NOPORTAL = 0x20,
	SURF_TRIGGER = 0x40,
	SURF_NODRAW = 0x80,
	SURF_HINT = 0x100,
	SURF_SKIP = 0x200,
	SURF_NOLIGHT = 0x400,
	SURF_BUMPLIGHT = 0x800,
	SURF_NOSHADOWS = 0x1000,
	SURF_NODECALS = 0x2000,
	SURF_NOCHOP = 0x4000,
	SURF_HITBOX = 0x8000,
};

struct texinfo_t
{
	float	textureVecs[2][4];	// [s/t][xyz offset]
	float	lightmapVecs[2][4];	// [s/t][xyz offset] - length is in units of texels/area
	int	flags;			// miptex flags	overrides
	int	texdata;		// Pointer to texture name, size, etc.
};

struct dtexdata_t
{
	vec3_t	reflectivity;		// RGB reflectivity
	int	nameStringTableID;	// index into TexdataStringTable
	int	width, height;		// source image
	int	view_width, view_height;
};

struct dmodel_t
{
	vec3_t	mins, maxs;		// bounding box
	vec3_t	origin;			// for sounds or lights
	int	headnode;		// index into node array
	int	firstface, numfaces;	// index into face array
};

struct dvis_t
{
	int	numclusters;
	// int	byteofs[numclusters][2]
};

struct dgamelump_t
{
	int		id;		// gamelump ID
	unsigned short	flags;		// flags
	unsigned short	version;	// gamelump version
	int		fileofs;	// offset to this gamelump
	int		filelen;	// length
};

struct dgamelumpheader_t
{
	int lumpCount;	// number of game lumps
	//dgamelump_t gamelump[lumpCount];
};

}

#pragma pack()