#include <gtest/gtest.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "../../../src/io/utils.hpp"  // FIXME there has to be a better way to include this

#define private public
#include "uinta/io.hpp"

using namespace uinta;

static inline logger_t logger = spdlog::stdout_color_mt("FileManagerTest");

TEST(ResourceManager_parsePaths, happyPath) {
  auto input = "/my/user/path;/second/user/path/";
  std::vector<std::string> buffer;
  parseFileSearchPaths(input, ';', buffer);
  ASSERT_EQ(2, buffer.size());
  ASSERT_EQ("/my/user/path/", buffer.at(0));
  ASSERT_EQ("/second/user/path/", buffer.at(1));
}

TEST(ResourceManager_parsePaths, emptyInput) {
  auto input = "";
  std::vector<std::string> buffer;
  parseFileSearchPaths(input, ';', buffer);
  ASSERT_EQ(0, buffer.size());
}

TEST(ResourceManager_parsePaths, singlePath) {
  auto input = "/my/user/path/";
  std::vector<std::string> buffer;
  parseFileSearchPaths(input, ';', buffer);
  ASSERT_EQ(1, buffer.size());
  ASSERT_EQ("/my/user/path/", buffer.at(0));
}

TEST(ResourceManager_parsePaths, semicolon_only) {
  auto input = ";";
  std::vector<std::string> buffer;
  parseFileSearchPaths(input, ';', buffer);
  ASSERT_EQ(0, buffer.size());
}

TEST(ResourceManager_parsePaths, buffer_clears) {
  auto input = ";";
  std::vector<std::string> buffer = {"Hello"};
  parseFileSearchPaths(input, ';', buffer);
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
  parseFileSearchPaths(input.c_str(), ';', buffer);
  ASSERT_EQ(pathCount, buffer.size());
  for (int i = 0; i < pathCount; i++) ASSERT_EQ(std::string("/path/to/folder/" + std::to_string(i)) + "/", buffer.at(i));
}

TEST(ResourceManager_parsePaths, delimiter) {
  ASSERT_EQ(';', UINTA_FILE_SEARCH_PATHS_DELIM) << "Delimiter is expected to match cmake's semicolon concatenation.";
}

TEST(ResourceManager_releaseFile, pathsClear) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", FileType::Text);
  ASSERT_EQ("test", rm.getPath(handle));
  rm.releaseFile(handle);
  ASSERT_EQ("", rm.getPath(handle));
}

TEST(ResourceManager_releaseFile, isBuffered_false) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", FileType::Text);
  rm.setIsBuffered(handle, true);
  ASSERT_TRUE(rm.isBuffered(handle));
  rm.releaseFile(handle);
  ASSERT_FALSE(rm.isBuffered(handle));
}

TEST(ResourceManager_releaseFile, links_reset) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", FileType::Text);
  rm.links.at(rm.getId(handle)) = MemoryLink(nullptr, 1);
  ASSERT_EQ(1, rm.links.at(rm.getId(handle)).size);
  rm.releaseFile(handle);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
}

TEST(ResourceManager_releaseFile, isActive_false) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("test", FileType::Text);
  ASSERT_TRUE(rm.isActive(handle));
  rm.releaseFile(handle);
  ASSERT_FALSE(rm.isActive(handle));
}

TEST(ResourceManager_registerFile, addHandle) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", FileType::Text);
  ASSERT_EQ(handle, rm.handles.at(rm.getId(handle)));
}

TEST(ResourceManager_registerFile, handlePath) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", FileType::Text);
  ASSERT_EQ("path", rm.handlePaths.at(rm.getId(handle)));
}

TEST(ResourceManager_registerFile, links) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", FileType::Text);
  ASSERT_EQ(1, rm.links.size());
  ASSERT_EQ(MemoryLink(), rm.links.at(rm.getId(handle)));
}

TEST(ResourceManager_registerFile, active) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", FileType::Text);
  ASSERT_TRUE(rm.isActive(handle));
}

TEST(ResourceManager_registerFile, type) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("path", FileType::Text);
  ASSERT_EQ(FileType::Text, rm.getType(handle));
}

TEST(ResourceManager_getSize, happyPath) {
  FileManager rm(KILOBYTES(1));
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
  rm.loadAll();
  ASSERT_EQ(77, rm.getSize(handle));
}

TEST(ResourceManager_getSize, loadedButInactive) {
  FileManager rm(KILOBYTES(1));
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
  rm.loadAll();
  ASSERT_EQ(77, rm.getSize(handle));
  rm.releaseFile(handle);
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
  ASSERT_TRUE(findPath("res_man1.txt", rm.fileSearchPaths).ends_with("/test/res/res_man1.txt"));
}

TEST(ResourceManager_findPath, invalid) {
  FileManager rm(0);
  rm.init();
  ASSERT_EQ("", findPath("foo/bar.baz", rm.fileSearchPaths));
}

TEST(ResourceManager_loadAllFiles, invalidPathreleaseFiles) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("foo/bar.baz", FileType::Text);
  ASSERT_TRUE(rm.isActive(handle));
  rm.loadAll();
  ASSERT_FALSE(rm.isActive(handle));
}

TEST(ResourceManager_loadAllFiles, pathIsSet) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  ASSERT_EQ("res_man1.txt", rm.getPath(handle));
  rm.loadAll();
  ASSERT_TRUE(rm.getPath(handle).ends_with("/test/res/res_man1.txt"));
}

TEST(ResourceManager_loadAllFiles, spaceReserved) {
  FileManager rm(100);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle)).ptr);
  rm.loadAll();
  ASSERT_NE(nullptr, rm.links.at(rm.getId(handle)).ptr);
}

