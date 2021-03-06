#include "Mesh.h"
#include <Circe/Circe.h>
#include <iostream>

namespace Medusa
{
	Attribute::Attribute(const std::string& name, const int& location, const int& size):name(name), location(location), size(size)
	{}


	VertexSpecs::VertexSpecs(const std::initializer_list<Attribute>& attributes)
	{
		for(Attribute attribute : attributes)
		{
			m_attributes.push_back(attribute);
		}
	}
			
	VertexSpecsIterator VertexSpecs::begin()
	{
		return VertexSpecsIterator(m_attributes);
	}

	VertexSpecsIterator::VertexSpecsIterator(const vector<Attribute>& attributes):m_attributes(attributes), index(0)
	{}
			
	bool VertexSpecsIterator::hasNext()
	{
		return index < m_attributes.size();
	}
	
	void VertexSpecsIterator::next()
	{
		++index;
	}
			
	Attribute* VertexSpecsIterator::operator->()
	{
		return &m_attributes[index];
	}
	
	void Mesh::draw(const int& culling) const
	{
		m_resource->draw(m_meshType, culling*m_faceOrientation);
	}
	
	Mesh::Mesh(const MeshData& data, const MeshType& meshType):ResourceHandle(data), m_meshType(meshType)
	{}
	
	void Mesh::setFaceOrientation(const int& orientation)
	{
		m_faceOrientation = orientation;
	}
	
	/*Mesh::Mesh(const Mesh& other):meshIndex(other.meshIndex), m_meshType(other.m_meshType), renderType(other.renderType)
	{}*/

	MeshData::MeshData(): vertices(), indices(), vertexBuffer(0), indexBuffer(0)
	{}
	
