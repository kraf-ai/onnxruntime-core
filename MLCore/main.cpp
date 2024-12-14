#include "common.h"
#include "adapter.h"

int main() {
    Logger::Init("log_backend.txt");

    auto dmlExecutionProvider = OrtUtils::ExecutionProvider::DirectML;

    MLBackend::AdapterInfo powerfulAdapter = MLBackend::Adapter::GetMostPowerfulAdapter(dmlExecutionProvider);

    MLBackend::Adapter::PrintAdapterDetails(powerfulAdapter);

    MLBackend::AdapterInfo efficientAdapter = MLBackend::Adapter::GetMostEfficientAdapter(dmlExecutionProvider);

    MLBackend::Adapter::PrintAdapterDetails(efficientAdapter);

    return 0;
}
