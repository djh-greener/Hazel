#pragma once

#include "Hazel/Debug/Instrumentor.h"
#include"Hazel/Core/Core.h"
#include <functional>

namespace Hazel {
	enum class EventType {
		None=0,
		WindowClose,WindowResize,WindowFocus,WindowLostFocus,WindowMoved,
		AppTick,AppUpdate,AppRender,
		KeyPressed,KeyReleased,KeyTyped,
		MouseButtonPressed,MouseButtonReleased,MouseMoved,MouseScrolled
	};
	enum EventCategory {
		None = 0,
		EventCategoryApplication =			BIT(0),
		EventCategoryInput =						BIT(1),
		EventCategoryKeyboard =				BIT(2),
		EventCategoryMouse=					BIT(3),
		EventCategoryMouseButton =		BIT(4),

	};
	//if type is MouseEvent,then  
	// EventType::##type = EventType::MouseEvent
	//return #type = return "MouseEvent"
#define EVENT_CLASS_TYPE(type) \
	static EventType GetStaticType(){return EventType::type;}\
	virtual EventType GetEventType()const {return GetStaticType();}\
	virtual const std::string GetName() const override{return #type;}

#define EVENT_CLASS_CATEGORY(category)\
	 virtual int GetCategoryFlags() const override { return category; }

	class  Event 
	{
	public:
		virtual ~Event() = default;
		virtual EventType GetEventType() const = 0 ;
		virtual const std::string GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const{ return GetName(); }
		inline bool IsInCategory(EventCategory Category) {
			return GetCategoryFlags() & Category;
		}
		bool Handled = false;

	};

	class  EventDispatcher {
	public:
		EventDispatcher(Event& event) :m_event(event){}

		//Dispatch base event to T type event，then pass to func and execute
		template<typename T, typename F>
			bool Dispatch(const F& func)
		{
				if (m_event.GetEventType() == T::GetStaticType())
				{
					//faster than dynamic_cast<T>(m_event)?
					m_event.Handled |= func(static_cast<T&>(m_event));
					return true;
				}
				return false;
		}


	private:
		Event& m_event;
	};
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}
