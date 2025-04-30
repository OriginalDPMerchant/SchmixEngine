#pragma once
#include <memory>
#include <string>
#include <functional>

namespace SchmixEngine
{
	class Object;

	struct Property
	{
		std::string PropertyName;
		std::function<Object* (Object*)> GetValue;
		std::function<void(Object*, const Object*)> SetValue;
	};
}