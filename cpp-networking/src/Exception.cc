#include "Exception.h"

namespace hvn3 {
	namespace System {

		Exception::Exception() : std::runtime_error("") {}
		Exception::Exception(const char* message) : std::runtime_error(message) {}
		Exception::Exception(const std::string& message) : std::runtime_error(message.c_str()) {}
		const char* Exception::what() const throw() {

			return std::runtime_error::what();

		}
		std::string Exception::Message() const {

			return std::string(what());

		}

	}
}