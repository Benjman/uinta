#ifndef UINTA_ARR_UTILS_H
#define UINTA_ARR_UTILS_H

namespace uinta {

	template<typename T>
	void arrShift(T *arr, T shift, size_t count) {
		while (count > 0) {
			arr[count - 1] += shift;
			count--;
		}
	}

	void arrShift(GLuint *arr, GLuint shift, size_t count) {
		arrShift<GLuint>(arr, shift, count);
	}

	void arrShift(GLfloat *arr, GLfloat shift, size_t count) {
		arrShift<GLfloat>(arr, shift, count);
	}

} // namespace uinta

#endif //UINTA_ARR_UTILS_H
