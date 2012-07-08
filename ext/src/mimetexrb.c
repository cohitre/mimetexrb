 /****************************************************************************
 * MimeTex_rb Copyright(c) 2007, 32leaves, All rights reserved.
 * --------------------------------------------------------------------------
 * This file is part of MimeTex_rb, which is free software. You may redistribute
 * and/or modify it under the terms of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *      MimeTeX_rb is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY, not even the implied warranty of MERCHANTABILITY.
 * See the GNU General Public License for specific details.
 *      By using MimeTex_rb, you warrant that you have read, understood and
 * agreed to these terms and conditions, and that you possess the legal
 * right and ability to enter into this agreement and to use MimeTex_rb
 * in accordance with it.
 *      Your MimeTex_rb distribution should contain a copy of the GNU General
 * Public License.  If not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA,
 * or point your browser to  http://www.gnu.org/licenses/gpl.html
 ****************************************************************************/


#include "ruby.h"
#include "mimetex.h"
#include <stdio.h>

#define MAXGIFSZ 131072

GLOBAL(raster,*bitmap_raster,NULL);

static VALUE rb_mMimeTex;
static VALUE rb_cMimeTexRender;

int	MIMETEX_GetPixel ( int x, int y ) {
	int	ipixel = y*bitmap_raster->width + x; /* pixel index for x,y-coords*/
	return (int)getlongbit(bitmap_raster->pixmap,ipixel); /*pixel = 0 or 1*/;
}

static VALUE MIMETEX_init(VALUE self, int fontSize) {
	rb_iv_set(self, "@fontSize", fontSize);
	return self;
}

static VALUE MIMETEX_render(VALUE self, VALUE rb_sLatexCode) {
	Check_Type (rb_sLatexCode, T_STRING);

	VALUE rb_sLatexCodeCopy;

	// Because plain duplication shares internal memory region.  You
	// have to call rb_str_modify() before modifying a string.
  rb_sLatexCodeCopy = rb_str_dup(rb_sLatexCode);
  rb_str_modify(rb_sLatexCodeCopy);

	int fontSize = FIX2INT(rb_iv_get(self, "@fontSize"));

	if(fontSize > 4 || fontSize < 1) fontSize = 2;

	subraster *sp = rasterize(StringValuePtr(rb_sLatexCodeCopy), fontSize);
	bitmap_raster = sp->image;

  type_raster(bitmap_raster,stdout);  /* display ascii image of raster */

	char *gif_buffer = ALLOCA_N(char, MAXGIFSZ);

	int gifSize = 0;
	int status = GIF_Create(gif_buffer, bitmap_raster->width, bitmap_raster->height, 2, 8);
	GIF_SetColor(1,0,0,0); /* foreground black if all 0 */

	if(status != 0) rb_raise(INT2NUM(status), "Could not create GIF image");
	GIF_SetTransparent(0);
	GIF_CompressImage(0, 0, -1, -1, MIMETEX_GetPixel); /* emit gif */
	gifSize = GIF_Close();

	if(sp != NULL) delete_subraster(sp);	/* and free expression */

	return rb_str_new(gif_buffer, gifSize);
}

void Init_mimetexrb() {
	rb_mMimeTex = rb_define_module("MimeTex");
	rb_cMimeTexRender = rb_define_class_under(rb_mMimeTex, "Render", rb_cObject);
	rb_define_method(rb_cMimeTexRender, "initialize", MIMETEX_init, 1);
	rb_define_method(rb_cMimeTexRender, "render", MIMETEX_render, 1);
}
