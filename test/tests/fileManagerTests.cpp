#include <gtest/gtest.h>

#include "../../../src/io/utils.hpp"  // FIXME there has to be a better way to include this

#define private public
#include <uinta/io.hpp>

using namespace uinta;

// TEST(FileManager_releaseFile, pathsClear) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("test", FileType::Text);
//   ASSERT_EQ("test", fm.getPath(handle));
//   fm.releaseFile(handle);
//   ASSERT_EQ("", fm.getPath(handle));
// }
//
// TEST(FileManager_releaseFile, isBuffered_false) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("test", FileType::Text);
//   fm.setIsBuffered(handle, true);
//   ASSERT_TRUE(fm.isBuffered(handle));
//   fm.releaseFile(handle);
//   ASSERT_FALSE(fm.isBuffered(handle));
// }
//
// TEST(FileManager_releaseFile, links_reset) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("test", FileType::Text);
//   fm.links.at(fm.getId(handle)) = MemoryLink(nullptr, 1);
//   ASSERT_EQ(1, fm.links.at(fm.getId(handle)).size);
//   fm.releaseFile(handle);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
// }
//
// TEST(FileManager_releaseFile, isActive_false) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("test", FileType::Text);
//   ASSERT_TRUE(fm.isActive(handle));
//   fm.releaseFile(handle);
//   ASSERT_FALSE(fm.isActive(handle));
// }
//
// TEST(FileManager_registerFile, addHandle) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("path", FileType::Text);
//   ASSERT_EQ(handle, fm.handles.at(fm.getId(handle)));
// }
//
// TEST(FileManager_registerFile, handlePath) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("path", FileType::Text);
//   ASSERT_EQ("path", fm.handlePaths.at(fm.getId(handle)));
// }
//
// TEST(FileManager_registerFile, links) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("path", FileType::Text);
//   ASSERT_EQ(1, fm.links.size());
//   ASSERT_EQ(MemoryLink(), fm.links.at(fm.getId(handle)));
// }
//
// TEST(FileManager_registerFile, active) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("path", FileType::Text);
//   ASSERT_TRUE(fm.isActive(handle));
// }
//
// TEST(FileManager_registerFile, type) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("path", FileType::Text);
//   ASSERT_EQ(FileType::Text, fm.getType(handle));
// }
//
// TEST(FileManager_getSize, happyPath) {
//   FileManager fm(KILOBYTES(1));
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
//   fm.loadAll();
//   ASSERT_EQ(77, fm.getSize(handle));
// }
//
// TEST(FileManager_getSize, loadedButInactive) {
//   FileManager fm(KILOBYTES(1));
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
//   fm.loadAll();
//   ASSERT_EQ(77, fm.getSize(handle));
//   fm.releaseFile(handle);
//   ASSERT_EQ(0, fm.getSize(handle));
// }
//
// #include <fstream>
// TEST(FileManager_findPath, absolutePath) {
//   if (!std::ifstream("/dev/null")) return;
//   ASSERT_EQ("/dev/null", findPath("/dev/null", {}));
// }
//
// TEST(FileManager_findPath, relativePath) {
//   FileManager fm(0);
//   fm.init();
//   ASSERT_TRUE(findPath("res_man1.txt", fm.fileSearchPaths).ends_with("/test/res/res_man1.txt"));
// }
//
// TEST(FileManager_findPath, invalid) {
//   FileManager fm(0);
//   fm.init();
//   ASSERT_EQ("", findPath("foo/bar.baz", fm.fileSearchPaths));
// }
//
// TEST(FileManager_loadAllFiles, invalidPathreleaseFiles) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("foo/bar.baz", FileType::Text);
//   ASSERT_TRUE(fm.isActive(handle));
//   fm.loadAll();
//   ASSERT_FALSE(fm.isActive(handle));
// }
//
// TEST(FileManager_loadAllFiles, pathIsSet) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   ASSERT_EQ("res_man1.txt", fm.getPath(handle));
//   fm.loadAll();
//   ASSERT_TRUE(fm.getPath(handle).ends_with("/test/res/res_man1.txt"));
// }
//
// TEST(FileManager_loadAllFiles, spaceReserved) {
//   FileManager fm(100);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle)).ptr);
//   fm.loadAll();
//   ASSERT_NE(nullptr, fm.links.at(fm.getId(handle)).ptr);
// }
//
// TEST(FileManager_loadAllFiles, loadsData) {
//   FileManager fm(100);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle)).ptr);
//   fm.loadAll();
//   ASSERT_NE(nullptr, fm.links.at(fm.getId(handle)).ptr);
//   std::string data = fm.getDataChars(handle);
//   ASSERT_TRUE(data.starts_with("File 1"));
// }
//
// TEST(FileManager_reserveSpace, blockResets) {
//   FileManager fm(0);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   fm.setIsActive(handle, false);
//   fm.links.at(fm.getId(handle)) = MemoryLink(nullptr, 1);
//   ASSERT_EQ(1, fm.links.at(fm.getId(handle)).size);
//   fm.reserveSpace(handle);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
// }
//
// TEST(FileManager_reserveSpace, reserveMoreThanAvailable) {
//   FileManager fm(1);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
// }
//
// TEST(FileManager_reserveSpace, firstItem) {
//   FileManager fm(100);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
//   fm.loadAll();
//   ASSERT_EQ(77, fm.links.at(fm.getId(handle)).size);
//   ASSERT_EQ(fm.storage, fm.links.at(fm.getId(handle)).ptr);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle)).back);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle)).forward);
// }
//
// TEST(FileManager_reserveSpace, secondItem) {
//   FileManager fm(200);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   fm.loadAll();
//   auto handle1Size = fm.links.at(fm.getId(handle1)).size;
//   ASSERT_EQ((char*)fm.storage + handle1Size + 1, fm.links.at(fm.getId(handle2)).ptr);
//   ASSERT_EQ(&fm.links.at(fm.getId(handle1)), fm.links.at(fm.getId(handle2)).back);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle2)).forward);
// }
//
// TEST(FileManager_reserveSpace, thirdItem) {
//   FileManager fm(300);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   auto handle1Size = fm.links.at(fm.getId(handle1)).size;
//   auto handle2Size = fm.links.at(fm.getId(handle1)).size;
//
//   ASSERT_EQ((char*)fm.storage + handle1Size + handle2Size + 2, fm.links.at(fm.getId(handle3)).ptr);
//
//   ASSERT_EQ(&fm.links.at(fm.getId(handle1)), fm.links.at(fm.getId(handle2)).back);
//   ASSERT_EQ(&fm.links.at(fm.getId(handle3)), fm.links.at(fm.getId(handle2)).forward);
//
//   ASSERT_EQ(&fm.links.at(fm.getId(handle2)), fm.links.at(fm.getId(handle3)).back);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle3)).forward);
// }
//
// TEST(FileManager_reserveSpace, noSpaceLeft) {
//   FileManager fm(100);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_NE(nullptr, fm.links.at(fm.getId(handle1)).ptr);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle2)).ptr);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle2)).size);
// }
//
// TEST(FileManager_reserveSpace, orphaned_headspace) {
//   FileManager fm(200);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   fm.loadAll();
//   fm.releaseFile(handle1);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_EQ((char*)fm.storage, fm.links.at(fm.getId(handle3)).ptr);
// }
//
// TEST(FileManager_reserveSpace, orphaned_middle) {
//   FileManager fm(300);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   fm.releaseFile(handle2);
//   auto* const handle4 = fm.registerFile("res_man4.txt", FileType::Text);
//   fm.loadAll();
//   auto& block1 = fm.links.at(fm.getId(handle1));
//   ASSERT_EQ((char*)block1.ptr + block1.size + 1, fm.links.at(fm.getId(handle4)).ptr);
// }
//
// TEST(FileManager_reserveSpace, orphaned_tail) {
//   FileManager fm(200);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   fm.loadAll();
//   fm.releaseFile(handle2);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   auto& block1 = fm.links.at(fm.getId(handle1));
//   ASSERT_EQ((char*)block1.ptr + block1.size + 1, fm.links.at(fm.getId(handle3)).ptr);
// }
//
// TEST(FileManager_releaseFile, resetsBlock) {
//   FileManager fm(300);
//   fm.init();
//   auto* const handle = fm.registerFile("res_man1.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_EQ(77, fm.links.at(fm.getId(handle)).size);
//   fm.releaseFile(handle);
//   ASSERT_EQ(0, fm.links.at(fm.getId(handle)).size);
// }
//
// TEST(FileManager_releaseFile, releaseFile_head) {
//   FileManager fm(300);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_EQ(&fm.links.at(fm.getId(handle1)), fm.links.at(fm.getId(handle2)).back);
//   fm.releaseFile(handle1);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle2)).back);
// }
//
// TEST(FileManager_releaseFile, releaseFile_middle) {
//   FileManager fm(300);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_EQ(&fm.links.at(fm.getId(handle2)), fm.links.at(fm.getId(handle1)).forward);
//   ASSERT_EQ(&fm.links.at(fm.getId(handle2)), fm.links.at(fm.getId(handle3)).back);
//   fm.releaseFile(handle2);
//   ASSERT_EQ(&fm.links.at(fm.getId(handle3)), fm.links.at(fm.getId(handle1)).forward);
//   ASSERT_EQ(&fm.links.at(fm.getId(handle1)), fm.links.at(fm.getId(handle3)).back);
// }
//
// TEST(FileManager_releaseFile, releaseFile_tail) {
//   FileManager fm(300);
//   fm.init();
//   auto* const handle1 = fm.registerFile("res_man1.txt", FileType::Text);
//   auto* const handle2 = fm.registerFile("res_man2.txt", FileType::Text);
//   auto* const handle3 = fm.registerFile("res_man3.txt", FileType::Text);
//   fm.loadAll();
//   ASSERT_EQ(&fm.links.at(fm.getId(handle3)), fm.links.at(fm.getId(handle2)).forward);
//   fm.releaseFile(handle3);
//   ASSERT_EQ(nullptr, fm.links.at(fm.getId(handle2)).forward);
// }
