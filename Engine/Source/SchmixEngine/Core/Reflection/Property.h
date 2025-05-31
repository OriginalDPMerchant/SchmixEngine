#pragma once
#include <memory>
#include <string>
#include <functional>

namespace SchmixEngine
{
	class Value;

	struct Property
	{
		std::string PropertyName;
		std::function<Value(const Value&)> GetValue;
		std::function<void(Value&, const Value&)> SetValue;
	};
}