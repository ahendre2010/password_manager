$BuildDirectory = "_build"
if (-not (Test-Path -LiteralPath $BuildDirectory)) {
	New-Item -Name $BuildDirectory -Type Directory
}

Push-Location _build
cmake -G "Visual Studio 17 2022" ..
Pop-Location
