# SimpleKit
A set of useful helpers (mostly) written in C++ and C#.

## Building
Most of the time, you'll want to use the `BuildAll.ps1` script. There are 4 parameters you can adjust:

- `BuildProjects`: whether to build the `SimpleKit` solution
- `CreatePackages`: whether to create NuGet packages for the projects - if you specify this parameter, `NuGetSource` should be set as well
- `ProjectLog`: where to log the project build output - by default it's logged to the folder containing the script, to a file named `out.txt`
- `NuGetSource`: a NuGet package source where the packages should be added if the `CreatePackages` parameter is set
