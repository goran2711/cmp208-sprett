#pragma once
#include <type_traits>

template <typename T>
class UniquePtr
{
public:
	UniquePtr() = default;
	UniquePtr(T* data) : m_data(data) {}
	UniquePtr(UniquePtr<T>&& other) { *this = std::move(other); }
	~UniquePtr() { delete m_data; }

	// Disallow copying
	UniquePtr(const UniquePtr<T>&) = delete;
	UniquePtr<T>& operator=(const UniquePtr<T>&) = delete;

	UniquePtr<T>& operator=(T* data)
	{
		if (m_data)
			delete m_data;

		m_data = data;
		return *this;
	}
	UniquePtr<T>& operator=(UniquePtr<T>&& other)
	{
		if (this == &other)
			return *this;

		if (m_data)
			delete m_data;

		m_data = other.m_data;
		other.m_data = nullptr;
		return *this;
	}

	T* get() const { return m_data; }

	T& operator*() const { return *m_data; }
	T* operator->() const { return m_data; }

	bool operator==(T* data) const { return m_data == data; }
	bool operator!=(T* data) const { return !(*this == data); }

	explicit operator bool() const { return static_cast<bool>(m_data);; }

private:
	T* m_data = nullptr;
};