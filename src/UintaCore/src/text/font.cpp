#include "font_loader.h"

#include <uinta/text/font.h>

uinta::Font uinta::Font::loadFont(const char *trueTypePath) {
	return FontLoader::loadFond(trueTypePath);
}
