#pragma once

#include <windows.h>
#include <dbghelp.h>
#include <sstream>

#include "common.h"

#pragma comment(lib, "Dbghelp.lib") // Link the DbgHelp library

namespace OrtUtils {
	/**
	 * @brief This enum contains the different execution providers from ONNX Runtime that are currently supported. 
	 */
	enum class ExecutionProvider {
		CPU,			// Default CPU execution provider
		DirectML		// DirectML for the GPUs on Windows
	};

	/*
	 * @brief Maps ExecutionProvider enum to the ONNX Runtime provider strings.
	 * 
	 * @param provider: A supported execution provider using the ExecutionProvider enum value.
	 * @return A string corresponding to the ONNX Runtime execution provider name.
	 */
	std::string ToProviderString(ExecutionProvider provider);
}

/**
 * @brief Utility to check if a function fails (returns a failing HRESULT) or throws an exception.
 *
 * ---
 *
 * If the function fails (returns a failing HRESULT) or throws an exception, this utility logs the error
 * message with the `Logger::Type::Error` log type, along with a stack trace for easier debugging.
 *
 * @param result: The HRESULT of a function.
 * @throws std::runtime_error: If the function throws an exception or returns a failing HRESULT.
 */
void ThrowIfFailed(HRESULT result);

/**
 * @brief Captures the current stack trace for debugging purposes.
 *
 * ---
 *
 * This function uses the Windows API to retrieve a stack trace of the current thread.
 * It resolves function names and addresses, providing a detailed view of the call stack
 * at the point where the function is invoked.
 *
 * Example stack trace:
 * ```
 * Stack trace:
 *   - SomeFunction at 0x7ff658330f8e
 *   - main at 0x7ff658330fa4
 *   - __libc_start_main at 0x7ff65801023d
 *   - _start at 0x7ff658000134
 * ```
 *
 * @return A formatted string containing the stack trace, with each frame showing the
 *         function name and memory address. If function names cannot be resolved,
 *         the frame is labeled as `[unknown symbol]`.
 */
std::string GetStackTrace();