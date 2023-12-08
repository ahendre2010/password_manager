# Download 3rd party diectory folder
if(-not (Test-Path -LiteralPath "3rd_party.7z")){	
	Invoke-WebRequest "https://drive.google.com/uc?export=download&id=1dObRp6LOEbOD46GZUBWIUNz-hk13DX-t" -OutFile 3rd_party.7z
	
	# If link above does not work, use commented link to download using browser.
	# https://drive.google.com/file/d/1dObRp6LOEbOD46GZUBWIUNz-hk13DX-t/view?usp=sharing
}

# Extract
7z x ./3rd_party.7z -y

$BuildDirectory = "_build"
if (-not (Test-Path -LiteralPath $BuildDirectory)) {
	New-Item -Name $BuildDirectory -Type Directory
}

Push-Location _build
cmake -G "Visual Studio 17 2022" ..
Pop-Location
