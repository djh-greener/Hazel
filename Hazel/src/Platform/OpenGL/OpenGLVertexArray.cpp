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
	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer Has No Layout");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		int index = 0;
		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataType2OpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, vertexBuffer->GetLayout().GetStride(), (const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.emplace_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}