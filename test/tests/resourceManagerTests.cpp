#include <gtest/gtest.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../../../src/resource/utils.hpp"

#define private public
#include "uinta/resource.hpp"

using namespace uinta;

static inline logger_t logger = spdlog::stdout_color_mt("FileManagerTest");

TEST(ResourceManager_parsePaths, happyPath) {
  auto input = "/my/user/path;/second/user/path/";
  std::vector<std::string> buffer;
  parseResourcePaths(input, ';', buffer, logger);
  ASSERT_EQ(2, buffer.size());
  ASSERT_EQ("/my/user/path/", buffer.at(0));
  ASSERT_EQ("/second/user/path/", buffer.at(1));
}

TEST(ResourceManager_parsePaths, emptyInput) {
  auto input = "";
  std::vector<std::string> buffer;
  parseResourcePaths(input, ';', buffer, logger);
  ASSERT_EQ(0, buffer.size());
}

TEST(ResourceManager_parsePaths, singlePath) {
  auto input = "/my/user/path/";
  std::vector<std::string> buffer;
  parseResourcePaths(input, ';', buffer, logger);
  ASSERT_EQ(1, buffer.size());
  ASSERT_EQ("/my/user/path/", buffer.at(0));
}

TEST(ResourceManager_parsePaths, semicolon_only) {
  auto input = ";";
  std::vector<std::string> buffer;
  parseResourcePaths(input, ';', buffer, logger);
  ASSERT_EQ(0, buffer.size());
}

TEST(ResourceManager_parsePaths, buffer_clears) {
  auto input = ";";
  std::vector<std::string> buffer = {"Hello"};
  parseResourcePaths(input, ';', buffer, logger);
  ASSERT_EQ(0, buffer.size());
}

TEST(ResourceManager_parsePaths, manyPaths) {
  int pathCount = 100;
  std::string input;
  for (int i = 0; i < pathCount; i++) {
    input += "/path/to/folder/" + std::to_string(i) + "/";
    if (i + 1 != pathCount) input += ";";
  }
  std::vector<std::string> buffer;
  parseResourcePaths(input.c_str(), ';', buffer, logger);
  ASSERT_EQ(pathCount, buffer.size());
  for (int i = 0; i < pathCount; i++) ASSERT_EQ(std::string("/path/to/folder/" + std::to_string(i)) + "/", buffer.at(i));
}

TEST(ResourceManager_parsePaths, delimiter) {
  ASSERT_EQ(';', RES_PATHS_DELIM) << "Delimiter is expected to match cmake's semicolon concatenation.";
}

TEST(ResourceManager_release, pathsClear) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", ResourceType::Text);
  ASSERT_EQ("test", rm.getPath(handle));
  rm.release(handle);
  ASSERT_EQ("", rm.getPath(handle));
}

TEST(ResourceManager_release, isBuffered_false) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", ResourceType::Text);
  rm.setIsBuffered(handle, true);
  ASSERT_TRUE(rm.isBuffered(handle));
  rm.release(handle);
  ASSERT_FALSE(rm.isBuffered(handle));
}

TEST(ResourceManager_release, blockList_reset) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", ResourceType::Text);
  rm.blockList.at(rm.getId(handle)) = ResourceManager::ResourceBlock(nullptr, 1);
  ASSERT_EQ(1, rm.blockList.at(rm.getId(handle)).size);
  rm.release(handle);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
}

TEST(ResourceManager_release, isActive_false) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", ResourceType::Text);
  ASSERT_TRUE(rm.isActive(handle));
  rm.release(handle);
  ASSERT_FALSE(rm.isActive(handle));
}

TEST(ResourceManager_registerFile, addHandle) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", ResourceType::Text);
  ASSERT_EQ(handle, rm.handles.at(rm.getId(handle)));
}

TEST(ResourceManager_registerFile, handlePath) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", ResourceType::Text);
  ASSERT_EQ("path", rm.handlePaths.at(rm.getId(handle)));
}

TEST(ResourceManager_registerFile, blockList) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", ResourceType::Text);
  ASSERT_EQ(1, rm.blockList.size());
  ASSERT_EQ(ResourceManager::ResourceBlock(), rm.blockList.at(rm.getId(handle)));
}

TEST(ResourceManager_registerFile, active) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", ResourceType::Text);
  ASSERT_TRUE(rm.isActive(handle));
}

TEST(ResourceManager_registerFile, type) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", ResourceType::Text);
  ASSERT_EQ(ResourceType::Text, rm.getType(handle));
}

TEST(ResourceManager_getSize, happyPath) {
  FileManager rm(KILOBYTES(1));
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
  rm.loadAllFiles();
  ASSERT_EQ(77, rm.getSize(handle));
}

TEST(ResourceManager_getSize, loadedButInactive) {
  FileManager rm(KILOBYTES(1));
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
  rm.loadAllFiles();
  ASSERT_EQ(77, rm.getSize(handle));
  rm.release(handle);
  ASSERT_EQ(0, rm.getSize(handle));
}

#include <fstream>
TEST(ResourceManager_findPath, absolutePath) {
  if (!std::ifstream("/dev/null")) return;
  ASSERT_EQ("/dev/null", findPath("/dev/null", {}));
}

TEST(ResourceManager_findPath, relativePath) {
  FileManager rm(0);
  rm.init();
  ASSERT_TRUE(findPath("res_man1.txt", rm.resourcePaths).ends_with("/test/res/res_man1.txt"));
}

TEST(ResourceManager_findPath, invalid) {
  FileManager rm(0);
  rm.init();
  ASSERT_EQ("", findPath("foo/bar.baz", rm.resourcePaths));
}

