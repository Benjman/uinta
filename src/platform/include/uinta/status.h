#ifndef SRC_PLATFORM_INCLUDE_UINTA_STATUS_H_
#define SRC_PLATFORM_INCLUDE_UINTA_STATUS_H_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#include "absl/status/status.h"
#include "absl/status/statusor.h"
#pragma GCC diagnostic pop

namespace uinta {

using absl::AbortedError;
using absl::AlreadyExistsError;
using absl::CancelledError;
using absl::DataLossError;
using absl::DeadlineExceededError;
using absl::FailedPreconditionError;
using absl::InternalError;
using absl::InvalidArgumentError;
using absl::IsAborted;
using absl::IsAlreadyExists;
using absl::IsCancelled;
using absl::IsDataLoss;
using absl::IsDeadlineExceeded;
using absl::IsFailedPrecondition;
using absl::IsInternal;
using absl::IsInvalidArgument;
using absl::IsNotFound;
using absl::IsOutOfRange;
using absl::IsPermissionDenied;
using absl::IsResourceExhausted;
using absl::IsUnauthenticated;
using absl::IsUnavailable;
using absl::IsUnimplemented;
using absl::IsUnknown;
using absl::NotFoundError;
using absl::OkStatus;
using absl::OutOfRangeError;
using absl::PermissionDeniedError;
using absl::ResourceExhaustedError;
using absl::Status;
using absl::StatusCode;
using absl::StatusOr;
using absl::StatusToStringMode;
using absl::UnauthenticatedError;
using absl::UnavailableError;
using absl::UnimplementedError;
using absl::UnknownError;

}  // namespace uinta

#endif  // SRC_PLATFORM_INCLUDE_UINTA_STATUS_H_