	void MeshData::draw(const GLenum& renderType, const int& culling) const
	{
		if(culling == -1){
			glEnable(GL_CULL_FACE);
			glCullFace(GL_BACK);
		}else if(culling == 0){
			glDisable(GL_CULL_FACE);
		}else{
			glEnable(GL_CULL_FACE);
			glCullFace(GL_FRONT);
		}
		glBindVertexArray(vao);
		glDrawElements(renderType, indices.size(), GL_UNSIGNED_INT, (void*)0);
		glBindVertexArray(0);
	}
	

	
	void OBJLoader::readFile(const string& fileName, MeshData& meshData)
	{
		std::FILE* file = fopen(fileName.c_str(), "r");
		if(file == NULL)
		{
			CIRCE_ERROR("Could not open "+fileName+" mesh.");
		}
		
		std::map<unsigned int, TempVertex> vertices;
		std::map<unsigned int, Circe::Vec<3>> normals;
		std::map<unsigned int, Circe::Vec<2>> textCoords;
		std::vector<unsigned int> resultIndices;
		unsigned int vIndex(1);
		unsigned int vtIndex(1);
		unsigned int vnIndex(1);
		bool hasNormals = false;
		bool hasTextures = false;
		
		char lineStart[32];
		int firstWord = fscanf(file, "%s", lineStart);
		while(firstWord != EOF)
		{
			if(strcmp(lineStart, "v")==0)		//Position coords
			{
				TempVertex vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				vertices[vIndex] = vertex;
				vIndex++;				
			}
			else if(strcmp(lineStart, "vt")==0) //Texture coords
			{
				float u, v;
				fscanf(file, "%f %f\n", &u, &v);
				textCoords[vtIndex]=Circe::Vec<2>(u, v);
				vtIndex++;
				hasTextures = true;
			}
			else if(strcmp(lineStart, "vn")==0) //Normal coords
			{
				float nx, ny, nz;
				fscanf(file, "%f %f %f\n", &nx, &ny, &nz);
				normals[vnIndex]=Circe::Vec<3>(nx, ny, nz);
				hasNormals = true;
				vnIndex++;
			}
			else if(strcmp(lineStart, "f")==0) //Facets
			{
				if(hasTextures && hasNormals)
				{
					unsigned int p[4], u[4], n[4];
					
					int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d %d/%d/%d\n", &p[0], &u[0], &n[0], &p[1], &u[1], &n[1], &p[2], &u[2], &n[2], &p[3], &u[3], &n[3]);
					if(matches == 9 || matches ==12){//It's a triangle or a quad
						for(int i = 0; i<(matches/3); i++)
						{						
							Circe::Vec<2> uv = textCoords[u[i]];
							Circe::Vec<3> normal = normals[n[i]];
							
							vertices[p[i]].u += uv(0);
							vertices[p[i]].v += uv(1);
							vertices[p[i]].textCount++;
							
							vertices[p[i]].nx += normal(0);
							vertices[p[i]].ny += normal(1);
							vertices[p[i]].nz += normal(2);
							vertices[p[i]].normalCount++;

							
						}
						
						resultIndices.push_back(p[0]-1);
						resultIndices.push_back(p[1]-1);
						resultIndices.push_back(p[2]-1);
						if(matches == 12)
						{
							resultIndices.push_back(p[0]-1);
							resultIndices.push_back(p[2]-1);
							resultIndices.push_back(p[3]-1);
						}
						
					}else{
						CIRCE_ERROR("Mesh "+fileName+" format not handled.");
						break;
					}
				}
				else if(!hasTextures && !hasNormals)
				{
					/*unsigned int p[4];
					
					int matches = fscanf(file, "%d %d %d %d\n", &p[0], &p[1], &p[2], &p[3]);
					
					if(matches == 3 || matches ==4){//It's a triangle or a quad
						
						Circe::Vec<3> line1(vertices[p[1]].x-vertices[p[0]].x, vertices[p[1]].y-vertices[p[0]].y, vertices[p[1]].z-vertices[p[0]].z);
						Circe::Vec<3> line2(vertices[p[2]].x-vertices[p[0]].x, vertices[p[2]].y-vertices[p[0]].y, vertices[p[2]].z-vertices[p[0]].z);
						Circe::Vec<3> normal = Circe::cross(line1, line2);
						
						for(int i = 0; i<matches; i++)
						{						
							vertices[p[i]].u += 0;//uv(0);
							vertices[p[i]].v += 0;//uv(1);
							vertices[p[i]].textCount++;
							
							vertices[p[i]].nx += 0;//normal(0);
							vertices[p[i]].ny += 0;//normal(1);
							vertices[p[i]].nz += 0;//normal(2);
							vertices[p[i]].normalCount++;
						}
						
						resultIndices.push_back(p[0]-1);
						resultIndices.push_back(p[1]-1);
						resultIndices.push_back(p[2]-1);
						if(matches == 4)
						{
							resultIndices.push_back(p[0]-1);
							resultIndices.push_back(p[2]-1);
							resultIndices.push_back(p[3]-1);
						}
					}else{
						CIRCE_ERROR("Mesh "+fileName+" format not handled.");
						break;
					}
					*/
				}
				else if(hasTextures && !hasNormals)
				{/*
					unsigned int p[4], u[4];
					
					int matches = fscanf(file, "%d/%d %d/%d %d/%d %d/%d\n", &p[0], &u[0], &p[1], &u[1], &p[2], &u[2], &p[3], &u[3]);
					if(matches == 6 || matches ==8){//It's a triangle or a quad
						for(int i = 0; i<(matches/2); i++)
						{						
							Circe::Vec<2> uv = textCoords[u[i]];
							
							vertices[p[i]].u += uv(0);
							vertices[p[i]].v += uv(1);
							vertices[p[i]].textCount++;
							
						}
						
						resultIndices.push_back(p[0]-1);
						resultIndices.push_back(p[1]-1);
						resultIndices.push_back(p[2]-1);
						if(matches == 8)
						{
							resultIndices.push_back(p[0]-1);
							resultIndices.push_back(p[2]-1);
							resultIndices.push_back(p[3]-1);
						}
						
					}else{
						CIRCE_ERROR("Mesh "+fileName+" format not handled.");
						break;
					}*/
				}
				
			}
			firstWord = fscanf(file, "%s", lineStart);
		}
		
		std::vector<Vertex> resultVertices;
		for(std::pair<unsigned int, TempVertex> vertexPair : vertices)
		{
			TempVertex vertex(vertexPair.second);
			Vertex resultVertex;
			resultVertex.x = vertex.x;
			resultVertex.y = vertex.y;
			resultVertex.z = vertex.z;
			resultVertex.u = vertex.u/vertex.textCount;
			resultVertex.v = vertex.v/vertex.textCount;
			resultVertex.nx = vertex.nx/vertex.normalCount;
			resultVertex.ny = vertex.ny/vertex.normalCount;
			resultVertex.nz = vertex.nz/vertex.normalCount;
			resultVertices.push_back(resultVertex);
		}

		meshData.vertices = resultVertices;
		meshData.indices = resultIndices;
		fclose(file);
	}
	
	void OBJLoader::load(const string& folderLocation, const string& fileName, MeshData& mesh)
	{
		
		readFile(folderLocation+fileName, mesh);
		
		std::vector<Vertex> vertices = mesh.vertices;
		std::vector<unsigned int> indices = mesh.indices;
		
		GLuint vao;
		glGenVertexArrays(1, &vao);
		mesh.vao=vao;
		glBindVertexArray(vao);
		
		GLuint vertexBuffer;
		glGenBuffers(1, &vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);
		mesh.vertexBuffer = vertexBuffer;
		
		GLuint indexBuffer;
		glGenBuffers(1, &indexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);
		mesh.indexBuffer = indexBuffer;
		
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) 0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 3));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*) (sizeof(float) * 6));
	
		glBindVertexArray(0);
		
		mesh.m_fileName = fileName;
		
		CIRCE_INFO("Mesh "+fileName+" loaded.");
	}
	
	void OBJLoader::unload(MeshData& mesh)
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &(mesh.vertexBuffer));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &(mesh.indexBuffer));
		
		CIRCE_INFO("Mesh "+mesh.m_fileName+" unloaded.");
	}
	
}