TEST(ResourceManager_loadAllFiles, invalidPathReleases) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("foo/bar.baz", ResourceType::Text);
  ASSERT_TRUE(rm.isActive(handle));
  rm.loadAllFiles();
  ASSERT_FALSE(rm.isActive(handle));
}

TEST(ResourceManager_loadAllFiles, pathIsSet) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  ASSERT_EQ("res_man1.txt", rm.getPath(handle));
  rm.loadAllFiles();
  ASSERT_TRUE(rm.getPath(handle).ends_with("/test/res/res_man1.txt"));
}

TEST(ResourceManager_loadAllFiles, spaceReserved) {
  FileManager rm(100);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle)).ptr);
  rm.loadAllFiles();
  ASSERT_NE(nullptr, rm.blockList.at(rm.getId(handle)).ptr);
}

TEST(ResourceManager_loadAllFiles, loadsData) {
  FileManager rm(100);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle)).ptr);
  rm.loadAllFiles();
  ASSERT_NE(nullptr, rm.blockList.at(rm.getId(handle)).ptr);
  std::string data = rm.getDataChars(handle);
  ASSERT_TRUE(data.starts_with("File 1"));
}

TEST(ResourceManager_reserveSpace, blockResets) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  rm.setIsActive(handle, false);
  rm.blockList.at(rm.getId(handle)) = ResourceManager::ResourceBlock(nullptr, 1);
  ASSERT_EQ(1, rm.blockList.at(rm.getId(handle)).size);
  rm.reserveSpace(handle);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
}

TEST(ResourceManager_reserveSpace, reserveMoreThanAvailable) {
  FileManager rm(1);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
}

TEST(ResourceManager_reserveSpace, firstItem) {
  FileManager rm(100);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
  rm.loadAllFiles();
  ASSERT_EQ(77, rm.blockList.at(rm.getId(handle)).size);
  ASSERT_EQ(rm.storage, rm.blockList.at(rm.getId(handle)).ptr);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle)).back);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle)).forward);
}

TEST(ResourceManager_reserveSpace, secondItem) {
  FileManager rm(200);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  rm.loadAllFiles();
  auto handle1Size = rm.blockList.at(rm.getId(handle1)).size;
  ASSERT_EQ((char*)rm.storage + handle1Size + 1, rm.blockList.at(rm.getId(handle2)).ptr);
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle1)), rm.blockList.at(rm.getId(handle2)).back);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle2)).forward);
}

TEST(ResourceManager_reserveSpace, thirdItem) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  auto handle1Size = rm.blockList.at(rm.getId(handle1)).size;
  auto handle2Size = rm.blockList.at(rm.getId(handle1)).size;

  ASSERT_EQ((char*)rm.storage + handle1Size + handle2Size + 2, rm.blockList.at(rm.getId(handle3)).ptr);

  ASSERT_EQ(&rm.blockList.at(rm.getId(handle1)), rm.blockList.at(rm.getId(handle2)).back);
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle3)), rm.blockList.at(rm.getId(handle2)).forward);

  ASSERT_EQ(&rm.blockList.at(rm.getId(handle2)), rm.blockList.at(rm.getId(handle3)).back);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle3)).forward);
}

TEST(ResourceManager_reserveSpace, noSpaceLeft) {
  FileManager rm(100);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_NE(nullptr, rm.blockList.at(rm.getId(handle1)).ptr);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle2)).ptr);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle2)).size);
}

TEST(ResourceManager_reserveSpace, orphaned_headspace) {
  FileManager rm(200);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  rm.loadAllFiles();
  rm.release(handle1);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_EQ((char*)rm.storage, rm.blockList.at(rm.getId(handle3)).ptr);
}

TEST(ResourceManager_reserveSpace, orphaned_middle) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  rm.release(handle2);
  auto* handle4 = rm.registerFile("res_man4.txt", ResourceType::Text);
  rm.loadAllFiles();
  auto& block1 = rm.blockList.at(rm.getId(handle1));
  ASSERT_EQ((char*)block1.ptr + block1.size + 1, rm.blockList.at(rm.getId(handle4)).ptr);
}

TEST(ResourceManager_reserveSpace, orphaned_tail) {
  FileManager rm(200);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  rm.loadAllFiles();
  rm.release(handle2);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  auto& block1 = rm.blockList.at(rm.getId(handle1));
  ASSERT_EQ((char*)block1.ptr + block1.size + 1, rm.blockList.at(rm.getId(handle3)).ptr);
}

TEST(ResourceManager_release, resetsBlock) {
  FileManager rm(300);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_EQ(77, rm.blockList.at(rm.getId(handle)).size);
  rm.release(handle);
  ASSERT_EQ(0, rm.blockList.at(rm.getId(handle)).size);
}

TEST(ResourceManager_release, release_head) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle1)), rm.blockList.at(rm.getId(handle2)).back);
  rm.release(handle1);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle2)).back);
}

TEST(ResourceManager_release, release_middle) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle2)), rm.blockList.at(rm.getId(handle1)).forward);
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle2)), rm.blockList.at(rm.getId(handle3)).back);
  rm.release(handle2);
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle3)), rm.blockList.at(rm.getId(handle1)).forward);
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle1)), rm.blockList.at(rm.getId(handle3)).back);
}

TEST(ResourceManager_release, release_tail) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", ResourceType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", ResourceType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", ResourceType::Text);
  rm.loadAllFiles();
  ASSERT_EQ(&rm.blockList.at(rm.getId(handle3)), rm.blockList.at(rm.getId(handle2)).forward);
  rm.release(handle3);
  ASSERT_EQ(nullptr, rm.blockList.at(rm.getId(handle2)).forward);
}
