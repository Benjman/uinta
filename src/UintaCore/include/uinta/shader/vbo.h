#ifndef UINTA_VBO_H
#define UINTA_VBO_H

#include <uinta/gl/gl_types.h>

// TODO release vaos

namespace uinta {

	class Vbo {
		vbo_id_t _id = INVALID_VBO_ID;
		vbo_target_t _target;
		vbo_usage_t _usage;
		vbo_size_t _size;

	public:
		static Vbo requestVbo(vbo_target_t target, vbo_usage_t usage) { return requestVbo(target, usage, 0); }

		static Vbo requestVbo(vbo_target_t target, vbo_usage_t usage, vbo_size_t size) { return requestVbo(target, usage, size, nullptr); }

		static Vbo requestVbo(vbo_target_t target, vbo_usage_t usage, vbo_size_t size, const void *data);

		static void unbind(vbo_target_t target);

	public:
		void bind() const;

		void storeData(const void *data, vbo_size_t size) { storeData(data, size, 0); }

		void storeData(const void *data, vbo_size_t size, vbo_size_t offset);

		void unbind() const;

		~Vbo();

	private:
		Vbo(vbo_target_t target, vbo_usage_t usage) : Vbo(target, usage, INVALID_VBO_ID, 0) {}

		explicit Vbo(vbo_target_t target, vbo_usage_t usage, vbo_id_t id, vbo_size_t size) : _target(target), _usage(usage), _id(id), _size(size) {}

		void resize(vbo_size_t size);

	};

}

#endif //UINTA_VBO_H
