#pragma once
#include"Event.h"
#include "Hazel/Core/KeyCodes.h"
namespace Hazel {
	class  KeyEvent :public Event
	{
	public:
		KeyCode GetKeyCode()const { return m_KeyCode; }
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
	protected:
		//donot implement, similar to a interface class
		KeyEvent(const KeyCode KeyCode):m_KeyCode(KeyCode){}
		KeyCode m_KeyCode;
	};


	class   KeyPressedEvent :public KeyEvent
	{
	public:
		 KeyPressedEvent(const KeyCode keyCode, uint16_t RepeatCount) :
			KeyEvent(keyCode), m_RepeatCount(RepeatCount){}
		 uint16_t GetKeyRepeatCount()const { return m_RepeatCount; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << " KeyPressedEvent: " << m_KeyCode << "(" << m_RepeatCount << "repeated)";
			return ss.str();
		}
		EVENT_CLASS_TYPE(KeyPressed);
	private:
		uint16_t m_RepeatCount;
	};


	class  KeyReleasedEvent :public KeyEvent
	{
	public:
		KeyReleasedEvent(const KeyCode KeyCode) :KeyEvent(KeyCode){}
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
		KeyTypedEvent(const KeyCode keycode)
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
