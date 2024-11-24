#pragma once
#include"Event.h"

namespace Hazel {
	class HAZEL_API KeyEvent :public Event
	{
	public:
		int GetKeyCode()const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		//donot implement, similar to a interface class
		KeyEvent(int KeyCode):m_KeyCode(KeyCode){}
		int m_KeyCode;
	};


	class HAZEL_API  KeyPressedEvent :public KeyEvent
	{
	public:
		 KeyPressedEvent(int keyCode, int RepeatCount) :
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


	class HAZEL_API KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(int KeyCode) :KeyEvent(KeyCode){}
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyReleased)
	};


	class HAZEL_API KeyTypedEvent : public KeyEvent
	{
	public:
		KeyTypedEvent(int keycode)
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
