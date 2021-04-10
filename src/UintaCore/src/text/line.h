#ifndef UINTA_LINE_H
#define UINTA_LINE_H

#include "word.h"

#include <uinta/types.h>

#include <vector>

namespace uinta {

	struct Line {
 		float_t lineWidthPx = 0;
 		float_t maxWidthPx = 0;
		std::vector<Word *> words;

		explicit Line(float_t maxWidth) : maxWidthPx(maxWidth) {}

		bool tryAddWord(Word &word) {
			if (word.width + lineWidthPx > maxWidthPx) {
				return false;
			}
			words.push_back(&word);
			lineWidthPx += word.width;
			return true;
		}

		~Line() {
			for (auto word : words) {
				delete word;
			}
		}

	}; // struct Line

} // namespace uinta

#endif //UINTA_LINE_H
