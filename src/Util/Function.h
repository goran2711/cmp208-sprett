#pragma once
#include <cassert>
#include <cstddef>		// For std::nullptr_t
#include <type_traits>	// For std::move()
#include "SharedPtr.h"

#ifdef _WIN32

#include <functional>
template <typename T>
using Function = std::function<T>;

#else

template<typename T>
class Function;

template <typename R, typename ... Args>
class Function<R(Args...)>
{
	using function = Function<R(Args...)>;
public:
	Function() = default;
	Function(std::nullptr_t) : Function() {}
	Function(const function& other) { *this = other; }
	Function(function&& other) { *this = std::move(other); }
	template <typename Func>
	Function(Func func) : m_callable(makeCallable(func)) {}

	function& operator=(const function& other)
	{
		m_callable = other.m_callable;
		return *this;
	}

	function& operator=(function&& other)
	{
		m_callable = other.m_callable;
		other.m_callable = nullptr;
		return *this;
	}

	template <typename Func>
	function& operator=(Func func)
	{
		m_callable = makeCallable(func);
		return *this;
	}

	R operator()(Args ... args) const
	{
		assert(("Uninitialized function object", m_callable));
		return (*m_callable)(args...);
	}

	explicit operator bool() { return static_cast<bool>(m_callable); }

private:
	struct Handler
	{
		virtual ~Handler() = default;
		virtual R operator()(Args ...) = 0;
	};

	template <typename Func>
	SharedPtr<Handler> makeCallable(Func f)
	{
		struct Impl : Handler
		{
			Impl(Func func) : m_func(func) {}
			R operator()(Args ... args) override
			{
				return m_func(args...);
			}

		private:
			Func m_func;
		};

		return SharedPtr<Handler>( new Impl(f) );
	}

	SharedPtr<Handler> m_callable;
};
#endif