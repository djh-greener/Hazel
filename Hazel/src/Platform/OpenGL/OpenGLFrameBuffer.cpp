#include "hzpch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Hazel {

	static const uint32_t s_MaxFramebufferSize = 8192;

	namespace Utils {
		//FramebufferTextureFormat--->GLTextureFormat
		static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat& format)
		{
			switch (format)
			{
			case FramebufferTextureFormat::RGBA8:									return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER:						return GL_RED_INTEGER;
			case FramebufferTextureFormat::DEPTH24STENCIL8:				return GL_DEPTH24_STENCIL8;
			}

			HZ_CORE_ASSERT(false);
			return 0;
		}

		static GLenum TextureTarget(FramebufferSpecification& spec )
		{
			if (spec.IsCubeMap)
				return GL_TEXTURE_CUBE_MAP;
			
			return spec.Samples > 1 ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void AttachColorTexture(uint32_t id, FramebufferSpecification& spec, GLenum internalFormat, GLenum format, int index)
		{
			//MSAA
			bool multisample = spec.Samples> 1;
			if (multisample)
			{
				
				glTexImage2DMultisample(TextureTarget(spec), spec.Samples, internalFormat, spec.Width, spec.Height, GL_TRUE);
			}
			else
			{
				glTexImage2D(TextureTarget(spec), 0, internalFormat, spec.Width, spec.Height, 0, format, GL_UNSIGNED_BYTE,nullptr);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(spec), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, FramebufferSpecification& spec, GLenum format, GLenum attachmentType)
		{
			bool multisampled = spec.Samples> 1;
			if (multisampled)
			{
				glTexImage2DMultisample(TextureTarget(spec), spec.Samples, format, spec.Width, spec.Height, GL_TRUE);
			}
			else
			{
				glTexStorage2D(TextureTarget(spec), 1, format, spec.Width, spec.Height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(spec), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat& format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:	return true;
			}
			return false;
		}
	}


	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec)
		: m_Specification(spec)
	{
		for (auto spec : m_Specification.Attachments.Attachments)
		{
			if (!Utils::IsDepthFormat(spec.TextureFormat))
				m_ColorAttachmentSpecifications.emplace_back(spec);
			else
				m_DepthAttachmentSpecification = spec;
		}
		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{

		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);

	}
	//ReCreate Framebuffer 
	//TODO: support GL_TEXTURE_CUBE_MAP
	void OpenGLFramebuffer::Invalidate()
	{
		if (m_RendererID)
		{
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures((int)m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);
			m_ColorAttachments.clear();
			m_DepthAttachment = 0;
		}
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		//Add Color
		if (m_ColorAttachmentSpecifications.size())
		{
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			glCreateTextures(Utils::TextureTarget(m_Specification), (int)m_ColorAttachments.size(), m_ColorAttachments.data());
			for (int i = 0; i < m_ColorAttachments.size(); ++i)
			{
				FramebufferTextureFormat TextureFormat = m_ColorAttachmentSpecifications[i].TextureFormat;
				glBindTexture(Utils::TextureTarget(m_Specification), m_ColorAttachments[i]);

				switch (TextureFormat)
				{
				case Hazel::FramebufferTextureFormat::RGBA8:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification, GL_RGBA8,GL_RGBA, i);
					break;
				case Hazel::FramebufferTextureFormat::RED_INTEGER:
					Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification, GL_R32I, GL_RED_INTEGER, i);
					break;
				}
				glBindTexture(Utils::TextureTarget(m_Specification), 0);
			}
		}
		//Add Depth
		if (m_DepthAttachmentSpecification.TextureFormat!=FramebufferTextureFormat::None)
		{
			glCreateTextures(Utils::TextureTarget(m_Specification), 1, &m_DepthAttachment);
			glBindTexture(Utils::TextureTarget(m_Specification), m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat)
			{
			case FramebufferTextureFormat::DEPTH24STENCIL8:
				Utils::AttachDepthTexture(m_DepthAttachment, m_Specification, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT);
				break;
			}
			glBindTexture(Utils::TextureTarget(m_Specification), 0);
		}

		if (m_ColorAttachments.size() > 1)
		{
			HZ_CORE_ASSERT(m_ColorAttachments.size() <= 4);
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers((int)m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty())
		{
			// Only depth-pass
			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);
		}

		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind(unsigned int target)
	{
		glBindFramebuffer(target, m_RendererID);
		
		glViewport(0, 0, (uint32_t)m_Specification.Width, (uint32_t)m_Specification.Height);
	}

	void OpenGLFramebuffer::Unbind(unsigned int target)
	{
		glBindFramebuffer(target, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize)
		{
			HZ_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}
		m_Specification.Width = width;
		m_Specification.Height= height;
		Invalidate();
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		HZ_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);
		return pixelData;

	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		HZ_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size());

		auto& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
			Utils::HazelFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}
}
