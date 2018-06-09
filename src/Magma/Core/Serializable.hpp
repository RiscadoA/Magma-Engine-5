#pragma once

#include <istream>
#include <ostream>

namespace Magma
{
	/// <summary>
	///		Thrown when there is a serialization error
	/// </summary>
	class SerializationError : public std::runtime_error { public: using std::runtime_error::runtime_error; };

	/// <summary>
	///		Serialization modes (some serializable objects may not support all serialization modes)
	/// </summary>
	enum class SerializationMode
	{
		Invalid = -1,

		Binary,
		XML,

		Count
	};

	/// <summary>
	///		Abstract class for objects that can be serialized.
	///		All serializable objects should have empty constructors in order for them to be initialized from deserialization.
	/// </summary>
	class Serializable
	{
	public:
		Serializable() = default;
		virtual ~Serializable() = default;

		virtual void Serialize(std::ostream& os, SerializationMode mode) const = 0;
		virtual void Deserialize(std::istream& os, SerializationMode mode) = 0;
	};
}