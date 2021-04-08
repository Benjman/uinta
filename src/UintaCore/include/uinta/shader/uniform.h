#ifndef UINTA_UNIFORM_H
#define UINTA_UNIFORM_H

#include <uinta/types.h>
#include <uinta/gl/gl_types.h>

#include <glm/glm.hpp>

namespace uinta {

	static const size_t INVALID_UNIFORM_ID = 0;

	class IUniform {
	protected:
		bool _loaded = false;
		const char *_name;

		GLuint _id = INVALID_UNIFORM_ID;

	public:
		explicit IUniform(const char *name) : _name(name) { }

	};

	template<typename T>
	class Uniform : public IUniform {
	protected:
		T _value;

		explicit Uniform(const char *name) : IUniform(name) {}

		void load(T value) {
			if (_loaded && _value == value) return;
			_value = value;
			doLoad();
		}

		virtual void doLoad() = 0;

	};

	template<typename T>
	class Uniformv : public Uniform<T> {
	protected:
		size_t _count = 0;

		Uniformv(const char *name, size_t count) : Uniform<T>(name), _count(count) {}

	}; // class Uniformv

	class Uniform1f : public Uniform<GLfloat> {
	protected:
		void doLoad() override {
			glUniform1f(_id, 1.f);
		}

	}; // class Uniform1f

	class Uniform2f : public Uniform<glm::vec2> {
	protected:
		void doLoad() override {
			glUniform2f(_id, _value.x, _value.y);
		}

	}; // class Uniform2f

	class Uniform3f : public Uniform<glm::vec3> {
	protected:
		void doLoad() override {
			glUniform3f(_id, _value.x, _value.y, _value.z);
		}

	}; // class Uniform3f

	class Uniform4f : public Uniform<glm::vec4> {
	protected:
		void doLoad() override {
			glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
		}

	}; // class Uniform4f

	class Uniform1i : public Uniform<int32_t> {
	protected:
		void doLoad() override {
			glUniform1i(_id, _value);
		}

	}; // class Uniform1i

	class Uniform2i : public Uniform<glm::ivec2> {
	protected:
		void doLoad() override {
			glUniform2i(_id, _value.x, _value.y);
		}

	}; // class Uniform2i

	class Uniform3i : public Uniform<glm::ivec3> {
	protected:
		void doLoad() override {
			glUniform3i(_id, _value.x, _value.y, _value.z);
		}

	}; // class Uniform3i

	class Uniform4i : public Uniform<glm::ivec4> {
	protected:
		void doLoad() override {
			glUniform4i(_id, _value.x, _value.y, _value.z, _value.w);
		}

	}; // class Uniform4i

	class Uniform1ui : public Uniform<uint32_t> {
	protected:
		void doLoad() override {
			glUniform1ui(_id, _value);
		}

	}; // class Uniform1ui

	class Uniform2ui : public Uniform<glm::uvec2> {
	protected:
		void doLoad() override {
			glUniform2ui(_id, _value.x, _value.y);
		}

	}; // class Uniform2ui

	class Uniform3ui : public Uniform<glm::uvec3> {
	protected:
		void doLoad() override {
			glUniform3ui(_id, _value.x, _value.y, _value.z);
		}

	}; // class Uniform3ui

	class Uniform4ui : public Uniform<glm::uvec4> {
	protected:
		void doLoad() override {
			glUniform4ui(_id, _value.x, _value.y, _value.z, _value.w);
		}

	}; // class Uniform4ui

	class Uniform1fv : public Uniformv<GLfloat*> {
	public:
		explicit Uniform1fv(const char *name, size_t count) : Uniformv<GLfloat*>(name, count) {}

	protected:
		void doLoad() override {
			glUniform1fv(_id, _count, _value);
		}

	}; // class Uniform1fv

	class Uniform2fv : public Uniformv<GLfloat*> {
	public:
		explicit Uniform2fv(const char *name, size_t count) : Uniformv<GLfloat*>(name, count) {}

	protected:
		void doLoad() override {
			glUniform2fv(_id, _count, _value);
		}

	}; // class Uniform2fv

	class Uniform3fv : public Uniformv<GLfloat*> {
	public:
		explicit Uniform3fv(const char *name, size_t count) : Uniformv<GLfloat*>(name, count) {}

	protected:
		void doLoad() override {
			glUniform3fv(_id, _count, _value);
		}

	}; // class Uniform3fv

	class Uniform4fv : public Uniformv<GLfloat*> {
	public:
		explicit Uniform4fv(const char *name, size_t count) : Uniformv<GLfloat *>(name, count) {}

	protected:
		void doLoad() override {
			glUniform4fv(_id, _count, _value);
		}

	}; // class Uniform4fv

	class UniformMatrix2fv : public Uniformv<GLfloat*> {
	public:
		explicit UniformMatrix2fv(const char *name, size_t count) : Uniformv<GLfloat *>(name, count) {}

	protected:
		void doLoad() override {
			glUniform4fv(_id, _count, _value);
		}

	}; // class UniformMatrix2fv

	class UniformMatrix3fv : public Uniformv<GLfloat*> {
	public:
		explicit UniformMatrix3fv(const char *name, size_t count) : Uniformv<GLfloat *>(name, count) {}

	protected:
		void doLoad() override {
			glUniform4fv(_id, _count, _value);
		}

	}; // class UniformMatrix3fv

	class UniformMatrix4fv : public Uniformv<GLfloat*> {
	public:
		explicit UniformMatrix4fv(const char *name, size_t count) : Uniformv<GLfloat *>(name, count) {}

	protected:
		void doLoad() override {
			glUniform4fv(_id, _count, _value);
		}

	}; // class UniformMatrix4fv

} // namespace uinta

#endif //UINTA_UNIFORM_H
