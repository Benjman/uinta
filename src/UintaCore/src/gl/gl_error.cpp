#include <uinta/gl/gl_error.h>
#include <uinta/gl/gl_types.h>

#include <iostream> // TODO remove when logging is implemented

namespace uinta {

	void checkGlBufferData(GLenum err) {
		switch (err) {
			case GL_INVALID_ENUM:
				std::cerr << "glBufferData GL_INVALID_ENUM: Either;";
				std::cerr << "\n\t1)_target is not one of the accepted buffer targets. Or,";
				std::cerr << "\n\t2) _usage is not GL_STREAM_DRAW, GL_STREAM_READ, GL_STREAM_COPY, ";
				std::cerr << "GL_STATIC_DRAW, GL_STATIC_READ, GL_STATIC_COPY, GL_DYNAMIC_DRAW, ";
				std::cerr << "GL_DYNAMIC_READ, or GL_DYNAMIC_COPY.";
				std::cerr << std::endl;
				return;

			case GL_INVALID_VALUE:
				std::cerr << "glBufferData GL_INVALID_VALUE: Size is negative." << std::endl;
				return;

			case GL_INVALID_OPERATION:
				std::cerr << "glBufferData GL_INVALID_OPERATION: The reserved buffer object named 0 is ";
				std::cerr << "bound to target." << std::endl;
				return;

			case GL_OUT_OF_MEMORY:
				std::cerr << "glBufferData GL_INVALID_OPERATION: GL is unable to create a data store ";
				std::cerr << "with the specified size." << std::endl;
				return;

			default:
				std::cerr << "Unknown glBufferData error." << std::endl;
		}
	}

	void checkGlGenBuffers(GLenum err) {
		if (err == GL_INVALID_VALUE) {
			std::cerr << "glGenBuffers GL_INVALID_VALUE: n is negative." << std::endl;
		} else {
			std::cerr << "Unknown glGenBuffers error." << std::endl;
		}
	}

	void checkGlVertexAttribPointer(GLenum err) {
		switch (err) {
			case GL_INVALID_VALUE:
				std::cerr << "glVertexAttribPointer GL_INVALID_VALUE: Either;";
				std::cerr << "\n\t1) _index is greater than or equal to GL_MAX_VERTEX_ATTRIBS. Or,";
				std::cerr << "\n\t2) _size is not 1, 2, 3 or 4.";
				std::cerr << "\n\t3) _stride is negative." << std::endl;
				break;

			case GL_INVALID_ENUM:
				std::cerr << "glVertexAttribPointer GL_INVALID_ENUM: _type is not an accepted value." << std::endl;
				break;

			case GL_INVALID_OPERATION:
				std::cerr << "glVertexAttribPointer GL_INVALID_OPERATION: Either;";
				std::cerr << "\n\t1) _type is GL_INT_2_10_10_10_REV or ";
				std::cerr << "GL_UNSIGNED_INT_2_10_10_10_REV and size is not 4. Or,";
				std::cerr << "\n\t2) A non-zero vertex array object is bound, zero is bound to the ";
				std::cerr << "GL_ARRAY_BUFFER buffer object binding point and the pointer argument ";
				std::cerr << "is not NULL." << std::endl;
				break;

			default:
				std::cerr << "Unknown glVertexAttribPointer error." << std::endl;
		}
	}

	void checkGlBufferSubData(GLenum err) {
		switch (err) {
			case GL_INVALID_ENUM:
				std::cerr << "glBufferSubData GL_INVALID_ENUM: Unacceptable VBO target type." << std::endl;
				return;

			case GL_INVALID_VALUE:
				std::cerr << "glBufferSubData GL_INVALID_VALUE: VBO offset or size is negative, or together ";
				std::cerr << "they define a region of memory that extends beyond the buffer object's allocated ";
				std::cerr << "data store." << std::endl;
				return;

			case GL_INVALID_OPERATION:
				std::cerr << "glBufferSubData GL_INVALID_OPERATION: VBO operation invalid because either;";
				std::cerr << "\n\t1) The reserved buffer object name O is bound to target. Or,";
				std::cerr << "\n\t2) The buffer object being updated is mapped." << std::endl;
				return;


			default:
				std::cerr << "Unknown glBufferSubData error." << std::endl;
		}
	}

	void checkGlBindBuffer(GLenum err) {
		if (err == GL_INVALID_ENUM) {
			std::cerr << "glBindBuffer GL_INVALID_ENUM: _target is not one of the allowable values." << std::endl;
		} else {
			std::cerr << "Unknown glBindBuffer error." << std::endl;
		}
	}

	void checkGlDisableVertexAttribArray(GLenum err) {
		if (err == GL_INVALID_VALUE) {
			std::cerr << "glDisableVertexAttribArray  GL_INVALID_VALUE: _index is greater than or equal to ";
			std::cerr << "GL_MAX_VERTEX_ATTRIBS" << std::endl;
		} else {
			std::cerr << "Unknown glDisableVertexAttribArray error." << std::endl;
		}
	}

	void checkGlEnableVertexAttribArray(GLenum err) {
		if (err == GL_INVALID_VALUE) {
			std::cerr << "glEnableVertexAttribArray  GL_INVALID_VALUE: _index is greater than or equal to ";
			std::cerr << "GL_MAX_VERTEX_ATTRIBS" << std::endl;
		} else {
			std::cerr << "Unknown glEnableVertexAttribArray error." << std::endl;
		}
	}

}

void uinta::checkGlError(gl_error_check_type type) {
#ifdef UINTA_DEBUG
	GLenum err = glGetError();
	if (!err) return;

	switch (type) {
		case GL_BUFFER_DATA:
			checkGlBufferData(err);
			return;
		case GL_GEN_BUFFERS:
			checkGlGenBuffers(err);
			return;
		case GL_VERTEX_ATTRIB_POINTER:
			checkGlVertexAttribPointer(err);
			return;
		case GL_BUFFER_SUB_DATA:
			checkGlBufferSubData(err);
			break;
		case GL_BIND_BUFFER:
			checkGlBindBuffer(err);
			break;
		case GL_DISABLE_VERTEX_ATTRIB_ARRAY:
			checkGlDisableVertexAttribArray(err);
			break;
		case GL_ENABLE_VERTEX_ATTRIB_ARRAY:
			checkGlEnableVertexAttribArray(err);
			break;
	}
#endif
}
