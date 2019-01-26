#include "Shader.h"
#include <fstream>
#include <sstream>

namespace Medusa
{
	//--- Uniform ---//
	Uniform::Uniform()
	{}
	
	UniformVec2::UniformVec2(const string& name, const Vec<2>& value):TypedUniform<Vec<2>>(name, value, "vec2")
	{}
	
	UniformVec3::UniformVec3(const string& name, const Vec<3>& value):TypedUniform<Vec<3>>(name, value, "vec3")
	{}
	
	UniformVec4::UniformVec4(const string& name, const Vec<4>& value):TypedUniform<Vec<4>>(name, value, "vec4")
	{}
	
	UniformMat4::UniformMat4(const string& name, const Mat<4>& value):TypedUniform<Mat<4>>(name, value, "mat4")
	{}
	
	UniformFloat::UniformFloat(const string& name, const float& value):TypedUniform<float>(name, value, "float")
	{}
	
	UniformTexture::UniformTexture(const string& name, const Texture& value):TypedUniform<Texture>(name, value, "sampler2D")
	{}
	
	void UniformVec2::update(const GLint& location)
	{
		glUniform2f(location, (GLfloat)m_value(0), (GLfloat)m_value(1));
	}
	
	void UniformVec3::update(const GLint& location)
	{
		glUniform3f(location, (GLfloat)m_value(0), (GLfloat)m_value(1), (GLfloat)m_value(2));
	}
	
	void UniformVec4::update(const GLint& location)
	{
		glUniform4f(location, (GLfloat)m_value(0), (GLfloat)m_value(1), (GLfloat)m_value(2), (GLfloat)m_value(3));
	}
	
	void UniformMat4::update(const GLint& location)
	{
		glUniformMatrix4fv(location, 1, GL_TRUE, (GLfloat*)(&m_value(0,0)) );
	}
	
	void UniformFloat::update(const GLint& location)
	{
		glUniform1f(location, (GLfloat)m_value);
	}
	
	void UniformTexture::update(const GLint& location)
	{
		//m_value.activate(location);
		//m_value.bind();
		m_value.read();
	}
	
	
	
	//--- Material ---//
	
	Material::Material():uniforms()
	{
		CIRCE_INFO("Initializing material.");
	}
			
	Material::~Material()
	{
		CIRCE_INFO("Destroying material.");
	}
	
	void  Material::uploadUniform(size_t id, const GLint& location) const
	{
		if(uniforms.find(id)!=uniforms.end())
		{	
			uniforms.at(id)->update(location);
		}
	}
	
	void Material::addUniform(const string& name, size_t& id, const Vec<2>& vec)
	{
		uniforms[id]=make_shared<UniformVec2>(UniformVec2(name, vec));
	}
	
	void Material::addUniform(const string& name, size_t& id, const Vec<3>& vec)
	{
		uniforms[id]=make_shared<UniformVec3>(UniformVec3(name, vec));
	}
	
	void Material::addUniform(const string& name, size_t& id, const Vec<4>& vec)
	{
		uniforms[id]=make_shared<UniformVec4>(UniformVec4(name, vec));
	}
	
	void Material::addUniform(const string& name, size_t& id, const Mat<4>& mat)
	{
		uniforms[id]=make_shared<UniformMat4>(UniformMat4(name, mat));
	}
	
	void Material::addUniform(const string& name, size_t& id, const float& f)
	{
		uniforms[id]=make_shared<UniformFloat>(UniformFloat(name, f));
	}
	
	void Material::addUniform(const string& name, size_t& id, const Texture& texture)
	{
		uniforms[id]=make_shared<UniformTexture>(UniformTexture(name, texture));
	}
	
	
	
	//--- Shader ---//
	
	Shader::Shader()
	{}
	
	Shader::Shader(ShaderData& data):ResourceHandle(data)
	{}
	
	void Shader::bind()
	{
		m_resource->bind();
	}
	
	void Shader::update(const std::shared_ptr<Material> material)
	{
		m_resource->update(material);
	}
	
