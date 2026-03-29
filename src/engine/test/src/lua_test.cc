#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <unordered_set>

#include "uinta/lua/api_version.h"
#include "uinta/lua/lua_runtime.h"
#include "uinta/lua/plugin_info.h"
#include "uinta/lua/plugin_loader.h"
#include "uinta/lua/plugin_registry.h"

namespace uinta {

// ============================================================================
// ApiVersion Tests
// ============================================================================

TEST(ApiVersion, DefaultValues) {
  ApiVersion v{.major = 0, .minor = 0, .patch = 0};
  EXPECT_EQ(0, v.major);
  EXPECT_EQ(0, v.minor);
  EXPECT_EQ(0, v.patch);
}

TEST(ApiVersion, Parse) {
  auto v = ApiVersion::parse("1.2.3");
  EXPECT_EQ(1, v.major);
  EXPECT_EQ(2, v.minor);
  EXPECT_EQ(3, v.patch);
}

TEST(ApiVersion, ParseMissingPatch) {
  auto v = ApiVersion::parse("1.2");
  EXPECT_EQ(1, v.major);
  EXPECT_EQ(2, v.minor);
  EXPECT_EQ(0, v.patch);
}

TEST(ApiVersion, ParseMissingMinor) {
  auto v = ApiVersion::parse("1");
  EXPECT_EQ(1, v.major);
  EXPECT_EQ(0, v.minor);
  EXPECT_EQ(0, v.patch);
}

TEST(ApiVersion, ParseEmpty) {
  auto v = ApiVersion::parse("");
  EXPECT_EQ(0, v.major);
  EXPECT_EQ(0, v.minor);
  EXPECT_EQ(0, v.patch);
}

TEST(ApiVersion, ParseInvalid) {
  auto v = ApiVersion::parse("abc.def.ghi");
  EXPECT_EQ(0, v.major);
  EXPECT_EQ(0, v.minor);
  EXPECT_EQ(0, v.patch);
}

TEST(ApiVersion, ToString) {
  ApiVersion v{.major = 1, .minor = 2, .patch = 3};
  EXPECT_EQ("1.2.3", v.toString());
}

TEST(ApiVersion, CompatibleSameVersion) {
  ApiVersion current{.major = 1, .minor = 0, .patch = 0};
  ApiVersion required{.major = 1, .minor = 0, .patch = 0};
  EXPECT_TRUE(current.isCompatible(required));
}

TEST(ApiVersion, CompatibleHigherMinor) {
  ApiVersion current{.major = 1, .minor = 2, .patch = 0};
  ApiVersion required{.major = 1, .minor = 1, .patch = 0};
  EXPECT_TRUE(current.isCompatible(required));
}

TEST(ApiVersion, IncompatibleLowerMinor) {
  ApiVersion current{.major = 1, .minor = 0, .patch = 0};
  ApiVersion required{.major = 1, .minor = 1, .patch = 0};
  EXPECT_FALSE(current.isCompatible(required));
}

TEST(ApiVersion, IncompatibleDifferentMajor) {
  ApiVersion current{.major = 2, .minor = 0, .patch = 0};
  ApiVersion required{.major = 1, .minor = 0, .patch = 0};
  EXPECT_FALSE(current.isCompatible(required));
}

TEST(ApiVersion, Equality) {
  ApiVersion a{.major = 1, .minor = 2, .patch = 3};
  ApiVersion b{.major = 1, .minor = 2, .patch = 3};
  ApiVersion c{.major = 1, .minor = 2, .patch = 4};
  EXPECT_EQ(a, b);
  EXPECT_NE(a, c);
}

TEST(ApiVersion, LessThan) {
  EXPECT_LT((ApiVersion{0, 0, 0}), (ApiVersion{1, 0, 0}));
  EXPECT_LT((ApiVersion{1, 0, 0}), (ApiVersion{1, 1, 0}));
  EXPECT_LT((ApiVersion{1, 1, 0}), (ApiVersion{1, 1, 1}));
  EXPECT_FALSE((ApiVersion{1, 0, 0}) < (ApiVersion{0, 9, 9}));
}

// ============================================================================
// PluginRegistry Tests
// ============================================================================

TEST(PluginRegistry, RegisterAndRetrieve) {
  PluginRegistry registry;
  PluginInfo info;
  info.id = "test_plugin";
  info.name = "Test Plugin";
  info.version = "1.0.0";

  registry.registerPlugin(info);

  EXPECT_TRUE(registry.hasPlugin("test_plugin"));
  const PluginInfo* retrieved = registry.getPlugin("test_plugin");
  ASSERT_NE(nullptr, retrieved);
  EXPECT_EQ("test_plugin", retrieved->id);
  EXPECT_EQ("Test Plugin", retrieved->name);
}

TEST(PluginRegistry, Unregister) {
  PluginRegistry registry;
  PluginInfo info;
  info.id = "test_plugin";

  registry.registerPlugin(info);
  EXPECT_TRUE(registry.hasPlugin("test_plugin"));

  registry.unregisterPlugin("test_plugin");
  EXPECT_FALSE(registry.hasPlugin("test_plugin"));
}

TEST(PluginRegistry, GetNonexistent) {
  PluginRegistry registry;
  EXPECT_EQ(nullptr, registry.getPlugin("nonexistent"));
  EXPECT_FALSE(registry.hasPlugin("nonexistent"));
}

TEST(PluginRegistry, GetAllPlugins) {
  PluginRegistry registry;

  PluginInfo info1;
  info1.id = "plugin1";
  registry.registerPlugin(info1);

  PluginInfo info2;
  info2.id = "plugin2";
  registry.registerPlugin(info2);

  auto plugins = registry.getAllPlugins();
  EXPECT_EQ(2, plugins.size());
}

TEST(PluginRegistry, GetPluginsInLoadOrder) {
  PluginRegistry registry;

  PluginInfo info1;
  info1.id = "plugin1";
  info1.loadOrder = 10;
  registry.registerPlugin(info1);

  PluginInfo info2;
  info2.id = "plugin2";
  info2.loadOrder = 5;
  registry.registerPlugin(info2);

  PluginInfo info3;
  info3.id = "plugin3";
  info3.loadOrder = 15;
  registry.registerPlugin(info3);

  auto plugins = registry.getPluginsInLoadOrder();
  ASSERT_EQ(3, plugins.size());
  EXPECT_EQ("plugin2", plugins[0]->id);
  EXPECT_EQ("plugin1", plugins[1]->id);
  EXPECT_EQ("plugin3", plugins[2]->id);
}

TEST(PluginRegistry, DisablePlugin) {
  PluginRegistry registry;
  PluginInfo info;
  info.id = "test_plugin";
  info.enabled = true;

  registry.registerPlugin(info);
  EXPECT_TRUE(registry.getPlugin("test_plugin")->enabled);

  registry.disablePlugin("test_plugin");
  EXPECT_FALSE(registry.getPlugin("test_plugin")->enabled);
}

TEST(PluginRegistry, Clear) {
  PluginRegistry registry;

  PluginInfo info1;
  info1.id = "plugin1";
  registry.registerPlugin(info1);

  PluginInfo info2;
  info2.id = "plugin2";
  registry.registerPlugin(info2);

  EXPECT_EQ(2, registry.size());
  EXPECT_FALSE(registry.empty());

  registry.clear();

  EXPECT_EQ(0, registry.size());
  EXPECT_TRUE(registry.empty());
}

// ============================================================================
// PluginLoader Tests
// ============================================================================

class PluginLoaderTest : public ::testing::Test {
 protected:
  void SetUp() override {
    testDir_ = std::filesystem::temp_directory_path() / "uinta_lua_test";
    std::filesystem::create_directories(testDir_);
  }

