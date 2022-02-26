#include "Shader.h"



namespace engine
{
	//-----------public----------

	Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
	{
		std::string vertexContent;
		std::string fragmentContent;

		readContent(vertexPath, fragmentPath, vertexContent, fragmentContent);

		const char* vertexCode = vertexContent.c_str();
		const char* fragmentCode = fragmentContent.c_str();

		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexCode, NULL);
		glCompileShader(vertexShader);
		checkCompileErrors(vertexShader, CompileType::VERTEX);

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentCode, NULL);
		glCompileShader(fragmentShader);
		checkCompileErrors(fragmentShader, CompileType::FRAGMENT);

		m_program = glCreateProgram();
		glAttachShader(m_program, vertexShader);
		glAttachShader(m_program, fragmentShader);
		glLinkProgram(m_program);
		checkCompileErrors(m_program, CompileType::PROGRAM);

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	void Shader::Use() const
	{
		glUseProgram(m_program);
	}

	void Shader::SetProjection(glm::mat4& proj)
	{
		setMat4("projection", proj);
	}

	void Shader::SetView(glm::mat4& view)
	{
		setMat4("view", view);
	}

	void Shader::setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), (int)value);
	}

	void Shader::setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_program, name.c_str()), value);
	}

	void Shader::setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_program, name.c_str()), value);
	}

	void Shader::setMat4(const std::string& name, glm::mat4& value) const
	{
		int loc = glGetUniformLocation(m_program, name.c_str());
		glUniformMatrix4fv(loc, 1, false, &value[0][0]);
	}

	void Shader::setVec3(const std::string& name, const glm::vec3& value) const
	{
		int loc = glGetUniformLocation(m_program, name.c_str());
		glUniform3f(loc, value.r, value.g, value.b);
	}


	//------------private---------

	void Shader::readContent(const std::string& vertexPath, const std::string& fragmentPath, std::string& vertexContent, std::string& fragmentContent) const
	{
		std::ifstream vertexFile;
		std::ifstream fragmentFile;

		vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
		fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

		try
		{
			vertexFile.open(SHADERS_DIR + vertexPath);
			fragmentFile.open(SHADERS_DIR + fragmentPath);
			std::stringstream ssVertex;
			std::stringstream ssFragment;

			ssVertex << vertexFile.rdbuf();
			ssFragment << fragmentFile.rdbuf();

			vertexContent = ssVertex.str();
			fragmentContent = ssFragment.str();

			vertexFile.close();
			fragmentFile.close();
		}
		catch (std::ifstream::failure& e)
		{
			std::cerr << "ERROR::SHADER::COULD_NOT_READ_SHADER\n";
		}
	}
	

	void Shader::checkCompileErrors(GLuint shader, CompileType type) const
	{
		int success;
		char infoLog[1024];
		if (type == CompileType::VERTEX)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: VERTEX" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else if (type == CompileType::FRAGMENT)
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: FRAGMENT" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cerr << "ERROR::PROGRAM_LINKING_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}


	
}