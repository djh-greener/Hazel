#include"hzpch.h"
#include"OpenGLTexture.h"
#include<glad/glad.h>
#include<stb_image.h>
namespace Hazel {
	OpenGLTexture2D::OpenGLTexture2D(uint32_t width, uint32_t height):
		m_Width(width),m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;
		m_DataFormat = GL_RGBA;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

	}
	OpenGLTexture2D::OpenGLTexture2D(const std::string& path,bool gamma,bool flip):m_Path(path)
	{
		int width, height, channels;
		stbi_set_flip_vertically_on_load(flip);
		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data)
		{
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;


			GLenum internalFormat = 0, dataFormat = 0;
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
			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, width, height);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTextureSubImage2D(m_RendererID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);


			stbi_image_free(data);
		}
		else
			HZ_CORE_ERROR("Cannot Load a Texture from {0}", path);

	}
	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(1, &m_RendererID);
	}
	void OpenGLTexture2D::Bind(uint32_t slot)
	{
		glBindTextureUnit(slot, m_RendererID);
	}
	void OpenGLTexture2D::UnBind(uint32_t slot)
	{
		glBindTextureUnit(slot, 0);
	}
	void OpenGLTexture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}
}
