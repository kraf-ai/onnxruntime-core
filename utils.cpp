#include "utils.h"

std::string OrtUtils::ToProviderString(ExecutionProvider provider) {
	switch (provider)
	{
	case OrtUtils::ExecutionProvider::CPU:
		return "CPU";
	case OrtUtils::ExecutionProvider::DirectML:
		return "DirectML";
	default:
		THROW_RUNTIME_ERROR("The execution provider is not supported.");
	}
}

#ifdef _WIN64
void ThrowIfFailed(HRESULT result) {
	if (FAILED(result)) {
		std::ostringstream oss;
		oss << "Function failed with HRESULT: " << std::hex << result << "\n";
		oss << GetStackTrace();

		std::string logMessage = oss.str();
		THROW_RUNTIME_ERROR(logMessage);
	}
}

std::string GetStackTrace() {
	const int maxFrames = 62; // Maximum number of stack frames to capture
	void* stack[maxFrames];
	USHORT frames = CaptureStackBackTrace(0, maxFrames, stack, nullptr);

	std::ostringstream oss;
	oss << "Stack trace:\n";

	HANDLE process = GetCurrentProcess();
	SymInitialize(process, nullptr, TRUE);

	for (USHORT i = 0; i < frames; ++i) {
		DWORD64 address = (DWORD64)(stack[i]);

		char symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
		SYMBOL_INFO* symbol = reinterpret_cast<SYMBOL_INFO*>(symbolBuffer);
		symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
		symbol->MaxNameLen = MAX_SYM_NAME;

		if (SymFromAddr(process, address, nullptr, symbol)) {
			oss << "  - " << symbol->Name << " at 0x" << std::hex << symbol->Address << "\n";
		}
		else {
			oss << "  - [unknown symbol] at 0x" << std::hex << address << "\n";
		}
	}

	SymCleanup(process);
	return oss.str();
}
#endif