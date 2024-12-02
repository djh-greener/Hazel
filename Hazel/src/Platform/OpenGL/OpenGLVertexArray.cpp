#include"hzpch.h"
#include"OpenGLVertexArray.h"
#include<glad/glad.h>
namespace Hazel {
	static GLenum ShaderDataType2OpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case Hazel::ShaderDataType::Float:			return GL_FLOAT;			break;
		case Hazel::ShaderDataType::Float2:		return GL_FLOAT;			break;
		case Hazel::ShaderDataType::Float3:		return GL_FLOAT;			break;
		case Hazel::ShaderDataType::Float4:		return GL_FLOAT;			break;
		case Hazel::ShaderDataType::Mat3:		return GL_FLOAT;			break;
		case Hazel::ShaderDataType::Mat4:		return GL_FLOAT;			break;
		case Hazel::ShaderDataType::Int:			return GL_INT;					break;
		case Hazel::ShaderDataType::Int2:			return GL_INT;					break;
		case Hazel::ShaderDataType::Int3:			return GL_INT;					break;
		case Hazel::ShaderDataType::Int4:			return GL_INT;					break;
		case Hazel::ShaderDataType::Bool:			return GL_BOOL;				break;
		}
		HZ_CORE_ASSERT(false, "Error ShaderDataType");
		return 0;
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
		
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		glBindVertexArray(0);

	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer Has No Layout");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(), ShaderDataType2OpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, vertexBuffer->GetLayout().GetStride(), (const void*)(intptr_t)element.Offset);
			m_VertexBufferIndex++;
		}
		m_VertexBuffers.emplace_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}