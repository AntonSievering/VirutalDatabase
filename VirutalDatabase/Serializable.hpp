#pragma once

template <class T>
struct Serializable
{
	union
	{
		T object;
		char data[sizeof(T)];
	};

public:
	Serializable() noexcept = default;

	Serializable(const T &obj) noexcept
	{
		object = obj;
	}

public:
	size_t size() const noexcept
	{
		return sizeof(T);
	}
};
