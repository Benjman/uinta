#ifndef UINTA_SHADER_LOADER_H
#define UINTA_SHADER_LOADER_H

#include <uinta/shader/shader_dto.h>
#include <uinta/shader/shader.h>

#include <uinta/io.h>
#include <uinta/util/assert.h> // NOLINT(modernize-deprecated-headers)
#include <uinta/gl/gl_error.h>

#include <cstring>
#include <iostream> // TODO remove after logging is implemented

namespace uinta {

	struct ShaderLoader {

		static GLuint loadShader(ShaderDto &dto) {
			if (dto.sourceType == IO) {
				loadSources(dto);
			}

			ASSERT(!std::string(dto.vertSource).empty());
			ASSERT(!std::string(dto.fragSource).empty());

			GLuint vertId = compileShader(GL_VERTEX_SHADER, dto.vertSource);
			GLuint fragId = compileShader(GL_FRAGMENT_SHADER, dto.fragSource);
			return linkProgram(vertId, fragId);
		}

	private:
		static void loadSources(ShaderDto &dto) {
			ASSERT(!std::string(dto.vertPath).empty());
			ASSERT(!std::string(dto.fragPath).empty());

			// TODO thread this bad boy
			File vertFile = File::requestFile(dto.vertPath);
			File fragFile = File::requestFile(dto.fragPath);

			dto.vertLength = vertFile.getContentLength();
			dto.fragLength = fragFile.getContentLength();

			dto.vertSource = new char[dto.vertLength]{};
			dto.fragSource = new char[dto.fragLength]{};

			strncpy(dto.vertSource, vertFile.getContents(), dto.vertLength - 1);
			strncpy(dto.fragSource, fragFile.getContents(), dto.fragLength - 1);
		}

		static GLuint compileShader(GLuint type, const char *source) {
			GLuint id = glCreateShader(type);
			glCheckError(GL_CREATE_SHADER);

			glShaderSource(id, 1, &source, nullptr);
			glCheckError(GL_SHADER_SOURCE);

			glCompileShader(id);
			glCheckError(GL_COMPILE_SHADER);

#ifdef UINTA_DEBUG
			// Error checking
			int success;
			char infoLog[1024];
			glGetShaderiv(id, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(id, sizeof(infoLog) / sizeof(char), nullptr, infoLog);
				std::cerr << "Failed to compile shader.\nError from OpenGL: " << infoLog << std::endl;
			}
#endif

			return id;
		}

		static GLuint linkProgram(GLuint vertId, GLuint fragId) {
			GLuint id = glCreateProgram();
			glCheckError(GL_CREATE_PROGRAM);

			glAttachShader(id, vertId);
			glCheckError(GL_ATTACH_SHADER);

			glAttachShader(id, fragId);
			glCheckError(GL_ATTACH_SHADER);

			glLinkProgram(id);
			glCheckError(GL_LINK_PROGRAM);

#ifdef UINTA_DEBUG
			int success;
			char infoLog[1024];
			glGetProgramiv(id, GL_LINK_STATUS, &success);
			if (!success) {
				glGetProgramInfoLog(id, sizeof(infoLog) / sizeof(char), nullptr, infoLog);
				std::cerr << "Failed to link shader.\nError from OpenGL: " << infoLog << std::endl;
			}
#endif

			glDeleteShader(vertId);
			glCheckError(GL_DELETE_SHADER);

			glDeleteShader(fragId);
			glCheckError(GL_DELETE_SHADER);

			return id;
		}

	};

}

#endif //UINTA_SHADER_LOADER_H
