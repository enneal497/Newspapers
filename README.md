# Placeholder

This is a basic plugin template using CommonLibSSE-NG.

### Requirements
* [XMake](https://xmake.io) [2.8.2+]
* C++23 Compiler (MSVC, Clang-CL)

## Getting Started
```bat
git clone --recurse-submodules https://github.com/enneal497/Newspapers
cd Newspapers
```

### Setup
If you want to redirect the build output, set one of or both of the following environment variables:
- Path to a Skyrim install folder: `XSE_TES5_GAME_PATH`
- Path to a Mod Manager mods folder: `XSE_TES5_MODS_PATH`

To set up the project, run:
```bat
xmake lua setup.lua MyPluginName
```

### Build
To build the project, run the following command:
```bat
xmake build
```

> ***Note:*** *This will generate a `build/windows/` directory in the **project's root directory** with the build output.*

### Project Generation (Optional)
If you want to generate a Visual Studio project, run the following command:
```bat
xmake project -k vsxmake
```

> ***Note:*** *This will generate a `vsxmakeXXXX/` directory in the **project's root directory** using the latest version of Visual Studio installed on the system.*

### Upgrading Packages (Optional)
If you want to upgrade the project's dependencies, run the following commands:
```bat
xmake repo --update
xmake require --upgrade
```

## Documentation
Please refer to the [Wiki](https://github.com/libxse/commonlibsse-ng-template/wiki) for more advanced topics.
