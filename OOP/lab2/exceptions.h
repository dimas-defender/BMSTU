#pragma once

#include <exception>
#include <string>

namespace flexlist {
class ListException : public std::exception {
 public:
	explicit ListException(
						   const std::string& filename,
						   const std::string& classname,
						   const int line,
						   const char *time,
						   const std::string& info) {
		msg = "In: "			+ filename			   +
				"\ninside: "	+ classname			   +
				"\nat line: "	+ std::to_string(line) +
				"\nat: "		+ time				   +
				"\noccured: "	+ info;
	}

	const char *what() const noexcept override {
		return msg.c_str();
	}
 private:
	std::string msg;
};

class MemoryError : public ListException {
 public:
	MemoryError(
				const std::string& filename,
				const std::string& classname,
				const int line,
				const char *time,
				const std::string& info = "Memory error")
	: ListException(filename, classname, line, time, info) { }
};

class InvalidListStream : public ListException {
 public:
	InvalidListStream(
					const std::string& filename,
					const std::string& classname,
					const int line,
					const char *time,
					const std::string& info = "Reading from stream failed")
	: ListException(filename, classname, line, time, info) { }
};

class ListOutOfBounds : public ListException {
 public:
	ListOutOfBounds(
					const std::string& filename,
					const std::string& classname,
					const int line,
					const char *time,
					const std::string& info = "Out of bounds access")
	: ListException(filename, classname, line, time, info) { }
};

class EmptyList : public ListException {
 public:
	EmptyList(
			  const std::string& filename,
			  const std::string& classname,
			  const int line,
			  const char *time,
			  const std::string& info = "Empty list access")
	: ListException(filename, classname, line, time, info) { }
};

class ConstructorBadArguments : public ListException {
 public:
	ConstructorBadArguments(
							const std::string& filename,
							const std::string& classname,
							const int line,
							const char *time,
							const std::string& info = "Bad arguments received")
	: ListException(filename, classname, line, time, info) { }
};
}  // namespace flexlist