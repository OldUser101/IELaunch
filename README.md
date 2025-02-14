# IELaunch
IELaunch is a Windows application designed to launch Internet Explorer under Windows 10 and greater, where it is no longer possible due to operating system restrictions.
This is helpful for legacy applications that require Internet Explorer to function. 

⚠ **WARNING**: Internet Explorer is no longer supported by Microsoft, and may be vulnerable. See [this notice](https://github.com/OldUser101/IELaunch#notice) or the Microsoft website for further details.

## Compatibility
IELaunch is compatible with the following operating systems:
- **Windows 11**
- **Windows 10**

Older Windows versions may be supported, but Internet Explorer restrictions are not in place, so IELaunch is not needed.

IELaunch is compatible with the following processor architecures:
- **x86-64 (x64)**
- **x86**

IELaunch may function on **ARM64** architectures, but this has not been tested, and binaries for ARM64 are not currently provided.

## Installation
IELaunch can be installed in one of the following ways:
- GitHub Releases MSI (recommended)
- GitHub Releases ZIP
- Building from source

Read the corresponding sections for more information.

### GitHub Releases MSI
1. Visit [the releases page](https://github.com/OldUser101/IELaunch/releases) and download the appropriate MSI package for your system.
2. Open the downloaded package and wait for the installation to complete.
3. Desktop and Start Menu shortcuts are created automatically.
4. Run `IELaunch` from the Desktop or Start Menu to start Internet Explorer.

### GitHub Releases ZIP
1. Visit [the releases page](https://github.com/OldUser101/IELaunch/releases) and download the appropriate ZIP package for your system.
2. Extract the contents of the downloaded ZIP file into a directory of your choice.
3. Run `IELaunch.exe` to start Internet Explorer.
4. **OPTIONAL**: Create shortcuts to `IELaunch.exe` for easy access. 

### Building from source
Prerequisites:
- **Microsoft Visual Studio** (Tested with Visual Studio 2022, but may work on older versions), with C++ Desktop Development workload.
- **Windows SDK** (Tested with version 10, but may work with older versions). Automatically installed when selecting C++ Desktop Development workload in Visual Studio.
- **WiX Toolset v5** for building installer.

1. Clone this repository with `git clone https://github.com/OldUser101/IELaunch.git`.
2. Open the solution file `IELaunch.sln` in Visual Studio.
3. Select the appropriate configuration, and build `IELaunch.vcxproj`.
4. Locate the compiled binaries, look for `IELaunch.exe` in your project directory.

## Notice
⚠ **WARNING**: Internet Explorer is **deprecated** and is no longer supported by Microsoft. This means it will no longer receive important security updates. Check Microsoft's website for further details. IELaunch is intended for legacy applications where Internet Explorer is required. It is **strongly** recommended that you do not use Internet Explorer. 
