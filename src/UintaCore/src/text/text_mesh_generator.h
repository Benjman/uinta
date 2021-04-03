#ifndef UINTA_FONT_MESH_GENERATOR_H
#define UINTA_FONT_MESH_GENERATOR_H

#include "line.h"
#include "word.h"

#include <cstring>
#include <uinta/text.h>
#include <uinta/gl/gl_macros.h>
#include <uinta/gl/gl_state.h>

namespace uinta {

	struct TextMeshGenerator {

		struct GenerationContext {
			Text &text;
			Font &font;
			float_t *vBuffer{};
			float_t aspectRatio = 0.f;
			float_t xcursor = 0.f;
			float_t ycursor = 0.f;
			float_t lineHeight = 0.f;
			float_t viewportHeight = 0.f;
			uint32_t dataPointer = 0;
			uint32_t *iBuffer{};

			GenerationContext(Text &text, Font &font, float_t *vBuffer, uint32_t *iBuffer)
					: text(text),
					  font(font),
					  vBuffer(vBuffer),
					  iBuffer(iBuffer) {
				aspectRatio = gl_state::getViewportAspectRatio();
				dataPointer = 0;
				viewportHeight = gl_state::getViewportHeight();
				lineHeight = Font::LINE_HEIGHT;
				xcursor = text.getPosition().x;
				ycursor = text.getPosition().y;
			}
		};

		static void generateMesh(Text &text, Font &font, float_t *data, uint32_t *indices) {
			GenerationContext context(text, font, data, indices);
			const std::vector<Line *> &lines = generateStructure(context);
			processLines(lines, context);
		}

	private:
		static std::vector<Line *> generateStructure(GenerationContext &context) {
			std::vector<Line *> lines;
			Text &text = context.text;
			const Font &font = context.font;
			if (!text.getValue().empty()) {
				Line *line = new Line(gl_state::getViewportWidth());
				lines.push_back(line);
				Word *word = new Word;

				for (size_t i = 0, len = text.getValue().size(); i < len; i++) {
					char c = text.getValue()[i];
					if (c == ' ') {
						if (!line->tryAddWord(*word)) {
							line = new Line(gl_state::getViewportWidth());
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
			for (size_t i = 0, size = line.words.size(); i < size; i++) {
				processWord(*line.words[i], context);
				if (i + 1 < size) {
					// add space
					//	really just advancing the xcursor
					context.font.getQuadInfo(' ', &context.xcursor, &context.ycursor);
				}
			}
		}

		static void processWord(const Word &word, GenerationContext &context) {
			for (size_t i = 0, len = word.characters.size(); i < len; i++) {
				const stbtt_aligned_quad quad = context.font.getQuadInfo(word.characters[i],
																				&context.xcursor,
																				&context.ycursor);
				processQuad(quad, context);
				if (i + 1 != len) {
					// kerning
					context.xcursor += (float_t) stbtt_GetCodepointKernAdvance(&context.font._stbttFontInfo,
																			   word.characters[i],
																			   word.characters[i + 1]);
				}
			}
		}

		static void processQuad(const stbtt_aligned_quad &quad, GenerationContext &context) {
			float_t x0 = quad.x0;
			float_t x1 = quad.x1;
			float_t y0 = context.lineHeight + quad.y0;
			float_t y1 = context.lineHeight + quad.y1;

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

			const float_t vertices[16]{
					x0, y0,
					quad.s0, quad.t0,

					x0, y1,
					quad.s0, quad.t1,

					x1, y1,
					quad.s1, quad.t1,

					x1, y0,
					quad.s1, quad.t0
			};
			std::memcpy(&context.vBuffer[context.dataPointer * 16], vertices, sizeof(vertices));

			uint32_t indices[6]{
					0, 1, 3,
					1, 2, 3
			};
			for (uint32_t &index : indices) {
				index += context.dataPointer * 4;
			}
			std::memcpy(&context.iBuffer[context.dataPointer * 6], indices, sizeof(indices));

			context.dataPointer++;
		}

	}; // class FontMeshGenerator

} // namespace uinta


#endif //UINTA_FONT_MESH_GENERATOR_H
