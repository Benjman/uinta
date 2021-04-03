#ifndef UINTA_ARR_UTILS_H
#define UINTA_ARR_UTILS_H

namespace uinta {

	template<typename T>
	void shiftArr(T shift, T *arr, size_t count) {
		while (count > 0) {
			arr[count - 1] += shift;
			count--;
		}
	}

} // namespace uinta

#endif //UINTA_ARR_UTILS_H
