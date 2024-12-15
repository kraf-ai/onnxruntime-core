# Create ML projects more easily using ONNX Runtime
This repository contains a set of **utilities and tools** that can be used to **create ONNX Runtime projects more quickly**.

<hr/>

### How to use

This project is intended to be used in a Visual Studio solution containing 2 projects:
1. The `MLCore` project with all these utilities and tools.
2. The main project where you'll use the utilities and tools from `MLCore`.

These are the steps you should follow to use it:
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
5. Now the new `.gitmodules` file and the `MLCore` folder are staged in Git. You can see this using `git status`. So commit change and push it
```sh
git commit -m "Adding MLCore submodule from onnxruntime-core"
git push origin main
```

Now you've cloned the MLCore submodule.

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
