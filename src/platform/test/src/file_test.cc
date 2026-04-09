#include "uinta/file.h"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <vector>

#include "uinta/dirs.h"
#include "uinta/status.h"

namespace uinta {
namespace {

class FileTestF : public ::testing::Test {
 protected:
  std::filesystem::path testDir_;
  std::filesystem::path testFile_;

  void SetUp() override {
    testDir_ = GetDir<DirType::RuntimeDir>().value_or("") / "uinta_file_test";
    std::filesystem::create_directories(testDir_);
    testFile_ = testDir_ / "test_file.txt";
  }

  void TearDown() override {
    std::error_code ec;
    std::filesystem::remove_all(testDir_, ec);
  }

  static void WriteTestFile(const std::filesystem::path& path,
                            const std::string& content) {
    std::ofstream file(path, std::ios::out | std::ios::binary);
    file << content;
  }

  static void WriteTestFileBinary(const std::filesystem::path& path,
                                  const std::vector<u8>& content) {
    std::ofstream file(path, std::ios::out | std::ios::binary);
    file.write(reinterpret_cast<const char*>(content.data()),
               static_cast<std::streamsize>(content.size()));
  }
};

// FileSearchDirs tests

TEST_F(FileTestF, FileSearchDirsReturnsNonEmpty) {
  auto dirs = FileSearchDirs();
  // Should have at least one search path configured
  EXPECT_FALSE(dirs.empty());
}

TEST_F(FileTestF, FileSearchDirsIsConsistent) {
  auto dirs1 = FileSearchDirs();
  auto dirs2 = FileSearchDirs();
  EXPECT_EQ(dirs1, dirs2);
}

// FindFile tests

TEST_F(FileTestF, FindFileReturnsNulloptForEmptyPath) {
  auto result = FindFile("");
  EXPECT_FALSE(result.has_value());
}

TEST_F(FileTestF, FindFileReturnsNulloptForNonExistent) {
  auto result = FindFile("/nonexistent/path/to/file.txt");
  EXPECT_FALSE(result.has_value());
}

TEST_F(FileTestF, FindFileReturnsAbsolutePathForExistingFile) {
  WriteTestFile(testFile_, "test content");

  auto result = FindFile(testFile_);

  ASSERT_TRUE(result.has_value());
  EXPECT_TRUE(result->is_absolute());
  EXPECT_TRUE(std::filesystem::exists(*result));
}

// ReadFile tests

TEST_F(FileTestF, ReadFileReturnsNotFoundForNonExistent) {
  std::string content;
  auto status = ReadFile("/nonexistent/file.txt", content);

  EXPECT_FALSE(status.ok());
  EXPECT_TRUE(IsNotFound(status));
}

TEST_F(FileTestF, ReadFileReadsTextContent) {
  const std::string expected = "Hello, World!\nLine 2";
  WriteTestFile(testFile_, expected);

  std::string content;
  auto status = ReadFile(testFile_, content);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(content, expected);
}

TEST_F(FileTestF, ReadFileReadsEmptyFile) {
  WriteTestFile(testFile_, "");

  std::string content;
  auto status = ReadFile(testFile_, content);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(content.empty());
}

TEST_F(FileTestF, ReadFilePreservesExactContent) {
  const std::string expected = "Line1\r\nLine2\nLine3\rLine4";
  WriteTestFile(testFile_, expected);

  std::string content;
  auto status = ReadFile(testFile_, content);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(content, expected);
}

// ReadFileBinary tests

TEST_F(FileTestF, ReadFileBinaryReturnsNotFoundForNonExistent) {
  std::vector<u8> content;
  auto status = ReadFileBinary("/nonexistent/file.bin", content);

  EXPECT_FALSE(status.ok());
  EXPECT_TRUE(IsNotFound(status));
}

TEST_F(FileTestF, ReadFileBinaryReadsBinaryContent) {
  const std::vector<u8> expected = {0x00, 0x01, 0x02, 0xFF, 0xFE, 0x00, 0x10};
  WriteTestFileBinary(testFile_, expected);

  std::vector<u8> content;
  auto status = ReadFileBinary(testFile_, content);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(content, expected);
}

TEST_F(FileTestF, ReadFileBinaryReadsEmptyFile) {
  WriteTestFileBinary(testFile_, {});

  std::vector<u8> content;
  auto status = ReadFileBinary(testFile_, content);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(content.empty());
}

// ReadFileLines tests

TEST_F(FileTestF, ReadFileLinesReturnsNotFoundForNonExistent) {
  std::vector<std::string> lines;
  auto status = ReadFileLines("/nonexistent/file.txt", lines);

  EXPECT_FALSE(status.ok());
  EXPECT_TRUE(IsNotFound(status));
}

TEST_F(FileTestF, ReadFileLinesReadsMultipleLines) {
  WriteTestFile(testFile_, "Line 1\nLine 2\nLine 3");

  std::vector<std::string> lines;
  auto status = ReadFileLines(testFile_, lines);

  EXPECT_TRUE(status.ok());
  ASSERT_EQ(lines.size(), 3);
  EXPECT_EQ(lines[0], "Line 1");
  EXPECT_EQ(lines[1], "Line 2");
  EXPECT_EQ(lines[2], "Line 3");
}

TEST_F(FileTestF, ReadFileLinesReadsEmptyFile) {
  WriteTestFile(testFile_, "");

  std::vector<std::string> lines;
  auto status = ReadFileLines(testFile_, lines);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(lines.empty());
}

TEST_F(FileTestF, ReadFileLinesHandlesTrailingNewline) {
  WriteTestFile(testFile_, "Line 1\nLine 2\n");

  std::vector<std::string> lines;
  auto status = ReadFileLines(testFile_, lines);

  EXPECT_TRUE(status.ok());
  ASSERT_EQ(lines.size(), 2);
  EXPECT_EQ(lines[0], "Line 1");
  EXPECT_EQ(lines[1], "Line 2");
}

TEST_F(FileTestF, ReadFileLinesHandlesSingleLine) {
  WriteTestFile(testFile_, "Single line without newline");

  std::vector<std::string> lines;
  auto status = ReadFileLines(testFile_, lines);

  EXPECT_TRUE(status.ok());
  ASSERT_EQ(lines.size(), 1);
  EXPECT_EQ(lines[0], "Single line without newline");
}

// CopyFile tests

TEST_F(FileTestF, CopyFileCopiesToNewLocation) {
  const std::string content = "File to copy";
  WriteTestFile(testFile_, content);
  auto dst = testDir_ / "copied_file.txt";

  auto status = CopyFile(testFile_, dst);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(std::filesystem::exists(dst));

  std::string copiedContent;
  (void)ReadFile(dst, copiedContent);
  EXPECT_EQ(copiedContent, content);
}

TEST_F(FileTestF, CopyFileCreatesParentDirectories) {
  const std::string content = "File to copy";
  WriteTestFile(testFile_, content);
  auto dst = testDir_ / "new_dir" / "nested" / "copied_file.txt";

  auto status = CopyFile(testFile_, dst);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(std::filesystem::exists(dst));
}

TEST_F(FileTestF, CopyFileOverwritesExisting) {
  const std::string original = "Original content";
  const std::string newContent = "New content";

  auto dst = testDir_ / "existing_file.txt";
  WriteTestFile(dst, original);
  WriteTestFile(testFile_, newContent);

  auto status = CopyFile(testFile_, dst);

  EXPECT_TRUE(status.ok());
  std::string copiedContent;
  (void)ReadFile(dst, copiedContent);
  EXPECT_EQ(copiedContent, newContent);
}

TEST_F(FileTestF, CopyFileReturnsErrorForNonExistentSource) {
  auto status =
      CopyFile("/nonexistent/source.txt", testDir_ / "destination.txt");

  EXPECT_FALSE(status.ok());
}

// FileReader tests

class FileReaderTestF : public FileTestF {};

TEST_F(FileReaderTestF, OpenReturnsNotFoundForNonExistent) {
  FileReader reader;
  auto status = FileReader::Open("/nonexistent/file.txt", reader);

  EXPECT_FALSE(status.ok());
  EXPECT_TRUE(IsNotFound(status));
}

TEST_F(FileReaderTestF, OpenSucceedsForExistingFile) {
  WriteTestFile(testFile_, "content");

  FileReader reader;
  auto status = FileReader::Open(testFile_, reader);

  EXPECT_TRUE(status.ok());
  EXPECT_TRUE(reader.is_open());
  EXPECT_EQ(reader.path(), testFile_);
}

TEST_F(FileReaderTestF, ReadLineReadsLinesSequentially) {
  WriteTestFile(testFile_, "Line 1\nLine 2\nLine 3");

  FileReader reader;
  ASSERT_TRUE(FileReader::Open(testFile_, reader).ok());

  std::optional<std::string> line;

  EXPECT_TRUE(reader.ReadLine(line).ok());
  ASSERT_TRUE(line.has_value());
  EXPECT_EQ(*line, "Line 1");

  EXPECT_TRUE(reader.ReadLine(line).ok());
  ASSERT_TRUE(line.has_value());
  EXPECT_EQ(*line, "Line 2");

  EXPECT_TRUE(reader.ReadLine(line).ok());
  ASSERT_TRUE(line.has_value());
  EXPECT_EQ(*line, "Line 3");

  EXPECT_TRUE(reader.ReadLine(line).ok());
  EXPECT_FALSE(line.has_value());  // EOF
}

TEST_F(FileReaderTestF, ReadLineReturnsNulloptOnEof) {
  WriteTestFile(testFile_, "");

  FileReader reader;
  ASSERT_TRUE(FileReader::Open(testFile_, reader).ok());

  std::optional<std::string> line;
  auto status = reader.ReadLine(line);

  EXPECT_TRUE(status.ok());
  EXPECT_FALSE(line.has_value());
}

TEST_F(FileReaderTestF, ReadAllLinesReadsRemainingLines) {
  WriteTestFile(testFile_, "Line 1\nLine 2\nLine 3");

  FileReader reader;
  ASSERT_TRUE(FileReader::Open(testFile_, reader).ok());

  // Read first line
  std::optional<std::string> firstLine;
  (void)reader.ReadLine(firstLine);

  // Read remaining lines
  std::vector<std::string> lines;
  auto status = reader.ReadAllLines(lines);

  EXPECT_TRUE(status.ok());
  ASSERT_EQ(lines.size(), 2);
  EXPECT_EQ(lines[0], "Line 2");
  EXPECT_EQ(lines[1], "Line 3");
}

TEST_F(FileReaderTestF, ReadAllReadsRemainingContent) {
  const std::string content = "Line 1\nLine 2\nLine 3";
  WriteTestFile(testFile_, content);

  FileReader reader;
  ASSERT_TRUE(FileReader::Open(testFile_, reader).ok());

  std::string result;
  auto status = reader.ReadAll(result);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(result, content);
}

TEST_F(FileReaderTestF, ReadAllBinaryReadsRemainingBinaryContent) {
  const std::vector<u8> content = {0x00, 0x01, 0x02, 0xFF, 0xFE};
  WriteTestFileBinary(testFile_, content);

  FileReader reader;
  ASSERT_TRUE(FileReader::Open(testFile_, reader).ok());

  std::vector<u8> result;
  auto status = reader.ReadAllBinary(result);

  EXPECT_TRUE(status.ok());
  EXPECT_EQ(result, content);
}

TEST_F(FileReaderTestF, MoveConstructorTransfersOwnership) {
  WriteTestFile(testFile_, "content");

  FileReader reader1;
  ASSERT_TRUE(FileReader::Open(testFile_, reader1).ok());

  FileReader reader2(std::move(reader1));

  EXPECT_TRUE(reader2.is_open());
  EXPECT_EQ(reader2.path(), testFile_);
}

TEST_F(FileReaderTestF, MoveAssignmentTransfersOwnership) {
  WriteTestFile(testFile_, "content");

  FileReader reader1;
  ASSERT_TRUE(FileReader::Open(testFile_, reader1).ok());

  FileReader reader2;
  reader2 = std::move(reader1);

  EXPECT_TRUE(reader2.is_open());
  EXPECT_EQ(reader2.path(), testFile_);
}

}  // namespace
}  // namespace uinta
