#include "Shader.h"
#include <fstream>
#include <sstream>

namespace Medusa
{
	//--- Uniform ---//
	/*Uniform::Uniform():m_type("")
	{}*/
	
	Uniform::Uniform(const std::vector<float>& value, const std::string& type):m_value(value), m_type(type)
	{}
	
	Uniform::Uniform(const Uniform& uniform):m_value(uniform.m_value), m_type(uniform.m_type)
	{}
	
	void Uniform::upload(const GLint& location)//This is ugly but let's settle for that now
	{
		if(m_type=="vec2"){
			glUniform2fv(location, 1, &m_value[0]);
		}else if(m_type=="vec3"){
			glUniform3fv(location, 1, &m_value[0]);
		}else if(m_type=="vec4"){
			glUniform4fv(location, 1, &m_value[0]);
		}else if(m_type=="mat4"){
			glUniformMatrix4fv(location, 1, GL_TRUE, &m_value[0]);
		}else{
			CIRCE_ERROR(m_type+" not recognized.");
		}
	}
	
	void Uniform::set(const std::vector<float>& value)
	{
		m_value = value;
	}
	
	
	//--- Material ---//
	
	void Material::setUniform(const std::string& name, const std::vector<float>& value, const std::string& type)
	{
		std::size_t id = Circe::getId(name);
		if(m_uniforms.find(id) == m_uniforms.end())
		{
			m_uniforms.insert(std::pair<std::size_t, std::unique_ptr<Uniform>>(id,std::make_unique<Uniform>(value, type)));
		}else{
			m_uniforms[id]->set(value);
		}
	}
	
	void Material::setUniform(const std::string& name, const float& value)
	{
		setUniform(name, std::vector<float>({value}), "float");
	}
	
	void Material::setUniform(const std::string& name, const Vec<2>& value)
	{
		setUniform(name, std::vector<float>({value(0), value(1)}), "vec2");
	}
	
	void Material::setUniform(const std::string& name, const Vec<3>& value)
	{
		setUniform(name, std::vector<float>({value(0), value(1), value(2)}), "vec3");
	}
	
	void Material::setUniform(const std::string& name, const Mat<4>& value)
	{
		setUniform(name, std::vector<float>({value(0,0), value(0,1), value(0,2), value(0,3),
											 value(1,0), value(1,1), value(1,2), value(1,3),
											 value(2,0), value(2,1), value(2,2), value(2,3),
											 value(3,0), value(3,1), value(3,2), value(3,3)}), "mat4");
	}
	
	void Material::setTexture(const TextureType& type, const Texture& texture)
	{
		m_textures.insert(std::pair<int, Texture>((int)type, texture));
	}

	void Material::uploadUniform(const std::size_t& index, const GLint& location)
	{
		if(m_uniforms.find(index) != m_uniforms.end())
		{
			m_uniforms[index]->upload(location);
		}
	}
	
	void Material::uploadTexture(const int& index)
	{
		if(m_textures.find(index) != m_textures.end())
		{		
			m_textures[index].read();
		}
	  /*else
		{
			CIRCE_ERROR("Texture not found at slot "+index);
		}*/
	}
	
	//--- Shader ---//
	
	Shader::Shader()
	{}
	
	Shader::Shader(const ShaderData& data):ResourceHandle(data)
	{}
	
	
	//--- ShaderData ---//
	
	void ShaderData::bind()
	{
		glUseProgram(program);
	}
	
	ShaderData::ShaderData(const VertexSpecs& specs):m_specs(specs), hasBeenInit(false)
	{}
	
	void ShaderData::update(const std::shared_ptr<Material> material)
	{
		if(!hasBeenInit)
		{
			init();
		}
		
		for(const std::pair<size_t, GLint> uniform : uniforms)
		{
			material->uploadUniform(uniform.first, uniform.second);
		}
		for(const std::pair<int, GLint> sampler : samplers)
		{
			material->uploadTexture(sampler.first);
		}
	}
	
	void ShaderData::attachTexture(const int& location, const int& textureSlot)
	{
		samplers.insert(std::pair<int, GLint>(textureSlot, location));
	}
	
	void ShaderData::init()
	{	
		hasBeenInit = true;
		for(const std::pair<int, GLint> sampler : samplers)
		{
			glUniform1i(sampler.second, sampler.first);
		}
	}
	
	
	//--- ShaderLoader ---//
	
	void ShaderLoader::load(const std::string& directory, const string& fileName, ShaderData& shader)
	{
		
		GLuint program = glCreateProgram();
		
		//TODO parse attributes in the file to add only whats needed
		VertexSpecsIterator it = shader.m_specs.begin();
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
		glDeleteProgram(shader.program);
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
			
			glDeleteShader(shaderStage);
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
				if(varType == "sampler2D")
				{
					//if(samplerLocations.find(Circe::getId(varName)) != samplerLocations.end())
					if(TextureSlot(varName) != -1)
					{	
						//shader.samplers.insert(std::pair<int, GLint>(samplerLocations[Circe::getId(varName)], glGetUniformLocation(program, varName.c_str())));
						//shader.attachTexture(glGetUniformLocation(program, varName.c_str()), samplerLocations[Circe::getId(varName)]);
						shader.attachTexture(glGetUniformLocation(program, varName.c_str()), TextureSlot(varName));
					}
					else
					{
						CIRCE_ERROR("Texture " +varName+" not recognized.");
					}
				}
				else
				{
					shader.uniforms.insert(std::pair<size_t, GLint>(Circe::getId(varName), glGetUniformLocation(program, varName.c_str())));
				}
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
