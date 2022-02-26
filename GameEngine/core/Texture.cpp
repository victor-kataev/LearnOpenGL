#include "Texture.h"

namespace engine
{
	//---------public----------

	Texture::Texture(const char* path, GLenum format)
	{
		stbi_set_flip_vertically_on_load(true); //to flip the image upside-down
		m_pictureData = stbi_load(path, &m_width, &m_height, &m_channels, 0);
		if (!m_pictureData)
			std::cerr << "Failed to load picture: " << path << std::endl;

		glGenTextures(1, &m_id);
		glBindTexture(GL_TEXTURE_2D, m_id);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_pictureData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	Texture::~Texture()
	{
		stbi_image_free(m_pictureData);
	}

	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

}