#pragma once
#include "SaveLoad.hpp"
#include <array>
#include <cstring>
#include <optional>

namespace Armed
{

class SaveStream final
{
public:
	explicit SaveStream( SaveLoadBuffer& out_buffer)
		: buffer_(out_buffer)
	{}

public:
	template<typename T, size_t N>
	void Write(const std::array<T, N>& a);

	template<typename T, size_t N>
	void Write(const T (&a)[N]);

	template<typename T>
	void Write(const std::optional<T>& o);

	template<typename T>
	void Write(const T& t);

private:
	SaveLoadBuffer& buffer_;
};

class LoadStream final
{
public:
	LoadStream(const SaveLoadBuffer& in_buffer, const size_t buffer_pos)
		: buffer_(in_buffer), buffer_pos_(buffer_pos)
	{}

	size_t GetBufferPos() const { return buffer_pos_; }

public:
	template<typename T, size_t N>
	void Read(std::array<T, N>& a);

	template<typename T, size_t N>
	void Read(T (&a)[N]);

	template<typename T>
	void Read(std::optional<T>& o);

	template<class T>
	void Read(T& t);

private:
	const SaveLoadBuffer& buffer_;
	size_t buffer_pos_;
};

template<typename T, size_t N>
void SaveStream::Write(const std::array<T, N>& a)
{
	for(const auto& el : a)
		Write(el);
}

template<typename T, size_t N>
void SaveStream::Write(const T (&a)[N])
{
	for(const auto& el : a)
		Write(el);
}

template<typename T>
void SaveStream::Write(const std::optional<T>& o)
{
	Write(o != std::nullopt);
	if(o != std::nullopt)
		Write(*o);
}

template<class T>
void SaveStream::Write(const T& t)
{
	static_assert(
		std::is_integral<T>::value || std::is_floating_point<T>::value,
		"Expected basic types");

	const size_t pos= buffer_.size();
	buffer_.resize( pos + sizeof(T) );
	std::memcpy(
		buffer_.data() + pos,
		&t,
		sizeof(T));
}

template<typename T, size_t N>
void LoadStream::Read(std::array<T, N>& a)
{
	for(T& t : a)
		Read(t);
}

template<typename T, size_t N>
void LoadStream::Read(T (&a)[N])
{
	for(T& t : a)
		Read(t);
}

template<typename T>
void LoadStream::Read(std::optional<T>& o)
{
	bool has_value= false;
	Read(has_value);
	if(has_value)
	{
		T value;
		Read(value);
		o= value;
	}
	else
		o= std::nullopt;
}

template<class T>
void LoadStream::Read(T& t)
{
	static_assert(
		std::is_integral<T>::value || std::is_floating_point<T>::value,
		"Expected basic types" );

	ARMED_ASSERT(buffer_pos_ + sizeof(T) <= buffer_.size());

	std::memcpy(
		&t,
		buffer_.data() + buffer_pos_,
		sizeof(T));

	buffer_pos_+= sizeof(T);
}


} // namespace Armed
