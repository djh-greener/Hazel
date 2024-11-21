#pragma once
#include"Event.h"
#include<sstream>

namespace Hazel {
	class HAZEL_API KeyEvent :public Event
	{
	public:
		int GetKeyCode()const { return m_KeyCode; }

	protected:
		//donot implement, similar to a interface class
		KeyEvent(int KeyCode):m_KeyCode(KeyCode){}
		int m_KeyCode;
	};


	class HAZEL_API KeyPressEvent :public KeyEvent
	{
	public:
		KeyPressEvent(int keyCode, int RepeatCount) :
			KeyEvent(keyCode), m_RepeatCount(RepeatCount){}
		int GetKeyRepeatCount()const { return m_RepeatCount; }
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyPressEvent: " << m_KeyCode << "(" << m_RepeatCount << "repeated)";
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

}