  void TearDown() override { std::filesystem::remove_all(testDir_); }

  static void writeFile(const std::filesystem::path& path,
                        const std::string& content) {
    std::filesystem::create_directories(path.parent_path());
    std::ofstream file(path);
    file << content;
  }

  std::filesystem::path testDir_;
  PluginLoader loader_;
};

TEST_F(PluginLoaderTest, DiscoverPlugins) {
  writeFile(testDir_ / "plugin_a" / "plugin.yaml", R"(
id: plugin_a
name: Plugin A
version: "1.0.0"
api_version: "1.0.0"
)");
  writeFile(testDir_ / "plugin_a" / "init.lua", "");

  writeFile(testDir_ / "plugin_b" / "plugin.yaml", R"(
id: plugin_b
name: Plugin B
version: "2.0.0"
api_version: "1.0.0"
)");
  writeFile(testDir_ / "plugin_b" / "init.lua", "");

  auto result = loader_.discoverPlugins({testDir_});
  ASSERT_TRUE(result.ok()) << result.status().message();

  const auto& plugins = *result;
  EXPECT_EQ(2, plugins.size());

  // Both plugins should be found (order may vary due to directory iteration)
  std::unordered_set<std::string> ids;
  for (const auto& p : plugins) {
    ids.insert(p.id);
  }
  EXPECT_TRUE(ids.contains("plugin_a"));
  EXPECT_TRUE(ids.contains("plugin_b"));
}

TEST_F(PluginLoaderTest, DiscoverPluginsSkipsDisabled) {
  writeFile(testDir_ / "enabled_plugin" / "plugin.yaml", R"(
id: enabled_plugin
name: Enabled
version: "1.0.0"
api_version: "1.0.0"
)");

  writeFile(testDir_ / "disabled_plugin" / "plugin.yaml", R"(
id: disabled_plugin
name: Disabled
version: "1.0.0"
api_version: "1.0.0"
)");
  writeFile(testDir_ / "disabled_plugin" / ".disabled", "");

  auto result = loader_.discoverPlugins({testDir_});
  ASSERT_TRUE(result.ok()) << result.status().message();

  const auto& plugins = *result;
  ASSERT_EQ(1, plugins.size());
  EXPECT_EQ("enabled_plugin", plugins[0].id);
}

TEST_F(PluginLoaderTest, DiscoverPluginsFirstWins) {
  // Create two search paths with a plugin of the same ID
  auto path1 = testDir_ / "path1";
  auto path2 = testDir_ / "path2";

  writeFile(path1 / "my_plugin" / "plugin.yaml", R"(
id: my_plugin
name: First Version
version: "1.0.0"
api_version: "1.0.0"
)");

  writeFile(path2 / "my_plugin" / "plugin.yaml", R"(
id: my_plugin
name: Second Version
version: "2.0.0"
api_version: "1.0.0"
)");

  auto result = loader_.discoverPlugins({path1, path2});
  ASSERT_TRUE(result.ok()) << result.status().message();

  const auto& plugins = *result;
  ASSERT_EQ(1, plugins.size());
  EXPECT_EQ("my_plugin", plugins[0].id);
  EXPECT_EQ("First Version", plugins[0].name);
  EXPECT_EQ("1.0.0", plugins[0].version);
}

TEST_F(PluginLoaderTest, DiscoverPluginsSkipsMissingYaml) {
  // Directory without plugin.yaml should be ignored
  std::filesystem::create_directories(testDir_ / "no_yaml_dir");

  writeFile(testDir_ / "valid_plugin" / "plugin.yaml", R"(
id: valid_plugin
name: Valid
version: "1.0.0"
api_version: "1.0.0"
)");

  auto result = loader_.discoverPlugins({testDir_});
  ASSERT_TRUE(result.ok()) << result.status().message();

  const auto& plugins = *result;
  ASSERT_EQ(1, plugins.size());
  EXPECT_EQ("valid_plugin", plugins[0].id);
}

TEST_F(PluginLoaderTest, DiscoverPluginsSkipsNonexistentPath) {
  // A nonexistent path should be silently skipped (with a warning logged)
  auto result = loader_.discoverPlugins(
      {std::filesystem::path("/nonexistent/path/that/does/not/exist")});
  ASSERT_TRUE(result.ok()) << result.status().message();
  EXPECT_TRUE(result->empty());
}

TEST_F(PluginLoaderTest, DiscoverPluginsEmptyPaths) {
  auto result = loader_.discoverPlugins({});
  ASSERT_TRUE(result.ok()) << result.status().message();
  EXPECT_TRUE(result->empty());
}

TEST_F(PluginLoaderTest, LoadPluginInfo) {
  std::string pluginYaml = R"(
id: my_plugin
name: My Plugin
version: "1.2.3"
api_version: "1.0.0"
author: Test Author
dependencies:
  - core
load_order: 5
)";
  writeFile(testDir_ / "my_plugin" / "plugin.yaml", pluginYaml);

