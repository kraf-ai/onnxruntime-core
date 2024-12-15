#pragma once

#ifdef WINDOWS
#include <windows.h>
#include <dbghelp.h>
#endif

#include <sstream>
#include <type_traits>
#include <stdexcept>

#include "common.h"

#ifdef WINDOWS
#pragma comment(lib, "Dbghelp.lib") // Link the DbgHelp library
#endif

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

#ifdef WINDOWS
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
#endif

#define DEFINE_THROW_MACRO(exceptionType, msg) \
    do { \
        Logger::Log(msg, Logger::Type::Error); \
        throw exceptionType(msg); \
    } while (0)

// Define all THROW macros
#define THROW_RUNTIME_ERROR(msg) DEFINE_THROW_MACRO(std::runtime_error, msg)
#define THROW_LOGIC_ERROR(msg)   DEFINE_THROW_MACRO(std::logic_error, msg)
#define THROW_INVALID_ARGUMENT(msg) DEFINE_THROW_MACRO(std::invalid_argument, msg)
#define THROW_OUT_OF_RANGE(msg) DEFINE_THROW_MACRO(std::out_of_range, msg)
#define THROW_LENGTH_ERROR(msg) DEFINE_THROW_MACRO(std::length_error, msg)
#define THROW_DOMAIN_ERROR(msg) DEFINE_THROW_MACRO(std::domain_error, msg)
#define THROW_OVERFLOW_ERROR(msg) DEFINE_THROW_MACRO(std::overflow_error, msg)
#define THROW_UNDERFLOW_ERROR(msg) DEFINE_THROW_MACRO(std::underflow_error, msg)
#define THROW_SYSTEM_ERROR(msg) DEFINE_THROW_MACRO(std::system_error, msg)