param
(
	[switch]$BuildProjects = $false,
	[switch]$CreatePackages = $false,
	[string]$ProjectLog = "$($PSScriptRoot)\out.txt",
	[string]$NuGetSource
)

if ($BuildProjects)
{
	Remove-Item $ProjectLog
	$platforms = @("x86","x64","ARM","ARM64") | ForEach { devenv SimpleKit.sln /Build "Release|$($_)" /Out $ProjectLog }
}

if ($CreatePackages)
{
	$dirs = Get-ChildItem -Directory "$($PSScriptRoot)\src"
	ForEach ($dir in $dirs)
	{
		if (Test-Path -Path "$($dir.FullName)\NuGetManifest.nuspec")
		{
			Remove-Item "$($dir.FullName)" -Recurse -Include *.nupkg

			nuget pack "$($dir.FullName)\NuGetManifest.nuspec" -OutputDirectory $dir.FullName
			Get-ChildItem -Path $dir.FullName -Filter *.nupkg | ForEach { nuget add $_.FullName -source $NugetSource }
		}
	}
}