  auto result = loader_.loadPluginInfo((testDir_ / "my_plugin").string());
  ASSERT_TRUE(result.ok()) << result.status().message();

  const PluginInfo& info = *result;
  EXPECT_EQ("my_plugin", info.id);
  EXPECT_EQ("My Plugin", info.name);
  EXPECT_EQ("1.2.3", info.version);
  EXPECT_EQ(1, info.apiVersion.major);
  EXPECT_EQ("Test Author", info.author);
  ASSERT_EQ(1, info.dependencies.size());
  EXPECT_EQ("core", info.dependencies[0]);
  EXPECT_EQ(5, info.loadOrder);
}

TEST_F(PluginLoaderTest, LoadPluginInfoNotFound) {
  auto result = loader_.loadPluginInfo("/nonexistent/plugin");
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(IsNotFound(result.status()));
}

TEST_F(PluginLoaderTest, ResolveLoadOrderNoDependencies) {
  std::vector<PluginInfo> plugins;

  PluginInfo p1;
  p1.id = "plugin1";
  p1.loadOrder = 10;
  plugins.push_back(p1);

  PluginInfo p2;
  p2.id = "plugin2";
  p2.loadOrder = 5;
  plugins.push_back(p2);

  auto result = loader_.resolveLoadOrder(plugins);
  ASSERT_TRUE(result.ok());

  const auto& order = *result;
  ASSERT_EQ(2, order.size());
  EXPECT_EQ("plugin2", order[0]);
  EXPECT_EQ("plugin1", order[1]);
}

