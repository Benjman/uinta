#ifndef SRC_PLATFORM_INCLUDE_UINTA_LIB_ABSL_STRINGS_H_
#define SRC_PLATFORM_INCLUDE_UINTA_LIB_ABSL_STRINGS_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "absl/strings/numbers.h"
#include "absl/strings/str_cat.h"
#include "absl/strings/str_format.h"
#include "absl/strings/str_join.h"
#include "absl/strings/str_split.h"
#include "absl/strings/string_view.h"
#include "absl/strings/substitute.h"
#pragma GCC diagnostic pop

namespace uinta {

using absl::ByAnyChar;
using absl::ByChar;
using absl::ByLength;
using absl::ByString;
using absl::MaxSplits;
using absl::SimpleAtob;
using absl::SimpleAtod;
using absl::SimpleAtof;
using absl::SimpleAtoi;
using absl::SimpleHexAtoi;
using absl::StrAppend;
using absl::StrAppendFormat;
using absl::StrCat;
using absl::StrContains;
using absl::StrContainsIgnoreCase;
using absl::StrFormat;
using absl::string_view;
using absl::StrJoin;
using absl::StrSplit;
using absl::Substitute;
using absl::SubstituteAndAppend;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_LIB_ABSL_STRINGS_H_
