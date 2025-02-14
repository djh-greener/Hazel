#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {

	enum class FramebufferTextureFormat {
		None=0,

		//Color
		RGBA8,
		RED_INTEGER,
		//Depth/Stencil
		DEPTH24STENCIL8,

		//Defaults
		Depth = DEPTH24STENCIL8,
	};



	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) :
			TextureFormat(format) {};
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		//TODO: wrap/filtering
	};
	struct FramebufferAttachmentSpecification
	{
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			: Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	struct FramebufferSpecification
	{
		FramebufferAttachmentSpecification Attachments;
		float Width=0, Height=0;
		uint32_t Samples = 1;
		bool SwapChainTarget = false;
		bool IsCubeMap = false;
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		
		virtual void Bind(unsigned int target = 0x8D40) = 0;//0x8D40 == GL_FRAMEBUFFER
		virtual void Unbind(unsigned int target = 0x8D40) = 0;
		virtual void Resize(float width, float height) = 0;
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0;

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0;
		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0;

		virtual const FramebufferSpecification& GetSpecification() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);
	};


}
