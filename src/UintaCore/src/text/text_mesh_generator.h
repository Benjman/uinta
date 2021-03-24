#ifndef UINTA_FONT_MESH_GENERATOR_H
#define UINTA_FONT_MESH_GENERATOR_H

#include "line.h"
#include "word.h"

#include <cstring>
#include <uinta/text.h>
#include <uinta/gl/gl_macros.h>
#include <uinta/gl/gl_state.h>

namespace uinta {

	struct GenerationContext {
		const Text &text;
		float_t *vBuffer;
		uint32_t *iBuffer;
		float_t cursorX;
		float_t cursorY;
		float_t aspectRatio;
		float_t scale;
		uint32_t dataPointer;

		GenerationContext(const Text &text, float_t *vBuffer, uint32_t *iBuffer)
				: text(text),
				  vBuffer(vBuffer),
				  iBuffer(iBuffer) {
			cursorX = 0;
			cursorY = text.getFont()->getLineHeight();
			aspectRatio = gl_state::getViewportAspectRatio();
			scale = (float_t) text.getFont()->getLineHeight() / UI_BASE_SIZE;
			dataPointer = 0;
		}
	};

	struct TextMeshGenerator {
		static void generateMesh(Text &text, float_t *data, uint32_t *indices) {
			const std::vector<Line *> &lines = generateStructure(text);
			GenerationContext context(text, data, indices);
			processLines(lines, context);
		}

	private:
		static std::vector<Line *> generateStructure(Text &text) {
			std::vector<Line *> lines;
			if (!text._value.empty()) {
				Line *line = new Line(text.getSize().x);
				lines.push_back(line);
				Word *word = new Word;

				for (char c : text._value) {
					if (c == ' ') {
						if (!line->tryAddWord(*word)) {
							line = new Line(text.getSize().x);
							lines.push_back(line);
							line->tryAddWord(*word);
						}
						word = new Word;
						continue;
					}
					float_t cursorX = 0, cursorY = 0;
					TexturedQuadDto quadInfo = text.getFont()->getTexturedQuadInfo(c, &cursorX, &cursorY);
					word->addChar(c, quadInfo.x1 - quadInfo.x0);
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
				context.cursorY += context.text.getFont()->getLineHeight();
				delete line;
			}
		}

		static void processLine(const Line &line, GenerationContext &context) {
			context.cursorX = context.text.getPosition().x;
			for (size_t i = 0, size = line.words.size(); i < size; i++) {
				processWord(*line.words[i], context);
				if (i + 1 < size) {
					// add space
					//	really just advancing the cursorX
					context.text.getFont()->getTexturedQuadInfo(' ', &context.cursorX, &context.cursorY);
				}
			}
		}

		static void processWord(const Word &word, GenerationContext &context) {
			for (char c : word.characters) {
				const TexturedQuadDto quadInfo = context.text.getFont()->getTexturedQuadInfo(c, &context.cursorX, &context.cursorY);
				processQuad(quadInfo, context);
			}
		}

		static void processQuad(const TexturedQuadDto &quadInfo, GenerationContext &context) {
			float_t arX = context.aspectRatio < 1 ? context.aspectRatio : 1;
			float_t arY = context.aspectRatio > 1 ? context.aspectRatio : 1;
			float_t vertices[16]{
					TO_GL_NDC_X(quadInfo.x0 * arX * context.scale), TO_GL_NDC_Y(quadInfo.y0 * arY * context.scale),
					quadInfo.s0, quadInfo.t0,

					TO_GL_NDC_X(quadInfo.x0 * arX * context.scale), TO_GL_NDC_Y(quadInfo.y1 * arY * context.scale),
					quadInfo.s0, quadInfo.t1,

					TO_GL_NDC_X(quadInfo.x1 * arX * context.scale), TO_GL_NDC_Y(quadInfo.y1 * arY * context.scale),
					quadInfo.s1, quadInfo.t1,

					TO_GL_NDC_X(quadInfo.x1 * arX * context.scale), TO_GL_NDC_Y(quadInfo.y0 * arY * context.scale),
					quadInfo.s1, quadInfo.t0
			};
			uint32_t indices[6]{
					0, 1, 3,
					1, 2, 3
			};
			// add to indices according to current buffer position
			for (size_t i = 0, size = sizeof(indices) / sizeof(uint32_t); i < size; i++) {
				indices[i] += context.dataPointer * 4;
			}
			std::memcpy(&context.vBuffer[context.dataPointer * 16], vertices, sizeof(vertices));
			std::memcpy(&context.iBuffer[context.dataPointer * 6], indices, sizeof(indices));
			context.dataPointer++;
		}

	}; // class FontMeshGenerator

} // namespace uinta


#endif //UINTA_FONT_MESH_GENERATOR_H
