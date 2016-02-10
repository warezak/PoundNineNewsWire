/**
 * \file
 *
 * \brief Graphical font support
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <assert.h>

#include "conf_sysfont.h"

#include "gfx.h"
#include "sysfont.h"

 /* #define USE_FONT_BPMONO_10x14 */
 /* #define USE_FONT_MONO_MMM_10x12 */
 //#define USE_FONT_BASIC_6x7  // normal sys font


/* Use macro from conf_sysfont.h to define font glyph data. */
SYSFONT_DEFINE_GLYPHS;

/**
 * \brief Initialize a basic system font
 *
 * This initializes a basic system font globally usable by the application.
 */
struct font sysfont = {
	.type           = FONT_LOC_PROGMEM,
	.width          = SYSFONT_WIDTH,
	.height         = SYSFONT_HEIGHT,
	.first_char     = SYSFONT_FIRSTCHAR,
	.last_char      = SYSFONT_LASTCHAR,
	.data           = {
		.progmem        = sysfont_glyphs,
	},
};

//#undef USE_FONT_BASIC_6x7
// #define USE_FONT_BPMONO_10x16  // big font

BIGFONT_DEFINE_GLYPHS
struct font bigfont = {
	.type           = FONT_LOC_PROGMEM,
	.width          = BIGFONT_WIDTH,
	.height         = BIGFONT_HEIGHT,
	.first_char     = BIGFONT_FIRSTCHAR,
	.last_char      = BIGFONT_LASTCHAR,
	.data           = {
		.progmem        = bigfont_glyphs,
	},
};

BOLDFONT_DEFINE_GLYPHS
struct font boldfont = {
	.type           = FONT_LOC_PROGMEM,
	.width          = BOLDFONT_WIDTH,
	.height         = BOLDFONT_HEIGHT,
	.first_char     = BOLDFONT_FIRSTCHAR,
	.last_char      = BOLDFONT_LASTCHAR,
	.data           = {
		.progmem        = boldfont_glyphs,
	},
};

BOLD_ARIEL_FONT_DEFINE_GLYPHS
struct font bold_ariel_font = {
	.type           = FONT_LOC_PROGMEM,
	.width          = BOLD_ARIEL_FONT_WIDTH,
	.height         = BOLD_ARIEL_FONT_HEIGHT,
	.first_char     = BOLD_ARIEL_FONT_FIRSTCHAR,
	.last_char      = BOLD_ARIEL_FONT_LASTCHAR,
	.data           = {
		.progmem        = bold_ariel_font_glyphs,
	},
};
/** @} */