TEST_F(PluginLoaderTest, ResolveLoadOrderWithDependencies) {
  std::vector<PluginInfo> plugins;

  PluginInfo p1;
  p1.id = "plugin1";
  p1.loadOrder = 0;
  p1.dependencies = {"plugin2"};
  plugins.push_back(p1);

  PluginInfo p2;
  p2.id = "plugin2";
  p2.loadOrder = 0;
  plugins.push_back(p2);

  auto result = loader_.resolveLoadOrder(plugins);
  ASSERT_TRUE(result.ok());

  const auto& order = *result;
  ASSERT_EQ(2, order.size());
  EXPECT_EQ("plugin2", order[0]);
  EXPECT_EQ("plugin1", order[1]);
}

TEST_F(PluginLoaderTest, ResolveLoadOrderCircularDependency) {
  std::vector<PluginInfo> plugins;

  PluginInfo p1;
  p1.id = "plugin1";
  p1.dependencies = {"plugin2"};
  plugins.push_back(p1);

  PluginInfo p2;
  p2.id = "plugin2";
  p2.dependencies = {"plugin1"};
  plugins.push_back(p2);

  auto result = loader_.resolveLoadOrder(plugins);
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(IsInvalidArgument(result.status()));
}

TEST_F(PluginLoaderTest, ResolveLoadOrderMissingDependency) {
  std::vector<PluginInfo> plugins;

  PluginInfo p1;
  p1.id = "plugin1";
  p1.dependencies = {"nonexistent"};
  plugins.push_back(p1);

  auto result = loader_.resolveLoadOrder(plugins);
  EXPECT_FALSE(result.ok());
  EXPECT_TRUE(IsInvalidArgument(result.status()));
}

// ============================================================================
// LuaRuntime Tests
// ============================================================================

TEST(LuaRuntime, Initialize) {
  LuaRuntime runtime;
  auto status = runtime.initialize();
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, ExecuteSimpleScript) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript("x = 1 + 1");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, ExecuteScriptError) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript("this is not valid lua syntax");
  EXPECT_FALSE(status.ok());
}