	void ShaderData::bind()
	{
		glUseProgram(program);
	}
	
	void ShaderData::update(const std::shared_ptr<Material> material)
	{
		for(const std::pair<size_t, GLint> uniform : uniforms)
		{
			material->uploadUniform(uniform.first, uniform.second);
		}
	}
	
	
	
	//--- ShaderLoader ---//
	
	ShaderLoader::ShaderLoader(const VertexSpecs& specs):m_specs(specs)
	{}
	
	void ShaderLoader::load(const std::string& directory, const string& fileName, ShaderData& shader)
	{
		
		GLuint program = glCreateProgram();
		
		//TODO parse attributes in the file to add only whats needed
		VertexSpecsIterator it = m_specs.begin();
		while(it.hasNext())
		{
			glBindAttribLocation(program, it->location, it->name.c_str());
			it.next();
		}
		
		loadShaderStage(directory+fileName+".vs", GL_VERTEX_SHADER, program, shader);
		loadShaderStage(directory+fileName+".fs", GL_FRAGMENT_SHADER, program, shader);

		glLinkProgram(program);
		shader.program = program;
		
		
		parseStage(directory+fileName+".vs", shader, program);
		parseStage(directory+fileName+".fs", shader, program);
		
		shader.m_fileName = fileName;
		
		CIRCE_INFO("Shader "+fileName+" loaded");
	}
	
	void ShaderLoader::unload(ShaderData& shader)
	{
		for(GLuint shaderStage : shader.shaderStages)
		{
			glDetachShader(shader.program, shaderStage);
			glDeleteShader(shaderStage);
		}
		CIRCE_INFO("Shader "+shader.m_fileName+" unloaded");
	}
	
	void ShaderLoader::loadShaderStage(const std::string& name, const GLenum& shaderType, GLuint& program, ShaderData& shader)
	{
		
		ifstream file;
		file.open(name);
		std::string shaderString;
		std::string line;
		while(std::getline(file, line))
		{
			shaderString += "\n" +line;
		}
		
		const char* shader_code=shaderString.c_str();

		
		GLuint shaderStage = glCreateShader(shaderType);
		glShaderSource(shaderStage, 1, &shader_code, NULL);
		glCompileShader(shaderStage);
		
		GLint isCompiled = 0;
		glGetShaderiv(shaderStage, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderStage, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> errorLog(maxLength);
			glGetShaderInfoLog(shaderStage, maxLength, &maxLength, &errorLog[0]);
			
			std::string errorMsg;
			for(GLchar character : errorLog)
			{
				errorMsg+=character;
			}
			std::cout << "Error compiling shader " << name << std::endl;
			std::cout << errorMsg << std::endl;
			
			glDeleteShader(shaderStage); // Don't leak the shader.
			file.close();
			return;
		}
		glAttachShader(program, shaderStage);
		shader.shaderStages.push_back(shaderStage);
		
		file.close();
	}
	
	void ShaderLoader::parseStage(const std::string& name, ShaderData& shader, GLuint& program)
	{
		ifstream file;
		file.open(name);
		std::string line;
		std::string varName;
		std::string varType;
		while(std::getline(file, line))
		{
			if(parseLine(line, "uniform", varName, varType))
			{
				shader.uniforms.insert(std::pair<size_t, GLint>(Circe::getId(varName), glGetUniformLocation(program, varName.c_str())));
			}
		}
		file.close();	
	}
	
	bool ShaderLoader::parseLine(const std::string& line, const std::string& typeName, std::string& varName, std::string& varType)
	{
		if(line.find(typeName) == string::npos)
		{
			return false;
		}
		
		size_t start = line.find(typeName)+typeName.length()+1;
		size_t end = line.find(";");
		string subline = line.substr(start, end-start);
		
		size_t mid = subline.find(" ");
		
		varType = subline.substr(0, mid);
		varName = subline.substr(mid+1,subline.length()-(mid+1));

		return true;
	}
}
