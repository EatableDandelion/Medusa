#pragma once
#include <iostream>
#include "Resource.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Circe/Circe.h>
#include <memory>
#include <unordered_map>
#include <Mesh.h>
#include <Texture.h>

namespace Medusa
{	
	using namespace Circe;
	using namespace std;
	
	struct Uniform;
	template<class T> struct TypedUniform;
	class Shader;
	class ShaderData;
	class ShaderLoader;
	
	struct Uniform
	{
		public:
			Uniform();
			virtual void update(const GLint& location)=0;
			
		protected:
			GLint m_location;
	};
	
	template<class T>
	struct TypedUniform:Uniform
	{	
		public:
			TypedUniform(const string& name, const T& value, const string& GLSLName):m_name(name), m_value(value), m_GLSLName(GLSLName)
			{}
			
			void set(const T& value)
			{
				m_value=value;
			}
		
		protected:
			std::string m_name;
			T m_value;
			std::string m_GLSLName;
	};
	
	struct UniformVec2:TypedUniform<Vec<2>>
	{	
		UniformVec2(const string& name, const Vec<2>& value);
		virtual void update(const GLint& location);
	};
	
	struct UniformVec3:TypedUniform<Vec<3>>
	{	
		UniformVec3(const string& name, const Vec<3>& value);	
		virtual void update(const GLint& location);
	};
	
	struct UniformVec4:TypedUniform<Vec<4>>
	{	
		UniformVec4(const string& name, const Vec<4>& value);
		virtual void update(const GLint& location);
	};
	
	struct UniformMat4:TypedUniform<Mat<4>>
	{		
		UniformMat4(const string& name, const Mat<4>& value);
		virtual void update(const GLint& location);
	};
	
	struct UniformFloat:TypedUniform<float>
	{	
		UniformFloat(const string& name, const float& value);
		virtual void update(const GLint& location);
	};
	
	struct UniformTexture:TypedUniform<Texture>
	{
		UniformTexture(const string& name, const Texture& value);
		virtual void update(const GLint& location);
	};
	
	
	class Material
	{
		public:
			Material();
			
			~Material();
					
			template<class T>
			void setUniform(const string& name, const T& value)
			{ 				
				size_t id = Circe::getId(name);
				if(uniforms.find(id) == uniforms.end())
				{
					addUniform(name, id, value);
				}else{
					dynamic_pointer_cast<TypedUniform<T>>(uniforms[id])->set(value);
				}				
			}
						
			void uploadUniform(size_t id, const GLint& location) const;
			
		private:
			unordered_map<size_t, shared_ptr<Uniform>> uniforms;
			
			void addUniform(const string& name, size_t& id, const Vec<2>& vec);
			void addUniform(const string& name, size_t& id, const Vec<3>& vec);
			void addUniform(const string& name, size_t& id, const Vec<4>& vec);			
			void addUniform(const string& name, size_t& id, const Mat<4>& mat);			
			void addUniform(const string& name, size_t& id, const float& f);
			void addUniform(const string& name, size_t& id, const Texture& texture);
	};

	class Shader : public ResourceHandle<Shader, ShaderData>
	{
		public:
			Shader();
		
			Shader(ShaderData& data);
	
			void bind();
	
			void update(const std::shared_ptr<Material> material);	
	};
	
	class ShaderData
	{
		public:		
			void bind();
			
			void update(const std::shared_ptr<Material> material);
		
		private:			
			GLuint program;
			unordered_map<size_t, GLint> uniforms;
			vector<GLuint> shaderStages;
			std::string m_fileName;
			friend class ShaderLoader;			
	};
	
	class ShaderLoader : public ResourceLoader<ShaderData>
	{
		public:
			ShaderLoader(const VertexSpecs& specs);
			virtual void load(const std::string& directory, const std::string& fileName, ShaderData& shader);
			
			virtual void unload(ShaderData& shader);
			
		private:
			void loadShaderStage(const std::string& name, const GLenum& shaderType, GLuint& program, ShaderData& shader);
			void parseStage(const std::string& name, ShaderData& shader, GLuint& program);
			bool parseLine(const std::string& line, const std::string& typeName, std::string& varName, std::string& type);
			vector<size_t> m_attributes;
			VertexSpecs m_specs;
	};
}