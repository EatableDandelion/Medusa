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
#include <variant>

namespace Medusa
{	
	using namespace Circe;
	using namespace std;
	
	struct Uniform;
	struct Material;
	class Shader;
	class ShaderData;
	class ShaderLoader;
	
	struct Uniform
	{
		public:
			//Uniform();
			Uniform(const std::vector<float>& value, const std::string& type);
			Uniform(const Uniform& uniform);
			
			void upload(const GLint& location);
			void set(const std::vector<float>& value);
			
		private:
			std::vector<float> m_value;
			std::string m_type;
	};
	
	struct Material
	{
		public:
			void setUniform(const std::string& name, const std::vector<float>& value, const std::string& type);
		
			void setUniform(const std::string& name, const float& value);
			void setUniform(const std::string& name, const Vec<2>& value);
			void setUniform(const std::string& name, const Vec<3>& value);
			void setUniform(const std::string& name, const Mat<4>& value);
			
			void setTexture(const TextureType& type, const Texture& texture);
			
			void uploadUniform(const std::size_t& index, const GLint& location);
			void uploadTexture(const int& slot);
			
			
		private:
			std::unordered_map<std::size_t, std::unique_ptr<Uniform>> m_uniforms;
			std::unordered_map<int, Texture> m_textures;
	};

	class Shader : public ResourceHandle<Shader, ShaderData>
	{
		public:
			Shader();
		
			Shader(const ShaderData& data);
			
	};
	
	
	class ShaderData
	{
		public:
			ShaderData(const VertexSpecs& specs);
		
			void bind();
			
			void update(const std::shared_ptr<Material> material);
			
			void attachTexture(const int& location, const int& textureSlot);
		
		private:			
			GLuint program;
			unordered_map<size_t, GLint> uniforms;
			unordered_map<int, GLint> samplers;
			vector<GLuint> shaderStages;
			std::string m_fileName;
			VertexSpecs m_specs;
			friend class ShaderLoader;	
			bool hasBeenInit;
			
			void init();
	};
	
	class ShaderLoader : public ResourceLoader<ShaderData>
	{
		public:
			virtual void load(const std::string& directory, const std::string& fileName, ShaderData& shader);
			
			virtual void unload(ShaderData& shader);
			
		private:
			void loadShaderStage(const std::string& name, const GLenum& shaderType, GLuint& program, ShaderData& shader);
			void parseStage(const std::string& name, ShaderData& shader, GLuint& program);
			bool parseLine(const std::string& line, const std::string& typeName, std::string& varName, std::string& type);
			vector<size_t> m_attributes;			
	};
}