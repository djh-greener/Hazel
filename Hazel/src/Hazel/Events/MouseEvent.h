#pragma once
#include"Event.h"
#include<sstream>

namespace Hazel {
	class HAZEL_API MouseMoveEvent :public Event
	{
	public:
		MouseMoveEvent(float MouseX,float MouseY):m_MouseX(MouseX),m_MouseY(MouseY){}
		float GetX()const { return m_MouseX; }
		float GetY()const { return m_MouseY; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MosueMoveEvent: " << m_MouseX << ","<<m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved);

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class HAZEL_API MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(float OffsetX, float OffsetY) :
			m_OffsetX(OffsetX), m_OffsetY(OffsetY){}

		float GetOffsetX()const { return m_OffsetX; }
		float GetOffsetY()const { return m_OffsetY; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << m_OffsetX << "," << m_OffsetY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled);
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_OffsetX, m_OffsetY;
	};

	class HAZEL_API MouseButtonEvent : public Event
	{
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	protected:
		MouseButtonEvent(int button)
			: m_Button(button) {}

		int m_Button;
	};

	class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}