#include"hzpch.h"
#include"OpenGLVertexArray.h"
#include<glad/glad.h>
namespace Hazel {
	static GLenum ShaderDataType2OpenGLBaseType(ShaderDataType type) {
		switch (type)
		{
		case ShaderDataType::Float:		return GL_FLOAT;
		case ShaderDataType::Float2:		return GL_FLOAT;
		case ShaderDataType::Float3:		return GL_FLOAT;
		case ShaderDataType::Float4:		return GL_FLOAT;
		case ShaderDataType::Mat3:		return GL_FLOAT;
		case ShaderDataType::Mat4:		return GL_FLOAT;
		case ShaderDataType::Int:			return GL_INT;		
		case ShaderDataType::Int2:			return GL_INT;		
		case ShaderDataType::Int3:			return GL_INT;		
		case ShaderDataType::Int4:			return GL_INT;		
		case ShaderDataType::Bool:		return GL_BOOL;	
		}
		HZ_CORE_ASSERT(false, "Error ShaderDataType");
		return 0;
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glCreateVertexArrays(1, &m_RendererID);
		
	}
	OpenGLVertexArray::~OpenGLVertexArray()
	{
		HZ_PROFILE_FUNCTION();

		glDeleteVertexArrays(1, &m_RendererID);
	}
	void OpenGLVertexArray::Bind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
	}
	void OpenGLVertexArray::UnBind() const
	{
		HZ_PROFILE_FUNCTION();

		glBindVertexArray(0);

	}
	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		HZ_PROFILE_FUNCTION();

		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "VertexBuffer Has No Layout");
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();
		for (const auto& element : vertexBuffer->GetLayout())
		{
			glEnableVertexAttribArray(m_VertexBufferIndex);
			glVertexAttribPointer(m_VertexBufferIndex, element.GetComponentCount(), ShaderDataType2OpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE, vertexBuffer->GetLayout().GetStride(), (const void*)element.Offset);
			m_VertexBufferIndex++;
		}
		m_VertexBuffers.emplace_back(vertexBuffer);
	}
	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{

		HZ_PROFILE_FUNCTION();

		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}

}