TEST(ResourceManager_loadAllFiles, loadsData) {
  FileManager rm(100);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle)).ptr);
  rm.loadAll();
  ASSERT_NE(nullptr, rm.links.at(rm.getId(handle)).ptr);
  std::string data = rm.getDataChars(handle);
  ASSERT_TRUE(data.starts_with("File 1"));
}

TEST(ResourceManager_reserveSpace, blockResets) {
  FileManager rm(0);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  rm.setIsActive(handle, false);
  rm.links.at(rm.getId(handle)) = MemoryLink(nullptr, 1);
  ASSERT_EQ(1, rm.links.at(rm.getId(handle)).size);
  rm.reserveSpace(handle);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
}

TEST(ResourceManager_reserveSpace, reserveMoreThanAvailable) {
  FileManager rm(1);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  rm.loadAll();
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
}

TEST(ResourceManager_reserveSpace, firstItem) {
  FileManager rm(100);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
  rm.loadAll();
  ASSERT_EQ(77, rm.links.at(rm.getId(handle)).size);
  ASSERT_EQ(rm.storage, rm.links.at(rm.getId(handle)).ptr);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle)).back);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle)).forward);
}

TEST(ResourceManager_reserveSpace, secondItem) {
  FileManager rm(200);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  rm.loadAll();
  auto handle1Size = rm.links.at(rm.getId(handle1)).size;
  ASSERT_EQ((char*)rm.storage + handle1Size + 1, rm.links.at(rm.getId(handle2)).ptr);
  ASSERT_EQ(&rm.links.at(rm.getId(handle1)), rm.links.at(rm.getId(handle2)).back);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle2)).forward);
}

TEST(ResourceManager_reserveSpace, thirdItem) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  auto handle1Size = rm.links.at(rm.getId(handle1)).size;
  auto handle2Size = rm.links.at(rm.getId(handle1)).size;

  ASSERT_EQ((char*)rm.storage + handle1Size + handle2Size + 2, rm.links.at(rm.getId(handle3)).ptr);

  ASSERT_EQ(&rm.links.at(rm.getId(handle1)), rm.links.at(rm.getId(handle2)).back);
  ASSERT_EQ(&rm.links.at(rm.getId(handle3)), rm.links.at(rm.getId(handle2)).forward);

  ASSERT_EQ(&rm.links.at(rm.getId(handle2)), rm.links.at(rm.getId(handle3)).back);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle3)).forward);
}

TEST(ResourceManager_reserveSpace, noSpaceLeft) {
  FileManager rm(100);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  rm.loadAll();
  ASSERT_NE(nullptr, rm.links.at(rm.getId(handle1)).ptr);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle2)).ptr);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle2)).size);
}

TEST(ResourceManager_reserveSpace, orphaned_headspace) {
  FileManager rm(200);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  rm.loadAll();
  rm.releaseFile(handle1);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  ASSERT_EQ((char*)rm.storage, rm.links.at(rm.getId(handle3)).ptr);
}

TEST(ResourceManager_reserveSpace, orphaned_middle) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  rm.releaseFile(handle2);
  auto* handle4 = rm.registerFile("res_man4.txt", FileType::Text);
  rm.loadAll();
  auto& block1 = rm.links.at(rm.getId(handle1));
  ASSERT_EQ((char*)block1.ptr + block1.size + 1, rm.links.at(rm.getId(handle4)).ptr);
}

TEST(ResourceManager_reserveSpace, orphaned_tail) {
  FileManager rm(200);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  rm.loadAll();
  rm.releaseFile(handle2);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  auto& block1 = rm.links.at(rm.getId(handle1));
  ASSERT_EQ((char*)block1.ptr + block1.size + 1, rm.links.at(rm.getId(handle3)).ptr);
}

TEST(ResourceManager_releaseFile, resetsBlock) {
  FileManager rm(300);
  rm.init();
  auto* handle = rm.registerFile("res_man1.txt", FileType::Text);
  rm.loadAll();
  ASSERT_EQ(77, rm.links.at(rm.getId(handle)).size);
  rm.releaseFile(handle);
  ASSERT_EQ(0, rm.links.at(rm.getId(handle)).size);
}

TEST(ResourceManager_releaseFile, releaseFile_head) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  ASSERT_EQ(&rm.links.at(rm.getId(handle1)), rm.links.at(rm.getId(handle2)).back);
  rm.releaseFile(handle1);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle2)).back);
}

TEST(ResourceManager_releaseFile, releaseFile_middle) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  ASSERT_EQ(&rm.links.at(rm.getId(handle2)), rm.links.at(rm.getId(handle1)).forward);
  ASSERT_EQ(&rm.links.at(rm.getId(handle2)), rm.links.at(rm.getId(handle3)).back);
  rm.releaseFile(handle2);
  ASSERT_EQ(&rm.links.at(rm.getId(handle3)), rm.links.at(rm.getId(handle1)).forward);
  ASSERT_EQ(&rm.links.at(rm.getId(handle1)), rm.links.at(rm.getId(handle3)).back);
}

TEST(ResourceManager_releaseFile, releaseFile_tail) {
  FileManager rm(300);
  rm.init();
  auto* handle1 = rm.registerFile("res_man1.txt", FileType::Text);
  auto* handle2 = rm.registerFile("res_man2.txt", FileType::Text);
  auto* handle3 = rm.registerFile("res_man3.txt", FileType::Text);
  rm.loadAll();
  ASSERT_EQ(&rm.links.at(rm.getId(handle3)), rm.links.at(rm.getId(handle2)).forward);
  rm.releaseFile(handle3);
  ASSERT_EQ(nullptr, rm.links.at(rm.getId(handle2)).forward);
}
