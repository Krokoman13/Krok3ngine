#pragma once
#include <exception>

namespace Exception {
	//Should not break the flow of the program too much
	class KROK3NGINE_API Error : public std::exception {
	public:
		Error(const char* a_errorMessage) : m_errorMessage(a_errorMessage) {}

	public:
		virtual const char* what() const throw() {
			return m_errorMessage;
		}

	private:
		const char* m_errorMessage;
	};

	//Should break the flow of the program
	class KROK3NGINE_API Warning : public Error {
	public:
		Warning(const char* a_warningMessage, unsigned int const a_severity = 0) : Error(a_warningMessage), m_severity(a_severity) {}

	public:
		unsigned int Severity() { return m_severity; }

	private:
		unsigned int const m_severity;
	};

	class KROK3NGINE_API DoubleCall : public Warning {
	public:
		DoubleCall(const char* a_warningMessage) :
			Warning((std::string(
					"Function has been called before, should not be called again, info: \n\t") 
					+ a_warningMessage).c_str()
				, 2) {}

		DoubleCall() : Warning("Function has been called before, should not be called again", 2) {}
	};

	class KROK3NGINE_API OrderMismatch : public Error {
	public:
		OrderMismatch(const char* a_errorMessage = nullptr) :
			Error((std::string("OrderMismatch: ") + a_errorMessage).c_str()) {}
	};
}