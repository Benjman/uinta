#ifndef UINTA_FONT_MESH_GENERATOR_H
#define UINTA_FONT_MESH_GENERATOR_H

#include "line.h"
#include "word.h"

#include <uinta/gl/gl_macros.h>
#include <uinta/gl/gl_state.h>
#include <uinta/text.h>

#include <cstring> // for memcpy

namespace uinta {

	struct TextMeshGenerator {

		struct GenerationContext {
			float_t aspectRatio = 0.f;
			Font &font;
			GLuint *iBuffer{};
			size_t indexOffset = 0;
			float_t lineHeight = 0.f;
			size_t pointer = 0;
			Text &text;
			GLfloat *vBuffer{};
			float_t viewportHeight = 0.f;
			float_t xcursor = 0.f;
			float_t ycursor = 0.f;

			GenerationContext(Text &text, Font &font, GLfloat *vBuffer, GLuint *iBuffer, size_t indexOffset)
					: text(text),
					  font(font),
					  vBuffer(vBuffer),
					  iBuffer(iBuffer),
					  indexOffset(indexOffset) {
				aspectRatio = gl_state::getViewportAspectRatio();
				viewportHeight = (float_t) gl_state::getViewportHeight();
				lineHeight = Font::LINE_HEIGHT;
				xcursor = (float_t) text.getPosition().x;
				ycursor = (float_t) text.getPosition().y;
			}
		};

		static void generateMesh(Text &text, Font &font, GLfloat *data, GLuint *indices, size_t indexOffset) {
			GenerationContext context(text, font, data, indices, indexOffset);
			const std::vector<Line *> &lines = generateStructure(context);
			if (lines.empty()) {
				setIndices(context);
				return;
			}
			processLines(lines, context);
		}

	private:
		static std::vector<Line *> generateStructure(GenerationContext &context) {
			std::vector<Line *> lines;
			Text &text = context.text;
			const Font &font = context.font;
			if (!text.getValue().empty()) {
				Line *line = new Line((float_t) gl_state::getViewportWidth());
				lines.push_back(line);
				Word *word = new Word;

				for (size_t i = 0, len = text.getValue().size(); i < len; i++) {
					char c = text.getValue()[i];
					if (c == ' ') {
						if (!line->tryAddWord(*word)) {
							line = new Line((float_t) gl_state::getViewportWidth());
							lines.push_back(line);
							line->tryAddWord(*word);
						}
						float_t tmpx = 0, tmpy = 0;
						font.getQuadInfo(' ', &tmpx, &tmpy);
						line->lineWidthPx += tmpx;
						word = new Word;
						continue;
					}
					float_t tmpx = 0, tmpy = 0, kern = 0;
					font.getQuadInfo(c, &tmpx, &tmpy);
					if (i + 1 != len) {
						kern = (float_t) stbtt_GetCodepointKernAdvance(&font._stbttFontInfo, c,
																	   text.getValue()[i + 1]);
					}
					tmpx += kern;
					word->addChar(c, tmpx);
				}

				if (!line->tryAddWord(*word)) {
					line = new Line(text.getSize().x);
					lines.push_back(line);
					line->tryAddWord(*word);
				}
			}

			return lines;
		}

		static void processLines(const std::vector<Line *> &lines, GenerationContext &context) {
			for (auto line : lines) {
				processLine(*line, context);
				context.ycursor += context.lineHeight;
				context.xcursor = 0;
				delete line;
			}
		}

		static void processLine(const Line &line, GenerationContext &context) {
			for (auto word : line.words) {
				processWord(*word, context);
				context.font.getQuadInfo(' ', &context.xcursor, &context.ycursor);
			}
		}

		static void processWord(const Word &word, GenerationContext &context) {
			for (size_t i = 0, len = word.characters.size(); i < len; i++) {
				const stbtt_aligned_quad quad = context.font.getQuadInfo(word.characters[i], &context.xcursor, &context.ycursor);
				processQuad(quad, context);
				if (i + 1 != len) {
					// kerning
					char ch1 = word.characters[i];
					char ch2 = word.characters[i + 1];
					int advance = stbtt_GetCodepointKernAdvance(&context.font._stbttFontInfo, ch1, ch2);
					context.xcursor += (float_t) advance * context.font._scale;
				}
			}
		}

		static void processQuad(const stbtt_aligned_quad &quad, GenerationContext &context) {
			GLfloat x0 = quad.x0;
			GLfloat x1 = quad.x1;
			GLfloat y0 = context.lineHeight + quad.y0;
			GLfloat y1 = context.lineHeight + quad.y1;

			x0 /= context.viewportHeight;
			x1 /= context.viewportHeight;
			y0 /= context.viewportHeight;
			y1 /= context.viewportHeight;

			x0 *= context.aspectRatio < 1 ? context.aspectRatio : 1;
			x1 *= context.aspectRatio < 1 ? context.aspectRatio : 1;
			y0 *= context.aspectRatio > 1 ? context.aspectRatio : 1;
			y1 *= context.aspectRatio > 1 ? context.aspectRatio : 1;

			x0 = TO_GL_NDC_X(x0);
			x1 = TO_GL_NDC_X(x1);
			y0 = TO_GL_NDC_Y(y0);
			y1 = TO_GL_NDC_Y(y1);

			const GLfloat vertices[TextController::VERTICES_PER_CHAR * TextController::ELEMENTS_PER_VERTEX]{
					x0, y0,
					quad.s0, quad.t0,

					x0, y1,
					quad.s0, quad.t1,

					x1, y1,
					quad.s1, quad.t1,

					x1, y0,
					quad.s1, quad.t0
			};
			std::memcpy(&context.vBuffer[context.pointer * 16], vertices, sizeof(vertices));

			setIndices(context);

			context.pointer++;
		}

		static void setIndices(GenerationContext &context) {
			GLuint indices[]{
					0, 1, 3,
					1, 2, 3
			};
			for (GLuint &index : indices) {
				index += context.pointer * 4;
				index += context.indexOffset;
			}
			memcpy(&context.iBuffer[context.pointer * 6], indices, sizeof(indices));
		}

	}; // class FontMeshGenerator

} // namespace uinta


#endif //UINTA_FONT_MESH_GENERATOR_H
