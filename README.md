<p align="center">
	<img src="assets/SimpleKit.png" />
	<br>
	<h1 align="center">SimpleKit</h1>
</p>

A set of useful helpers (mostly) written in C++ and C#

## Building
Most of the time, you'll want to use the `BuildAll.ps1` script. There are 4 parameters you can adjust:

- `BuildProjects`: whether to build the `SimpleKit` solution
- `CreatePackages`: whether to create NuGet packages for the projects - if you specify this parameter, `NuGetSource` should be set as well
- `ProjectLog`: where to log the project build output - by default it's logged to the folder containing the script, to a file named `out.txt`
- `NuGetSource`: a NuGet package source where the packages should be added if the `CreatePackages` parameter is set

If you want to build a single project, use the `BuildProject.ps1` script:

- `ProjectName`: name of the project to build, must be within the `SimpleKit` solution
- `CreatePackage`: whether to create a NuGet package for the project - if you specify this parameter, `NuGetSource` should be set as well
- `ProjectLog`: where to log the project build output - by default it's logged to the folder containing the script, to a file named `{ProjectName}-out.txt`
- `NuGetSource`: a NuGet package source where the package should be added if the `CreatePackage` parameter is set

## Templates
To build and/or use the SimpleKit templates, build the `SimpleKit.WindowsRuntime.Templates` project and install the resulting VSIX. VSIX distribution is under consideration.
