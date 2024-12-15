/*
 * Here are some resources for reference:
 * - Basic DirectX Graphics Infrastructure (DXGI) stuff: https://learn.microsoft.com/en-us/windows/win32/direct3dgetstarted/work-with-dxgi
 * - IDXGIFactory6 interface: https://learn.microsoft.com/en-us/windows/win32/api/dxgi1_6/nn-dxgi1_6-idxgifactory6
 */

#pragma once

#ifdef WINDOWS
#include <dxgi1_6.h>              // DXGI 1.6
#include <wrl/client.h>           // Windows Runtime Library - A way to work with COM (Component Object Model) objects
#include <windows.h>              // Windows SDK headers
#include <d3d12.h>                // DirectX12: Direct3D
#include <DirectML.h>             // Direct Machine Learning
#endif

#include <iostream>

#include "common.h"
#include "utils.h"

#ifdef WINDOWS
#pragma comment(lib, "d3d12.lib")     // Direct3D 12 library
#pragma comment(lib, "dxgi.lib")      // DirectX Graphics Infrastructure
#pragma comment(lib, "DirectML.lib")  // DirectML library
#endif

#ifdef WINDOWS
using Microsoft::WRL::ComPtr;
#endif

namespace MLBackend {
#ifdef WINDOWS
	struct AdapterInfo {
		ComPtr<IDXGIAdapter4> adapter;
		DXGI_ADAPTER_DESC3 desc;
	};
#else
	struct AdapterInfo {};
#endif
	
	class Adapter {
	public:
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
		static AdapterInfo GetMostPowerfulAdapter(OrtUtils::ExecutionProvider& executionProvider);

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
		static AdapterInfo GetMostEfficientAdapter(OrtUtils::ExecutionProvider& executionProvider);

		/**
		 * @brief Logs the adapter's description, memory details, and flags to the logging system.
		 */
		static void PrintAdapterDetails(AdapterInfo& adapterInfo);
	private:
#ifdef WINDOWS
		/**
		 * Get the most appropiate adapter given the specified GPU preference.
		 */
		static AdapterInfo GetAdapter(DXGI_GPU_PREFERENCE gpuPreference, OrtUtils::ExecutionProvider& executionProvider);
#endif
		/*
		 * Check if the given adapter is compatible with the given execution provider.
		 */
		static bool IsAdapterCompatible(AdapterInfo& adapterInfo, OrtUtils::ExecutionProvider& executionProvider);

		/**
		 * Check if DirectML is supported in the given adapter.
		 */
		static bool IsDMLSupported(AdapterInfo& adapterInfo);
	};
}