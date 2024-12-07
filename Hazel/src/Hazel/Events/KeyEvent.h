#pragma once
#include"Event.h"
#include "Hazel/Core/Input.h"
namespace Hazel {
	class  KeyEvent :public Event
	{
	public:
		KeyCode GetKeyCode()const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		//donot implement, similar to a interface class
		KeyEvent(KeyCode KeyCode):m_KeyCode(KeyCode){}
		KeyCode m_KeyCode;
	};


	class   KeyPressedEvent :public KeyEvent
	{
	public:
		 KeyPressedEvent(KeyCode keyCode, int RepeatCount) :
			KeyEvent(keyCode), m_RepeatCount(RepeatCount){}
		int GetKeyRepeatCount()const { return m_RepeatCount; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << " KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << "repeated)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed);
	private:
		int m_RepeatCount;
	};


	class  KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(KeyCode KeyCode) :KeyEvent(KeyCode){}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};


	class  KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode)
			: KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped)
	};
}
