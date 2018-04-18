#pragma once

#ifdef _WIN32

#include <memory>
template <typename T>
using SharedPtr = std::shared_ptr<T>;

#else

template <typename T>
class SharedPtr
{
public:
	SharedPtr() = default;
	SharedPtr(T* data) : m_data(data), m_refCounter(new RefCounter()) { m_refCounter->acquire(); }
	SharedPtr(const SharedPtr<T>& other)
	{
		*this = other;
	}
	SharedPtr<T>& operator=(const SharedPtr<T>& other)
	{
		if (m_data == other.m_data)
			return *this;

		release();

		m_data = other.m_data;
		m_refCounter = other.m_refCounter;

		// Assume if it doesn't have a refCounter there
		// is no data to count references to.
		if (m_refCounter)
			m_refCounter->acquire();		

		return *this;
	}
	SharedPtr<T>& operator=(T* data)
	{
		release();

		m_refCounter = new RefCounter();

		m_data = data;
		m_refCounter->acquire();
		return *this;
	}
	SharedPtr(SharedPtr<T>&& other) { *this = std::move(other); }
	SharedPtr<T>& operator=(SharedPtr<T>&& other)
	{
		if (m_data == other.m_data)
			return *this;

		release();

		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;

		m_data = other.m_data;
		other.m_data = nullptr;
		return *this;
	}
	~SharedPtr() { release(); }

	T* get() const { return m_data; }

	T& operator*() const { return *m_data; }
	T* operator->() const { return m_data; }

	bool operator==(T* data) const { return m_data == data; }
	bool operator!=(T* data) const { return !(*this == data); }

	explicit operator bool() const { return static_cast<bool>(m_data); }

private:
	void release()
	{
		if (!m_refCounter)
			return;
		
		if (m_refCounter->release() == 0)
		{
			delete m_refCounter;
			m_refCounter = nullptr;

			delete m_data;
			m_data = nullptr;
		}		
	}

	struct RefCounter
	{
		unsigned int count = 0;
		unsigned int acquire() { return ++count; }
		unsigned int release() { return --count; }
	};

	RefCounter* m_refCounter = nullptr;
	T* m_data = nullptr;
};

#endif

template <typename T, typename ... Args>
SharedPtr<T> makeShared(Args&& ... args)
{
#ifdef _WIN32
	return std::make_shared<T>(std::forward<Args>(args)...);
#else
	return SharedPtr<T>(new T(std::forward<Args>(args)...));
#endif
}