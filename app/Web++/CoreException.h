#ifndef COREEXCEPTION_H
#define COREEXCEPTION_H

#include <exception>
#include "messages.hpp"

namespace wpp
{
	enum class Error : unsigned int
	{
		DocumentUriNotSet = 0x100
	};

	class CoreException : public std::exception
	{
	public:
		CoreException();
		CoreException(const Error& err);

		const Error& getErrorNumber();
	protected:
		Error err;
	private:
	};
};

#endif