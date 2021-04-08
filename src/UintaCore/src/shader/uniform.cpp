#include <uinta/shader/uniform.h>
#include <uinta/gl/gl_error.h>

#include <iostream>

using namespace uinta;

void IUniform::storeLocation(GLuint programId) {
	_id = glGetUniformLocation(programId, _name);
	if (_id == INVALID_UNIFORM_ID) {
		std::cerr << "Uniform not found for '" << _name << "'.\n";
	}
}

void Uniform1f::doLoad() {
	glUniform1f(_id, 1.f);
	glCheckError(GL_UNIFORM);
}

void Uniform2f::doLoad() {
	glUniform2f(_id, _value.x, _value.y);
	glCheckError(GL_UNIFORM);
}

void Uniform3f::doLoad() {
	glUniform3f(_id, _value.x, _value.y, _value.z);
	glCheckError(GL_UNIFORM);
}

void Uniform4f::doLoad() {
	glUniform4f(_id, _value.x, _value.y, _value.z, _value.w);
	glCheckError(GL_UNIFORM);
}

void Uniform1i::doLoad() {
	glUniform1i(_id, _value);
	glCheckError(GL_UNIFORM);
}

void Uniform2i::doLoad() {
	glUniform2i(_id, _value.x, _value.y);
	glCheckError(GL_UNIFORM);
}

void Uniform3i::doLoad() {
	glUniform3i(_id, _value.x, _value.y, _value.z);
	glCheckError(GL_UNIFORM);
}

void Uniform4i::doLoad() {
	glUniform4i(_id, _value.x, _value.y, _value.z, _value.w);
	glCheckError(GL_UNIFORM);
}

void Uniform1ui::doLoad() {
	glUniform1ui(_id, _value);
	glCheckError(GL_UNIFORM);
}

void Uniform2ui::doLoad() {
	glUniform2ui(_id, _value.x, _value.y);
	glCheckError(GL_UNIFORM);
}

void Uniform3ui::doLoad() {
	glUniform3ui(_id, _value.x, _value.y, _value.z);
	glCheckError(GL_UNIFORM);
}

void Uniform4ui::doLoad() {
	glUniform4ui(_id, _value.x, _value.y, _value.z, _value.w);
	glCheckError(GL_UNIFORM);
}

void Uniform1fv::doLoad() {
	glUniform1fv(_id, _count, _value);
	glCheckError(GL_UNIFORM);
}

void Uniform2fv::doLoad() {
	glUniform2fv(_id, _count, _value);
	glCheckError(GL_UNIFORM);
}

void Uniform3fv::doLoad() {
	glUniform3fv(_id, _count, _value);
	glCheckError(GL_UNIFORM);
}

void Uniform4fv::doLoad() {
	glUniform4fv(_id, _count, _value);
	glCheckError(GL_UNIFORM);
}

void UniformMatrix2fv::doLoad() {
	glUniformMatrix2fv(_id, _count, GL_FALSE, _value);
	glCheckError(GL_UNIFORM);
}

void UniformMatrix3fv::doLoad() {
	glUniformMatrix3fv(_id, _count, GL_FALSE, _value);
	glCheckError(GL_UNIFORM);
}

void UniformMatrix4fv::doLoad() {
	glUniformMatrix4fv(_id, _count, GL_FALSE, _value);
	glCheckError(GL_UNIFORM);
}
