# Create ML projects more easily using ONNX Runtime
This repository contains a set of **utilities and tools** that can be used to **create ONNX Runtime projects more quickly**.

<hr/>

### How to use in Visual Studio

This project is intended to be used in a Visual Studio solution containing 2 projects:
1. The `MLCore` project with all these utilities and tools.
2. The main project where you'll use the utilities and tools from `MLCore`.

These are the steps to set this project as a Git submodule:
1. Open Visual Studio and click on `Create a new project`.
2. Create any C++ project that you want. For example, a `Console App`. Give it a name and click on the `Create` button. It's better not to click on the "Place solution and project in the same directory" checkbox to make the main project solution file easier to find.
3. Let's say the project you created has the name `MLProject`. Therefore, all the C++ files of the project wil be found in:
```
<project_path>/MLProject/MLProject
```
4. Now, open a terminal and go to the path `<project_path>/MLProject`. Then clone the `onnxruntime-core` project as a submodule as follows:
```sh
git submodule add https://github.com/kraf-ai/onnxruntime-core MLCore
```
5. Now the new `.gitmodules` file and the `MLCore` folder are staged in Git. You can see this using `git status`. So commit the change:
```sh
git commit -m "Adding MLCore submodule from onnxruntime-core"
```
6. It's also recommended to copy the file `MLCore/.gitignore` into the `<project_path>/MLProject` as it already have configured the files that have to be ignored:
```sh
cp .\MLCore\.gitignore .gitignore
```
7. And remember to commit all your changes
```sh
git add .
git commit -m "Adding new ML project"
```

Now you've cloned the MLCore submodule. It's time to add it as a project to your created solution:
1. Open the solution you've just created. Example of solution file: `MLProject.sln`.
2. In the Solution Explorer, right-click on the solution name. And select `Add → Existing Project`.
3. Then navigate to the folder `<project_path>/MLProject/MLCore` and select the file `MLCore.vcxproj`. The project will now appear under the solution in the Solution Explorer.
4. Now we've to make sure the `MLCore` project is built before the main project. For that purpose, right-click on the solution name and select `Properties`. Then go into `Common Properties → Project Dependencies`. Select your project in the dropdown (`MLProject` in this example). And under the `Depends on:` you'll be able to see the `MLCore` dependency. Click on the checkbox to make it a dependency. Finally, click on `Apply`.
5. To compile the `MLCore` for Windows, right click on the solution name and select `Properties`. Then go to `Configuration Properties → Configuration`. Select the configuration `Debug` and for the `MLCore` project select the `Debug (Win)` configuration. Click on `Apply`. Now do the same for `Release`: Select the `Release` configuration and set `Release (Win)` as the `MLCore` configuration.
6. Reference the `MLCore` library from `MLProject`. This is done by doing right click on `MLProject` and selecting `Add → Reference`. Then click on the checkbox at the left of `MLCore` and click on `OK`.
7. Make sure the `MLCore` is compiled as a static library (.lib). To do that, right click on the `MLCore` project and select `Properties`. Then go into `Configuration Properties → General`. As the `Configuration` set `Debug (Win)`. And modify the `Configuration Type` property selecting the option `Static Library (.lib)` in the dropdown. Apply this change. Now repeat this step but changing the `Configuration` to `Release (Win)`.
8. Update both projects to to C++17. In order to do that, right click on each of the projects and select `Properties`. Then go into `Configuration Properties → General`. And modify the `C++ Language Standard` property selecting the option `ISO C++17 Standard (/std:c++17)` in the dropdown. Apply this change. Do this for all the different configurations of both projects.

🎉 And now you're ready to go! 

To test it:
1. Write the following main function in the `MLProject`:
```cpp
#include "../MLCore/common.h"
#include "../MLCore/adapter.h"

int main() {
    MLBackend::AdapterInfo powerfulAdapter = MLBackend::Adapter::GetMostPowerfulAdapter(OrtUtils::ExecutionProvider::DirectML);
    MLBackend::Adapter::PrintAdapterDetails(powerfulAdapter);
    return 0;
}
```
2. And now run the solution. You should be able to see a terminal with the information of the most powerful GPU of your system. 

### Status of the project

> [!NOTE]
> This project is under development and the first beta hasn't been released yet.

TO-DO list:
- [ ] \(⚒️) Windows beta.
- [ ] \(🕒) Web beta.

Meaning of the emojis in the TO-DO list:
- ⚒️: Under development.
- 🕒: Not started.

### Additional information
This code does not belong to the official ONNX Runtime package and it hasn't being done by Microsoft.
