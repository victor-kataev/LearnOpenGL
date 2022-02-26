#pragma once

#include <stb_image.h>
#include <GL/glew.h>


#include <iostream>


namespace engine
{

	class Texture
	{
	public:
		Texture(const char* path, GLenum format);
		~Texture();
		void Bind() const;

	private:
		int m_width;
		int m_height;
		int m_channels;
		unsigned char* m_pictureData;

		GLuint m_id;
	};

}