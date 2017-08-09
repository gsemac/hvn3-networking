#pragma once
#include <stdexcept>
#include <string>

namespace hvn3 {
	namespace System {

		class Exception : public std::runtime_error {

		public:
			Exception();
			Exception(const char* message);
			Exception(const std::string& message);

			virtual const char* what() const throw();
			virtual std::string Message() const;

		};

	}
}