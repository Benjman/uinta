#ifndef UINTA_WORD_H
#define UINTA_WORD_H

#include <uinta/types.h>

#include <vector>

namespace uinta {

	struct Word {
		float_t width = 0;
		std::vector<char> characters;

		void addChar(char c, float_t widthPx) {
			characters.push_back(c);
			width += widthPx;
		}

	}; // struct Word

} // namespace uinta

#endif //UINTA_WORD_H
