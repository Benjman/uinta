#ifndef UINTA_SHADER_LOADER_H
#define UINTA_SHADER_LOADER_H

#include <uinta/shader/shader_dto.h>
#include <uinta/shader/shader.h>

#include <uinta/io.h>
#include <uinta/util/assert.h> // NOLINT(modernize-deprecated-headers)
#include <cstring>

#include <iostream> // TODO remove after logging is implemented

namespace uinta {

	struct ShaderLoader {

		static Shader loadShader(ShaderDto &dto) {
			if (dto._sourceType == IO) {
				loadSources(dto);
			}

			assert(!std::string(dto._vertSource).empty());
			assert(!std::string(dto._fragSource).empty());

			GLuint vertId = compileShader(GL_VERTEX_SHADER, dto._vertSource);
			GLuint fragId = compileShader(GL_FRAGMENT_SHADER, dto._fragSource);
			GLuint programId = linkProgram(vertId, fragId);

			return Shader(programId);
		}

	private:
		static void loadSources(ShaderDto &dto) {
			assert(!std::string(dto._vertPath).empty());
			assert(!std::string(dto._fragPath).empty());

			// TODO thread this bad boy
			File vertFile = File::requestFile(dto._vertPath);
			File fragFile = File::requestFile(dto._fragPath);

			dto._vertLength = vertFile.getContentLength();
			dto._fragLength = fragFile.getContentLength();

			dto._vertSource = new char[dto._vertLength]{};
			dto._fragSource = new char[dto._fragLength]{};

			strncpy(dto._vertSource, vertFile.getContents(), dto._vertLength - 1);
			strncpy(dto._fragSource, fragFile.getContents(), dto._fragLength - 1);
		}

		static GLuint compileShader(GLuint type, const char *source) {
			GLuint id = glCreateShader(type);

			glShaderSource(id, 1, &source, nullptr);

			glCompileShader(id);

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
			glAttachShader(id, vertId);
			glAttachShader(id, 5);
			glLinkProgram(id);

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
			glDeleteShader(fragId);

			return id;
		}

	};

}

#endif //UINTA_SHADER_LOADER_H
