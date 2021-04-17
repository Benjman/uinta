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
	glUniform1f(_id, _value);
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

void Uniform1fv::load(GLfloat* value) {
	glUniform1fv(_id, 1, value);
	glCheckError(GL_UNIFORM);
}

void Uniform2fv::load(GLfloat* value) {
	glUniform2fv(_id, 1, value);
	glCheckError(GL_UNIFORM);
}

void Uniform3fv::load(GLfloat* value) {
	glUniform3fv(_id, 1, value);
	glCheckError(GL_UNIFORM);
}

void Uniform4fv::load(GLfloat* value) {
	glUniform4fv(_id, 1, value);
	glCheckError(GL_UNIFORM);
}

void UniformMatrix2fv::load(glm::mat2 value) {
	glUniformMatrix2fv(_id, 1, GL_FALSE, &value[0][0]);
	glCheckError(GL_UNIFORM);
}

void UniformMatrix3fv::load(glm::mat3 value) {
	glUniformMatrix3fv(_id, 1, GL_FALSE, &value[0][0]);
	glCheckError(GL_UNIFORM);
}

void UniformMatrix4fv::load(glm::mat4 value) {
	glUniformMatrix4fv(_id, 1, GL_FALSE, &value[0][0]);
	glCheckError(GL_UNIFORM);
}
