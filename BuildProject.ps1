param
(
	[string]$ProjectName,
	[switch]$CreatePackage = $false,
	[string]$ProjectLog = "$($PSScriptRoot)\$($ProjectName)-out.txt",
	[string]$NuGetSource
)

$projectPath = "$($PSScriptRoot)\src\$($ProjectName)"

Remove-Item $ProjectLog
$platforms = @("x86","x64","ARM","ARM64")

ForEach ($platform in $platforms)
{
	devenv SimpleKit.sln /Build "Release|$($platform)" /Project $ProjectName /Out $ProjectLog
}

if ($CreatePackage)
{
	if (Test-Path -Path "$($projectPath)\NuGetManifest.nuspec")
	{
		Remove-Item $projectPath -Recurse -Include *.nupkg

		nuget pack "$($projectPath)\NuGetManifest.nuspec" -OutputDirectory $projectPath
		Get-ChildItem -Path $projectPath -Filter *.nupkg | ForEach { nuget add $_.FullName -source $NugetSource }
	}
}
