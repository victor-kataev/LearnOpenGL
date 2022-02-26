#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


#include <GL/glew.h>
#include <glm.hpp>


#define SHADERS_DIR "resources/shaders/"



namespace engine
{
	enum class CompileType
	{
		PROGRAM,
		VERTEX,
		FRAGMENT
	};

	
	class Shader
	{
	public:
		Shader(const std::string& vertex_path, const std::string& fragment_path);
		void Use() const;
		void SetProjection(glm::mat4& proj);
		void SetView(glm::mat4& view);
		void setBool(const std::string& name, bool value) const;
		void setInt(const std::string& name, int value) const;
		void setFloat(const std::string& name, float value) const;
		void setMat4(const std::string& name, glm::mat4& value) const;
		void setVec3(const std::string& name, const glm::vec3& value) const;

	private:
		GLuint m_program;

		void readContent(const std::string& vertexPath, const std::string& fragmentPath, std::string& vertexContent, std::string& fragmentContent) const;
		void checkCompileErrors(GLuint shader, CompileType type) const;
	};

}