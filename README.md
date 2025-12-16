# ObjectARX Project Setup Guide (Manual Configuration)

**Target:** AutoCAD 2022 (x64)
**Method:** Manual Configuration (No Wizard)

## 1. Project Creation
1. Open Visual Studio 2022.
2. Create a **Empty Project (C++)**.
3. Set Solution Platform to **x64**.

## 2. Project Properties
*Right-click Project > Properties. Ensure "All Configurations" and "x64" are selected.*

### A. General Settings
* **Configuration Properties > General:**
  * **Configuration Type:** Dynamic Library (.dll)
  * **Target Extension:** `.arx` (Manually type this)
  * **Platform Toolset:** v143 (or latest installed)
  * **C++ Language Standard:** ISO C++20 (or C++17)

### B. Compiler Settings
* **C/C++ > General:**
  * **Additional Include Directories:** `C:\Autodesk\ObjectARX_2022\inc` (Adjust to your specific path)
* **C/C++ > Code Generation:**
  * **Runtime Library:** Multi-threaded DLL (`/MD`) (**Crucial!** Must match AutoCAD)
* **C/C++ > Preprocessor:**
  * **Preprocessor Definitions:** Add `_ADESK_WINDOWS_;_WIN64;`

### C. Linker Settings
* **Linker > General:**
  * **Additional Library Directories:** `C:\Autodesk\ObjectARX_2022\lib-x64` (Adjust to your specific path)
* **Linker > Input:**
  * **Additional Dependencies:**
    ```text
    accore.lib;
    acdb24.lib;
    acge24.lib;
    rxapi.lib;
    ac1st24.lib;
    acad.lib;
    acapp.lib;
    acpal.lib;
    ```
  * **Module Definition File:** `ObjectARX_HelloWorld.def` (see E.)

### D. Debugging Setup (F5 Launch)
* **Configuration Properties > Debugging:**
  * **Command:** `...\AutoCAD 2022\acad.exe` (Path to your installed `acad.exe`)
  * **Command Arguments:** `/ld "$(TargetPath)"` (Auto-loads the plugin on launch)

### E. The Definition File (ObjectARX_HelloWorld.def)
* **Must be added to Source Files in Solution Explorer and linked in Properties.**
  ```c++
  LIBRARY "ObjectARX_HelloWorld"
  EXPORTS
  	acrxEntryPoint        PRIVATE
  	acrxGetApiVersion     PRIVATE
  ```
