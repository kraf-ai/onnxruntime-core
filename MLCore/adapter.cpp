#include "adapter.h"

namespace MLBackend {
	AdapterInfo Adapter::GetMostPowerfulAdapter(OrtUtils::ExecutionProvider& executionProvider) {
		return GetAdapter(DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, executionProvider);
		
	}

	AdapterInfo Adapter::GetMostEfficientAdapter(OrtUtils::ExecutionProvider& executionProvider) {
		return GetAdapter(DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER, executionProvider);

	}

	void Adapter::PrintAdapterDetails(AdapterInfo& adapterInfo) {
		const DXGI_ADAPTER_DESC3& desc = adapterInfo.desc;

		// Convert wide-character description to a narrow string for logging
		std::wstring wDescription(desc.Description);
		int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wDescription.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string description(sizeNeeded, 0);
		WideCharToMultiByte(CP_UTF8, 0, wDescription.c_str(), -1, &description[0], sizeNeeded, nullptr, nullptr);

		// Log the adapter details
		Log("Adapter Details:");
		Log("Description: " + description);
		Log("Dedicated Video Memory: " + std::to_string(desc.DedicatedVideoMemory / (1024 * 1024)) + " MB");
		Log("Dedicated System Memory: " + std::to_string(desc.DedicatedSystemMemory / (1024 * 1024)) + " MB");
		Log("Shared System Memory: " + std::to_string(desc.SharedSystemMemory / (1024 * 1024)) + " MB");
		Log("Adapter Flags: " + std::to_string(desc.Flags));
	}

	AdapterInfo Adapter::GetAdapter(DXGI_GPU_PREFERENCE gpuPreference, OrtUtils::ExecutionProvider& executionProvider) {
		ComPtr<IDXGIFactory6> factory;
		ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

		AdapterInfo bestAdapter = {};
		SIZE_T bestMetric = (gpuPreference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) ? 0 : SIZE_MAX;

		ComPtr<IDXGIAdapter4> adapter;
		UINT adapterIndex = 0;

		while (SUCCEEDED(factory->EnumAdapterByGpuPreference(adapterIndex++, gpuPreference, IID_PPV_ARGS(&adapter)))) {
			DXGI_ADAPTER_DESC3 desc;
			adapter->GetDesc3(&desc);

			// Software adapters are not backed by physical GPUs and rely on the CPU to emulate GPU functionality.
			// Some examples are the `Microsoft Basic Render Driver` or `WARP Adapter`.
			// These adapters are significantly slower and unsuitable for machine learning.
			bool isSoftwareAdapter = (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE);

			if (!isSoftwareAdapter) {
				bool betterChoice = false;
				if (gpuPreference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) {
					// For "most powerful", we look for maximum dedicated video memory
					if (desc.DedicatedVideoMemory > bestMetric) {
						bestMetric = desc.DedicatedVideoMemory;
						betterChoice = true;
					}
				}
				else if (gpuPreference == DXGI_GPU_PREFERENCE_MINIMUM_POWER) {
					// For "most efficient", we look for minimum dedicated video memory
					if (desc.DedicatedVideoMemory < bestMetric) {
						bestMetric = desc.DedicatedVideoMemory;
						betterChoice = true;
					}
				}

				if (betterChoice) {
					bestAdapter = { adapter, desc };
				}
			}

			adapter.Reset(); // Release the current adapter for the next iteration
		}

		if (!bestAdapter.adapter) {
			std::string error_message = "No suitable GPU found for the specified preference.";
			Log(error_message, Logger::Type::Error);
			throw std::runtime_error(error_message);
		}

		if (!IsAdapterCompatible(bestAdapter, executionProvider)) {
			std::string error_message = "The most powerful adapter is not compatible with the execution provider " + OrtUtils::ToProviderString(executionProvider);
			Log(error_message, Logger::Type::Error);
			throw std::runtime_error(error_message);
		}

		return bestAdapter;
	}

	bool Adapter::IsAdapterCompatible(AdapterInfo& adapterInfo, OrtUtils::ExecutionProvider& executionProvider) {
		switch (executionProvider)
		{
		case OrtUtils::ExecutionProvider::DirectML:
			return IsDMLSupported(adapterInfo);
		default:
			return false;
		}
	}

	bool Adapter::IsDMLSupported(AdapterInfo& adapterInfo) {
		ComPtr<ID3D12Device> device;

		// 1. Attempt to create a Direct3D 12 device using the given adapter
		HRESULT hr = D3D12CreateDevice(
			adapterInfo.adapter.Get(),
			D3D_FEATURE_LEVEL_11_0, // Minimum feature level required for DirectML
			IID_PPV_ARGS(&device)
		);

		if (FAILED(hr))
			return false; // Direct3D 12 device creation failed, so DirectML is not supported

		// 2. Create a DirectML device
		ComPtr<IDMLDevice> dmlDevice;
		hr = DMLCreateDevice(
			device.Get(),
			DML_CREATE_DEVICE_FLAG_NONE,
			IID_PPV_ARGS(&dmlDevice)
		);

		if (FAILED(hr))
			return false; // Failed to create a DirectML device

		// 3. Check for basic DirectML feature support
		DML_FEATURE_LEVEL requestedFeatureLevels[] = {
			DML_FEATURE_LEVEL_6_2, // Latest version
			DML_FEATURE_LEVEL_6_1,
			DML_FEATURE_LEVEL_6_0,
			DML_FEATURE_LEVEL_5_2,
			DML_FEATURE_LEVEL_5_1,
			DML_FEATURE_LEVEL_5_0,
			DML_FEATURE_LEVEL_4_1,
			DML_FEATURE_LEVEL_4_0,
			DML_FEATURE_LEVEL_3_1,
			DML_FEATURE_LEVEL_3_0,
			DML_FEATURE_LEVEL_2_1,
			DML_FEATURE_LEVEL_2_0,
			DML_FEATURE_LEVEL_1_0, // Basic support
		};

		DML_FEATURE_QUERY_FEATURE_LEVELS featureQuery = {};
		featureQuery.RequestedFeatureLevelCount = _countof(requestedFeatureLevels);
		featureQuery.RequestedFeatureLevels = requestedFeatureLevels;

		DML_FEATURE_DATA_FEATURE_LEVELS featureData = {};

		hr = dmlDevice->CheckFeatureSupport(
			DML_FEATURE_FEATURE_LEVELS,
			sizeof(featureQuery),
			&featureQuery,
			sizeof(featureData),
			&featureData
		);

		if (FAILED(hr))
			return false;

		// Check if we got at least DML_FEATURE_LEVEL_1_0 support
		return (featureData.MaxSupportedFeatureLevel >= DML_FEATURE_LEVEL_1_0);
	}
}