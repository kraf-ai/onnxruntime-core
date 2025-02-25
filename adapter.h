/*
 * Here are some resources for reference:
 * - Basic DirectX Graphics Infrastructure (DXGI) stuff: https://learn.microsoft.com/en-us/windows/win32/direct3dgetstarted/work-with-dxgi
 * - IDXGIFactory6 interface: https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_6/nn-dxgi1_6-idxgifactory6
 */

#pragma once

#ifdef _WIN64
#include <dxgi1_6.h>              // DXGI 1.6
#include <wrl/client.h>           // Windows Runtime Library - A way to work with COM (Component Object Model) objects
#include <windows.h>              // Windows SDK headers
#include <d3d12.h>                // DirectX12: Direct3D
#include <DirectML.h>             // Direct Machine Learning
#endif

#include <iostream>

#include "utils.h"

#ifdef _WIN64
#pragma comment(lib, "d3d12.lib")     // Direct3D 12 library
#pragma comment(lib, "dxgi.lib")      // DirectX Graphics Infrastructure
#pragma comment(lib, "DirectML.lib")  // DirectML library
#endif

#ifdef _WIN64
using Microsoft::WRL::ComPtr;
#endif

namespace MLCore {
#ifdef _WIN64
	struct AdapterInfo {
		ComPtr<IDXGIAdapter4> adapter;
		DXGI_ADAPTER_DESC3 desc;
	};
#else
	struct AdapterInfo {};
#endif

	/**
	 * @brief Get the most powerful device in the system that supports the given execution provider.
	 *
	 * ---
	 *
	 * This is usually a discrete GPU of the system, if available.
	 *
	 * @param executionProvider: The ORT execution provider that must be supported by the adapter.
	 * @return The information of the selected adapter.
	 * @throws std::runtime_error: If it fails to get the adapter or if no suitable adapter is found.
	 */
	AdapterInfo GetMostPowerfulAdapter(const OrtUtils::ExecutionProvider executionProvider);

	/**
	 * @brief Get the most efficient device in the system that supports the given execution provider.
	 *
	 * ---
	 *
	 * This is usually an integrated GPU of the system, if available.
	 *
	 * @param executionProvider: The ORT execution provider that must be supported by the adapter.
	 * @return The information of the selected adapter.
	 * @throws std::runtime_error: If it fails to get the adapter or if no suitable adapter is found.
	 */
	AdapterInfo GetMostEfficientAdapter(const OrtUtils::ExecutionProvider& executionProvider);

	/**
	 * @brief Logs the adapter's description, memory details, and flags to the logging system.
	 */
	void PrintAdapterDetails(AdapterInfo& adapterInfo);
}