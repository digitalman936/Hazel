#pragma once

#include "hzpch.h"
#include "Hazel/Core.h"

namespace Hazel {

	// Events in Hazel are currently blocking, meaning when an event occurs it
	// immediately gets dispatched and must be dealt with right then an there.
	// For the future, a better strategy might be to buffer events in an event
	// bus and process them during the "event" part of the update stage.

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication   = BIT(0),
		EventCategoryInput         = BIT(1),
		EventCategoryKeyboard      = BIT(2),
		EventCategoryMouse         = BIT(3),
		EventCategoryMouseButton   = BIT(4)
	};

// Declares common pure virtual functions for derived classes
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }
#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class HAZEL_API Event
	{
	public:
		bool Handled = false;

		// Returns the type of event
		virtual EventType GetEventType() const = 0;

		// Returns the name of the event as a string
		virtual const char* GetName() const = 0;

		// Can be overridden as a custom log message
		virtual std::string ToString() const { return GetName(); }

		// Returns an integer (using bit flag system) that represents the category 
		// EXAMPLE - EventCategoryApplication represented by 0001 (in binary)
		virtual int GetCategoryFlags() const = 0;

		// Checks the event using bitwise AND operation which compares
		// the integer obtained by GetCategoryFlags (unique bit flag) to categories in EventCategory.
		// Returns true if the result is non-zero
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}
	};

	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event& event)
			: m_Event(event) {}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				// Casts m_Event to type T and returns a valid pointer if m_Event is of type T.
				// Otherwise casted_event returns a nullptr
				T* casted_event = dynamic_cast<T*>(&m_Event);

				if (casted_event)
				{
					m_Event.Handled = func(*casted_event);
					return true;
				}
			}
			
			return false;
		}

	private:
		Event& m_Event;
	};

	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}