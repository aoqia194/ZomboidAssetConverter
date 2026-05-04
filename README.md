# ZomboidAssetConverter

A cli program to convert between certain data formats that are used by the game Project Zomboid.

## Features

- Convert DirectX to glTF2 for easy viewing of the original Project Zomboid animations.

## Usage

Viewing all of the options you can use:

```sh
./ZomboidAssetConverter --help
```

To convert a folder of DirectX files:

```sh
./ZomboidAssetConverter "C:\dev\zomboid_assets\anims_X" --output "C:\dev\zomboid_assets\anims_X_converted" --fix-assets --convert-assets
```

## Building

Make sure you have cmake, ninja, and vcpkg installed on your system.

Once vcpkg is setup ([follow this](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell)), you can simply do the following:

```sh
cmake --preset release
cmake --build --preset release
```

You can find the binary in `build/release/bin/`!
