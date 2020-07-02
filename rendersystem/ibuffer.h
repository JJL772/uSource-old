#pragma once

#include <stddef.h>
#include <stdint.h>
#include <initializer_list>
#include "public/containers/list.h"

/* Defines the type of buffer */
enum class EBufferType
{
	HOST = 0, /* Host-only */
	SHARED,   /* Mapped in both host and GPU memory, changes are propagated every so-often */
	CLIENT,   /* GPU only */
};

/* A single node for a buffer format */
struct buffer_fmt_node_t
{
	size_t length;
};

class CBufferFormat
{
	List<buffer_fmt_node_t> nodes; 
public:
	CBufferFormat(std::initializer_list<buffer_fmt_node_t> fmtspec)
	{
		for(auto x : fmtspec)
			nodes.push_back(x);
	}

};

class IBuffer
{
public:
	virtual ~IBuffer() = 0;

	virtual EBufferType GetType() const = 0;

	/**
	 * Returns the size of the internal data store 
	 */ 
	virtual size_t GetSize() const = 0;

	/**
	 * Returns a const ref to the buffer format description 
	 */ 
	virtual const CBufferFormat& GetBufferFormatDesc() const = 0;

	/**
	 * Returns a ptr to the internal data store
	 * @return Pointer to the internal data store
	 */ 
	virtual void* GetStorage() = 0;

	/**
	 * Performs a memory lock operation propagating changes from host memory to client memory if type == SHARED
	 */ 
	virtual void Lock() = 0;

	/**
	 * Allocates the internal storage for n elements 
	 * @param num_elements the number of elements inside of the buffer
	 */ 
	virtual void AllocateStorage(size_t num_elements) = 0;

	/**
	 * Allocates and initializes storage, assumes the target buffer is the same format as this buffer
	 * @param num_elements The number of elements in this data store
	 * @param buffer Pointer to a buffer that's the SAME SIZE as this one will be
	 */ 
	virtual void AllocateInitStorage(size_t num_elements, void* buffer) = 0;

	/**
	 * Copies data from the specified buffer into this current buffer
	 * @param buffer Pointer to the buffer
	 * @param size Size (IN NUMBER OF ELEMENTS) of the buffer
	 */ 
	virtual void CopyIntoBuffer(void* buf, size_t num_elements) = 0;
};


/*
Vertex formats are quite simple. They're intended to be tightly packed arrays of verticies for GL rendering
*/
enum class EVertexFormat
{
	INT2 = 0,    /* 2d vector that uses integers */
	FLOAT2,      /* 2d vector that uses floats for all elements */
	DOUBLE2,     /* 2d vector that uses doubles for all elements */
	INT3,        /* 3d vector of ints */
	FLOAT3,      /* 3d vector of floats */
	DOUBLE3,     /* 3d vector of doubles */
	INT4,        /* 4d vector of ints */
	FLOAT4,      /* 4d vector of floats */
	DOUBLE4,     /* 4d vector of doubles */
};

enum class EIndexFormat
{
	BYTE = 0, /* 8 bits per index, so 255 verticies max */
	SHORT,    /* 16 bits per index, so 16k verticies max */
	INT,      /* 32 bits per index, so 4 billion verticies max */
};

class IVertexBuffer
{
public:
	virtual ~IVertexBuffer() = 0;

	/**
	 * Allocates internal buffer storage for this buffer.
	 * @param num_elements number of elements we should allocate storage for
	 */ 
	virtual void AllocateStorage(size_t num_elements) = 0;

	/**
	 * Allocates and initializes storage, assumes the target buffer is the same format as this buffer
	 * @param num_elements The number of elements in this data store
	 * @param buffer Pointer to a buffer that's the SAME SIZE as this one will be
	 */ 
	virtual void AllocateInitStorage(size_t num_elements, void* buffer) = 0;

	/**
	 * Returns a pointer to the internal storage if this buffer is shared or host-only
	 */ 
	virtual void* GetStorage() const = 0;

	/**
	 * Copies the buffer into this index buffer
	 * @param buf Pointer to the buffer
	 * @param length size in ELEMENTS of the buffer
	 */ 
	virtual void CopyIntoBuffer(void* buf, size_t length) = 0;

	/**
	 * Returns the format of the buffer
	 */ 
	virtual EVertexFormat GetFormat() const = 0;

	/**
	 * Performs a memory lock operation which will propagate changes in host memory to GPU memory
	 * if the buffer type is shared
	 */ 
	virtual void Lock() = 0;

	/**
	 * Returns the type of this buffer.
	 */ 
	virtual EBufferType GetType() const = 0;

	/**
	 * Returns the size of the internal data store
	 */ 
	virtual size_t GetSize() const = 0;
};

class IIndexBuffer
{
public:
	virtual ~IIndexBuffer() = 0;

	/**
	 * Allocates internal buffer storage for this buffer.
	 * @param num_elements number of elements we should allocate storage for
	 */ 
	virtual void AllocateStorage(size_t num_elements) = 0;

	/**
	 * Returns a pointer to the internal storage if this buffer is shared or host-only
	 */ 
	virtual void* GetStorage() const = 0;

	/**
	 * Copies the buffer into this index buffer
	 * @param buf Pointer to the buffer
	 * @param length size in bytes of the buffer
	 */ 
	virtual void CopyIntoBuffer(void* buf, size_t length) = 0;

	/**
	 * Returns the format of the buffer
	 */ 
	virtual EIndexFormat GetFormat() const = 0;

	/**
	 * Performs a memory lock operation which will propagate changes in host memory to GPU memory
	 * if the buffer type is shared
	 */ 
	virtual void Lock() = 0;

	/**
	 * Returns the type of this buffer.
	 */ 
	virtual EBufferType GetType() const = 0;

	/**
	 * Returns the size of the internal data store
	 */ 
	virtual size_t GetSize() const = 0;
};