#include "adapter.h"
#include "utils.h"

#ifdef _WIN64
#include <windows.h>
#include <dxgi1_6.h>
#include <d3d12.h>
#include <DirectML.h>
#endif

namespace MLCore {
    bool IsDMLSupported(AdapterInfo& adapterInfo) {
#ifdef _WIN64
        ComPtr<ID3D12Device> device;

        HRESULT hr = D3D12CreateDevice(
            adapterInfo.adapter.Get(),
            D3D_FEATURE_LEVEL_11_0,
            IID_PPV_ARGS(&device)
        );

        if (FAILED(hr))
            return false;

        ComPtr<IDMLDevice> dmlDevice;
        hr = DMLCreateDevice(
            device.Get(),
            DML_CREATE_DEVICE_FLAG_NONE,
            IID_PPV_ARGS(&dmlDevice)
        );

        if (FAILED(hr))
            return false;

        DML_FEATURE_LEVEL requestedFeatureLevels[] = {
            DML_FEATURE_LEVEL_6_2,
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
            DML_FEATURE_LEVEL_1_0,
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

        return (featureData.MaxSupportedFeatureLevel >= DML_FEATURE_LEVEL_1_0);
#else
        return false;
#endif
    }

    /*
     * Check if the given adapter is compatible with the given execution provider.
     */
    bool IsAdapterCompatible(AdapterInfo& adapterInfo, const OrtUtils::ExecutionProvider& executionProvider) {
#ifdef _WIN64
        switch (executionProvider)
        {
        case OrtUtils::ExecutionProvider::DirectML:
            return IsDMLSupported(adapterInfo);
        default:
            return false;
        }
#else
        return false;
#endif
    }

#ifdef _WIN64
    /**
     * Get the most appropiate adapter given the specified GPU preference.
     */
    AdapterInfo GetAdapter(DXGI_GPU_PREFERENCE gpuPreference, const OrtUtils::ExecutionProvider& executionProvider) {
        ComPtr<IDXGIFactory6> factory;
        ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&factory)));

        AdapterInfo bestAdapter = {};
        SIZE_T bestMetric = (gpuPreference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) ? 0 : SIZE_MAX;

        ComPtr<IDXGIAdapter4> adapter;
        UINT adapterIndex = 0;

        while (SUCCEEDED(factory->EnumAdapterByGpuPreference(adapterIndex++, gpuPreference, IID_PPV_ARGS(&adapter)))) {
            DXGI_ADAPTER_DESC3 desc;
            adapter->GetDesc3(&desc);

            bool isSoftwareAdapter = (desc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE);

            if (!isSoftwareAdapter) {
                bool betterChoice = false;
                if (gpuPreference == DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE) {
                    if (desc.DedicatedVideoMemory > bestMetric) {
                        bestMetric = desc.DedicatedVideoMemory;
                        betterChoice = true;
                    }
                }
                else if (gpuPreference == DXGI_GPU_PREFERENCE_MINIMUM_POWER) {
                    if (desc.DedicatedVideoMemory < bestMetric) {
                        bestMetric = desc.DedicatedVideoMemory;
                        betterChoice = true;
                    }
                }

                if (betterChoice) {
                    bestAdapter = { adapter, desc };
                }
            }

            adapter.Reset();
        }

        if (!bestAdapter.adapter)
            THROW_RUNTIME_ERROR("No suitable GPU found for the specified preference.");

        //if (!IsAdapterCompatible(bestAdapter, executionProvider))
            //THROW_RUNTIME_ERROR("The most powerful adapter is not compatible with the execution provider " + OrtUtils::ToProviderString(executionProvider));

        return bestAdapter;
    }
#endif

    AdapterInfo GetMostPowerfulAdapter(const OrtUtils::ExecutionProvider executionProvider) {
#ifdef _WIN64
        return GetAdapter(DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, executionProvider);
#else
        THROW_RUNTIME_ERROR("Adapters are only available on Windows platforms.");
#endif
    }

    AdapterInfo GetMostEfficientAdapter(const OrtUtils::ExecutionProvider& executionProvider) {
#ifdef _WIN64
        return GetAdapter(DXGI_GPU_PREFERENCE::DXGI_GPU_PREFERENCE_MINIMUM_POWER, executionProvider);
#else
        THROW_RUNTIME_ERROR("Adapters are only available on Windows platforms.");
#endif
    }

    void PrintAdapterDetails(AdapterInfo& adapterInfo) {
#ifdef _WIN64
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
#endif
    }
}
