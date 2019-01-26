#pragma once
#include "Resource.h"
#include <vector>
#include <glad/glad.h>
#include <map>

namespace Medusa
{	
	class Resource;
	struct Attribute;
	class VertexSpecs;
	class VertexSpecsIterator;
	class Mesh;
	class MeshLoader;
	struct Vertex;
	class Mesh;
	class MeshData;
	
	
	struct Attribute
	{
		Attribute(const std::string& name, const int& location, const int& size);
		std::string name;
		int location;
		int size;
	};

	class VertexSpecs
	{
		public:
			VertexSpecs(const std::initializer_list<Attribute>& attributes);
			
			VertexSpecsIterator begin();
			
		private:			
			vector<Attribute> m_attributes;
			friend class VertexSpecsIterator;
	};
	
	class VertexSpecsIterator
	{
		public:
			VertexSpecsIterator(const vector<Attribute>& attributes);
			
			bool hasNext();
			void next();
			
			Attribute* operator->();
			
		private:
			int index;
			vector<Attribute> m_attributes;
	};
	
	enum MeshType{TRIANGLE_RENDERING = GL_TRIANGLES, WIRE_RENDERING = GL_LINES};
	
	/**
	Mesh is a container, a handle to be manipulated outside, encapsulating the mesh specs and 
	the mesh data (in the form of an index). meshResource.render(mesh.getIndex());
	**/
	class Mesh : public ResourceHandle<Mesh, MeshData>
	{
		public:
			Mesh(MeshData& data, const MeshType& meshType);	
			
			void draw();
			
		private:		
			unsigned int meshIndex;
			MeshType m_meshType;
			GLenum renderType;
	};	
	
	
	/**
	MeshData contains the data of the mesh, internal to the mesh resource manager.
	**/
	class MeshData
	{
		public:
			MeshData();
			
			void draw(const GLenum& renderType);
		
		private:
			std::vector<Vertex> vertices;
			std::vector<unsigned int> indices;
			GLuint vao;
			GLuint vertexBuffer;
			GLuint indexBuffer;
			std::string m_fileName;
			friend class MeshLoader;
	};
	
	class MeshLoader : public ResourceLoader<MeshData>
	{
		public:
			MeshLoader(const VertexSpecs& specs);
			
			virtual void readOBJFile(const string& file, MeshData& mesh);
			
			virtual void load(const string& folderLocation, const string& fileName, MeshData& resource);
			
			virtual void unload(MeshData& resource);
			
		private:
			vector<size_t> m_attributes;
			VertexSpecs m_specs;		
	};
	
	struct TempVertex
	{
		//TempVertex();
		float x, y, z;
		float nx, ny, nz;
		float u, v;
		unsigned int normalCount=0;
		unsigned int textCount=0;
	};
	
	//Internal struct
	struct Vertex
	{		
		float x, y, z; 			// Position
		float nx, ny, nz; 		// Normal
		float u, v;				// Texture
	};

}