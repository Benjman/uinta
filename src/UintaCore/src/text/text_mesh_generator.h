#ifndef UINTA_FONT_MESH_GENERATOR_H
#define UINTA_FONT_MESH_GENERATOR_H

#include "line.h"
#include "word.h"

#include <cstring>
#include <uinta/text.h>
#include <uinta/gl/gl_macros.h>

namespace uinta {

	struct TextMeshGenerator {
		static void generateMesh(Text &text,
				float_t *data,
				uint32_t *indices,
				size_t *vertexCount,
				size_t *indexCount) {
			size_t charCount = 0;
			std::vector<Line *> lines = generateStructure(text, &charCount);

			*vertexCount = charCount * 4;
			*indexCount = charCount * 6;

			float_t cursorY = 0;
			for (auto line : lines) {
				processLine(text, *line, data, indices, cursorY);
				cursorY += text._font->getLineHeight();
				delete line;
			}

		}

	private:
		static std::vector<Line *> generateStructure(Text &text, size_t *charCount) {
			std::vector<Line *> lines;
			float_t a = 0,
					lsb = 0;

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
					text._font->getCharWidth(c, &a, &lsb);
					word->addChar(c, a + lsb);
					(*charCount)++;
				}
				if (!line->tryAddWord(*word)) {
					line = new Line(text.getSize().x);
					lines.push_back(line);
					line->tryAddWord(*word);
				}
			}

			return lines;
		}

		static void processLine(const Text &text, const Line &line, float_t *data, uint32_t *indices, float_t cursorY) {
			float_t cursorX = text.getPosition().x;
			for (size_t i = 0, size = line.words.size(); i < size; i++) {
				processWord(text, *line.words[i], line, data, indices, cursorX, cursorY);
				if (i + 1 < size) {
					const TexturedQuadDto quadInfo = text._font->getTexturedQuadInfo(' ', cursorX, cursorY);
					processQuad(text, quadInfo, data, indices);
				}
			}
		}

		static void processWord(const Text &text, const Word &word, const Line &line, float_t *data, uint32_t *indices, float_t cursorX, float_t cursorY) {
			for (char c : word.characters) {
				const TexturedQuadDto quadInfo = text._font->getTexturedQuadInfo(c, cursorX, cursorY);
				processQuad(text, quadInfo, data, indices);
			}
		}

		static void processQuad(const Text &text, const TexturedQuadDto &quadInfo, float_t *data, uint32_t *indices) {
			float_t d[16] {
					TO_GL_NDC_X(quadInfo.x0),  TO_GL_NDC_Y(quadInfo.y0),   quadInfo.s0, quadInfo.t1,
					TO_GL_NDC_X(quadInfo.x0), TO_GL_NDC_Y(quadInfo.y1),   quadInfo.s0, quadInfo.t0,
					TO_GL_NDC_X(quadInfo.x1), TO_GL_NDC_Y(quadInfo.y1),   quadInfo.s1, quadInfo.t0,
					TO_GL_NDC_X(quadInfo.x1),  TO_GL_NDC_Y(quadInfo.y0),   quadInfo.s1, quadInfo.t1
			};
			uint32_t i[6] {
				0, 1, 3,
				1, 2, 3
			};
			std::memcpy(data, d, sizeof(d));
			std::memcpy(indices, i, sizeof(i));
		}

	}; // class FontMeshGenerator

} // namespace uinta


#endif //UINTA_FONT_MESH_GENERATOR_H
