#ifndef UINTA_TIMESTAMP_H
#define UINTA_TIMESTAMP_H

#include <uinta/types.h>

namespace uinta {

	union Timestamp {
		float_t _float;
		int32_t _integer;

		Timestamp() : _integer(0) {}

		explicit Timestamp(float_t value) : _float(0.f) {}

		explicit operator int32_t() const { return _integer; }

		inline bool operator!=(const Timestamp &other) const { return _integer != other._integer; }

		inline bool operator<(const Timestamp &other) const { return _float < other._float; }

		inline bool operator==(const Timestamp &other) const { return _integer == other._integer; }

		inline bool operator>(const Timestamp &other) const { return _float > other._float; }

	}; // class Timestamp

} // namespace uinta


#endif //UINTA_TIMESTAMP_H
