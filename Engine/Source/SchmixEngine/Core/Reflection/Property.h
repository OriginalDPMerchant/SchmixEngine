#pragma once
#include <memory>
#include <string>
#include <functional>

namespace SchmixEngine
{
	class Data;

	struct Property
	{
		std::string PropertyName;
		std::function<Data(const Data&)> GetValue;
		std::function<void(Data&, const Data&)> SetValue;
	};
}