TEST(LuaRuntime, SandboxingRemovesOsExecute) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  // os should be nil (not loaded) OR os.execute should be nil after sandboxing
  auto status = runtime.executeScript(R"(
    assert(os == nil or os.execute == nil, "os.execute should be nil or os should not be loaded")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, SandboxingRemovesIo) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  // io should be nil after sandboxing
  auto status = runtime.executeScript(R"(
    assert(io == nil, "io should be nil")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, SandboxingRemovesDebug) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  // debug should be nil after sandboxing
  auto status = runtime.executeScript(R"(
    assert(debug == nil, "debug should be nil")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, SandboxingRemovesLoadfile) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript(R"(
    assert(loadfile == nil, "loadfile should be nil")
    assert(dofile == nil, "dofile should be nil")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, CoreBindingsLogging) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  // Test that logging functions exist and are callable
  auto status = runtime.executeScript(R"(
    assert(uinta.log.info ~= nil, "uinta.log.info should exist")
    assert(uinta.log.warn ~= nil, "uinta.log.warn should exist")
    assert(uinta.log.error ~= nil, "uinta.log.error should exist")
    assert(uinta.log.debug ~= nil, "uinta.log.debug should exist")
    uinta.log.info("Test log message from Lua")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, CoreBindingsVec2) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript(R"(
    local v1 = uinta.vec2(1, 2)
    assert(v1.x == 1, "vec2.x failed")
    assert(v1.y == 2, "vec2.y failed")

    local v2 = uinta.vec2(3, 4)
    local v3 = v1 + v2
    assert(v3.x == 4, "vec2 addition x failed")
    assert(v3.y == 6, "vec2 addition y failed")

    local v4 = v1 * 2
    assert(v4.x == 2, "vec2 scalar multiply x failed")
    assert(v4.y == 4, "vec2 scalar multiply y failed")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, CoreBindingsVec3) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript(R"(
    local v1 = uinta.vec3(1, 2, 3)
    assert(v1.x == 1, "vec3.x failed")
    assert(v1.y == 2, "vec3.y failed")
    assert(v1.z == 3, "vec3.z failed")

    local v2 = uinta.vec3(4, 5, 6)
    local v3 = v1 + v2
    assert(v3.x == 5, "vec3 addition x failed")
    assert(v3.y == 7, "vec3 addition y failed")
    assert(v3.z == 9, "vec3 addition z failed")

    local len = v1:length()
    assert(len > 3.7 and len < 3.8, "vec3 length failed: " .. tostring(len))

    local cross = v1:cross(v2)
    assert(cross.x == -3, "cross product x failed")
    assert(cross.y == 6, "cross product y failed")
    assert(cross.z == -3, "cross product z failed")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, CoreBindingsVec4) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript(R"(
    local v = uinta.vec4(1, 2, 3, 4)
    assert(v.x == 1, "vec4.x failed")
    assert(v.y == 2, "vec4.y failed")
    assert(v.z == 3, "vec4.z failed")
    assert(v.w == 4, "vec4.w failed")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, EventSystem) {
  LuaRuntime runtime;
  ASSERT_TRUE(runtime.initialize().ok());

  auto status = runtime.executeScript(R"(
    local received = false
    local receivedData = nil

    uinta.events.on("test.event", function(data)
      received = true
      receivedData = data
    end)

    uinta.events.emit("test.event", "hello")

    assert(received == true, "Event was not received")
    assert(receivedData == "hello", "Event data was incorrect")
  )");
  EXPECT_TRUE(status.ok()) << status.message();
}

TEST(LuaRuntime, ApiVersion) {
  EXPECT_EQ(1, LuaRuntime::apiVersion().major);
  EXPECT_EQ(0, LuaRuntime::apiVersion().minor);
  EXPECT_EQ(0, LuaRuntime::apiVersion().patch);
}

// ============================================================================
// Custom BindingModule Test
// ============================================================================

}  // namespace uinta
