#ifndef SRC_PLATFORM_INCLUDE_UINTA_LIB_ABSL_SYNCHRONIZATION_H_
#define SRC_PLATFORM_INCLUDE_UINTA_LIB_ABSL_SYNCHRONIZATION_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "absl/base/call_once.h"
#include "absl/synchronization/barrier.h"
#include "absl/synchronization/mutex.h"
#include "absl/synchronization/notification.h"
#pragma GCC diagnostic pop

namespace uinta {

using absl::Barrier;
using absl::call_once;
using absl::Mutex;
using absl::MutexLock;
using absl::MutexLockMaybe;
using absl::Notification;
using absl::once_flag;
using absl::ReleasableMutexLock;
using absl::WriterMutexLock;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_LIB_ABSL_SYNCHRONIZATION_H_
