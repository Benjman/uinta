---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-03-01
status: complete
---

# Feature: File I/O

## Overview

Cross-platform file I/O abstraction for reading and writing files, with integrated support for common asset formats through STB libraries.

## Implementation Details

**File Class:**

- File reading abstraction
- File writing abstraction
- Cross-platform path handling
- Binary and text file support
- File existence checking

## Acceptance Criteria

- [x] Read files from filesystem
- [x] Write files to filesystem
- [x] Handle cross-platform paths
- [x] Support binary and text modes
- [x] Check file existence
- [x] Load images via STB Image
- [x] Load fonts via STB TrueType

## Usage Example

### Reading a file

```cpp
if (File::exists("config.yml")) {
  File configFile("config.yml");
  std::string contents = configFile.readAll();
}
```

### Writing a file

```cpp
File outputFile("output.txt");
outputFile.write("Hello, world!");
```
