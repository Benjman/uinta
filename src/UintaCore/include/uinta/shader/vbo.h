#ifndef UINTA_VBO_H
#define UINTA_VBO_H

#include <uinta/gl/gl_types.h>

namespace uinta {

	class Vbo {
		vbo_id_t _id = INVALID_VBO_ID;
		vbo_target_t _target;
		vbo_usage_t _usage;
		vbo_size_t _size;

	public:
		static Vbo requestVbo(Vao *vao, vbo_target_t target, vbo_usage_t usage, vbo_size_t size = 0, const void *data = nullptr);

		static void unbind(vbo_target_t target);

	public:
		void bind() const;

		void storeData(const void *data, vbo_size_t size, vbo_size_t offset = 0);

		void unbind() const;

		~Vbo();

	private:
		Vbo(vbo_target_t target, vbo_usage_t usage, vbo_size_t size = 0) : _target(target), _usage(usage), _size(size) {}

		void resize(vbo_size_t size, const void *data = nullptr);

	};

}

#endif //UINTA_VBO_H
