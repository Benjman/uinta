#ifndef UINTA_UNIFORM_H
#define UINTA_UNIFORM_H

#include <uinta/types.h>
#include <uinta/gl_types.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace uinta {

	static const GLuint INVALID_UNIFORM_ID = -1;

	class IUniform {
	protected:
		bool _loaded = false;
		const char *_name;

		GLuint _id = INVALID_UNIFORM_ID;

	public:
		explicit IUniform(const char *name) : _name(name) { }

		void storeLocation(GLuint programId);

	};

	template<typename T>
	class Uniform : public IUniform {
	public:
		virtual void load(T value) {
			if (_loaded && _value == value) return;
			_value = value;
			doLoad();
		}

	protected:
		T _value;

		explicit Uniform(const char *name) : IUniform(name) {}

		virtual void doLoad() = 0;

	};

	template<typename T>
	class Uniformv : public IUniform {
	public:
		virtual void load(T value) = 0;

	protected:
		Uniformv(const char *name) : IUniform(name) {}

	}; // class Uniformv

	class Uniform1f : public Uniform<GLfloat> {
	protected:
		void doLoad() override;

	}; // class Uniform1f

	class Uniform2f : public Uniform<glm::vec2> {
	protected:
		void doLoad() override;

	}; // class Uniform2f

	class Uniform3f : public Uniform<glm::vec3> {
	protected:
		void doLoad() override;

	}; // class Uniform3f

	class Uniform4f : public Uniform<glm::vec4> {
	protected:
		void doLoad() override;

	}; // class Uniform4f

	class Uniform1i : public Uniform<int32_t> {
	protected:
		void doLoad() override;

	}; // class Uniform1i

	class Uniform2i : public Uniform<glm::ivec2> {
	protected:
		void doLoad() override;

	}; // class Uniform2i

	class Uniform3i : public Uniform<glm::ivec3> {
	protected:
		void doLoad() override;

	}; // class Uniform3i

	class Uniform4i : public Uniform<glm::ivec4> {
	protected:
		void doLoad() override;

	}; // class Uniform4i

	class Uniform1ui : public Uniform<uint32_t> {
	protected:
		void doLoad() override;

	}; // class Uniform1ui

	class Uniform2ui : public Uniform<glm::uvec2> {
	protected:
		void doLoad() override;

	}; // class Uniform2ui

	class Uniform3ui : public Uniform<glm::uvec3> {
	protected:
		void doLoad() override;

	}; // class Uniform3ui

	class Uniform4ui : public Uniform<glm::uvec4> {
	protected:
		void doLoad() override;

	}; // class Uniform4ui

	class Uniform1fv : public Uniformv<GLfloat *> {
	public:
		explicit Uniform1fv(const char *name) : Uniformv<GLfloat *>(name) {}

		void load(GLfloat *value) override;

	}; // class Uniform1fv

	class Uniform2fv : public Uniformv<GLfloat *> {
	public:
		explicit Uniform2fv(const char *name) : Uniformv<GLfloat *>(name) {}

		void load(GLfloat *value) override;

	}; // class Uniform2fv

	class Uniform3fv : public Uniformv<GLfloat *> {
	public:
		explicit Uniform3fv(const char *name) : Uniformv<GLfloat *>(name) {}

		void load(GLfloat *value) override;

	}; // class Uniform3fv

	class Uniform4fv : public Uniformv<GLfloat *> {
	public:
		explicit Uniform4fv(const char *name) : Uniformv<GLfloat *>(name) {}

		void load(GLfloat *value) override;

	}; // class Uniform4fv

	class UniformMatrix2fv : public Uniformv<glm::mat2> {
	public:
		explicit UniformMatrix2fv(const char *name) : Uniformv<glm::mat2>(name) {}

		void load(glm::mat2) override;

	}; // class UniformMatrix2fv

	class UniformMatrix3fv : public Uniformv<glm::mat3> {
	public:
		explicit UniformMatrix3fv(const char *name) : Uniformv<glm::mat3>(name) {}

		void load(glm::mat3) override;

	}; // class UniformMatrix3fv

	class UniformMatrix4fv : public Uniformv<glm::mat4> {
	public:
		explicit UniformMatrix4fv(const char *name) : Uniformv<glm::mat4>(name) {}

		void load(glm::mat4) override;

	}; // class UniformMatrix4fv

} // namespace uinta

#endif //UINTA_UNIFORM_H
