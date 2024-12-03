#include"hzpch.h"
#include"OpenGLTexture.h"
#include<glad/glad.h>
#include<stb_image.h>
namespace Hazel {
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path,bool gamma):m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(1);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		HZ_CORE_ASSERT(data, "Failed To Load Image");
		m_Width = width;
		m_Height = height;


		GLenum internalFormat=0, dataFormat=0;
		if (channels == 1)
		{
			internalFormat = dataFormat = GL_R8;
		}
		else if (channels == 3)
		{
			internalFormat = gamma ? GL_SRGB8 : GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 4)
		{
			internalFormat = gamma ? GL_SRGB8_ALPHA8 : GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not Supported! ");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);


		stbi_image_free(data);

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_RendererID);
	}
}