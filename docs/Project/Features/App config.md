---
tags: [feature]
epic: "[[Epics/v0.1]]"
created: 2026-02-25
updated: 2026-02-25
status: complete
---

# Feature: Application Configuration

## Overview

YAML-based configuration system for managing application settings, providing runtime configuration access and persistence.

## Implementation Details

**AppConfig (Base Class):**

- Application configuration management with status tracking
- Runtime configuration access through typed getter/setter API
- Configuration persistence via `flush()` method
- File path resolution with automatic fallback and creation:
  1. Command-line argument (`--app-config-file`)
  2. Default destination directory
  3. FileSearchDirs() search paths
  4. Auto-creation from default template if not found

**AppConfigYaml (Implementation):**

- YAML configuration file loading with `yaml-cpp` library
- Configuration parsing and validation with error reporting
- Nested key support using dot notation (e.g., `"window.width"`)
- Vector type parsing from space-separated strings
- Configuration serialization back to YAML via `flush()`

**AppConfigHandler:**

- Command-line argument processing via ArgsProcessor
- Configuration file path resolution handling
- Validation of user-provided config paths

## API Reference

**Supported Data Types:**
- `getString(key)` / `setString(key, value)` - String values
- `getBoolean(key)` / `setBoolean(key, value)` - Boolean values
- `getInt(key)` / `setInt(key, value)` - Single integer values
- `getInt2/3/4(key, ptr)` / `setInt2/3/4(key, ptr)` - Integer vectors (2-4 components)
- `getFloat(key)` / `setFloat(key, value)` - Single float values
- `getFloat2/3/4(key, ptr)` / `setFloat2/3/4(key, ptr)` - Float vectors (2-4 components)
- `flush()` - Persist changes to disk

**Error Handling:**
- Missing keys return `std::nullopt` and log warnings
- Invalid file paths set error status accessible via `status()`
- Failed file operations report detailed error messages

## Acceptance Criteria

- [x] Load configuration from YAML files
- [x] Access configuration at runtime with typed getters
- [x] Support nested keys with dot notation
- [x] Handle default values for missing settings
- [x] Process command-line overrides for config file path
- [x] Parse and validate YAML structure
- [x] Support vector types (int2/3/4, float2/3/4)
- [x] Persist configuration changes via flush()
- [x] Auto-create config file from template if missing

## Usage Example

```cpp
// Initialize configuration
uinta::AppConfigYamlImpl appConfig(argsProcessor);
if (!appConfig.status().ok()) {
  LOG(ERROR) << appConfig.status().message();
  return;
}

// Read values
auto width = appConfig.getInt("window.width");
auto title = appConfig.getString("app.title");
i32 resolution[2];
auto res = appConfig.getInt2("window.resolution", resolution);

// Write values
appConfig.setInt("window.width", 1920);
appConfig.setString("app.title", "My App");
i32 newRes[2] = {1920, 1080};
appConfig.setInt2("window.resolution", newRes);

// Persist changes
appConfig.flush();
```