#pragma once
#include"Event.h"
#include "Hazel/Core/MouseCodes.h"
namespace Hazel {
	class  MouseMovedEvent :public Event
	{
	public:
		MouseMovedEvent(const float MouseX,const float MouseY):m_MouseX(MouseX),m_MouseY(MouseY){}
		float GetX()const { return m_MouseX; }
		float GetY()const { return m_MouseY; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ","<<m_MouseY;
			return ss.str();
		}
		EVENT_CLASS_TYPE(MouseMoved);

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	private:
		float m_MouseX, m_MouseY;
	};

	class  MouseScrolledEvent :public Event
	{
	public:
		MouseScrolledEvent(const float OffsetX, const float OffsetY) :
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

	class  MouseButtonEvent : public Event
	{
	public:
		MouseCode GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput | EventCategoryMouseButton)
	protected:
		MouseButtonEvent(const MouseCode button)
			: m_Button(button) {}

		MouseCode m_Button;
	};

	class  MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(const MouseCode button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class  MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(const MouseCode button)
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
