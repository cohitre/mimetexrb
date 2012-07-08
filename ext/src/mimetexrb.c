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

static VALUE rb_mMimeTex;
static VALUE rb_cMimeTexRender;

static VALUE MIMETEX_init(VALUE self, int fontSize) {
	rb_iv_set(self, "@fontSize", fontSize);
	return self;
}

static VALUE MIMETEX_bitmap(VALUE self, VALUE rb_sLatexCode) {
	Check_Type (rb_sLatexCode, T_STRING);

	int fontSize = FIX2INT(rb_iv_get(self, "@fontSize"));

	if(fontSize > 4 || fontSize < 1) fontSize = 2;

	subraster *sp = rasterize(StringValuePtr(rb_sLatexCode), fontSize);

  int width = sp->image->width;
	int height = sp->image->height;

	VALUE hash = rb_hash_new();
	VALUE bitsArray = rb_ary_new();

	int y;
	int x;
	for (x = 0; x < height; x++) {
		VALUE row = rb_ary_new();
		for (y = 0; y < width; y++) {
			int bitValue = (int)getpixel(sp->image, x, y);
		  rb_ary_push(row, INT2FIX(bitValue));
		}
		rb_ary_push(bitsArray, row);
	}

	rb_hash_aset (hash, rb_tainted_str_new2 ("width"),  INT2FIX(width));
	rb_hash_aset (hash, rb_tainted_str_new2 ("height"), INT2FIX(height));
	rb_hash_aset (hash, rb_tainted_str_new2 ("bitmap"), bitsArray);

  return hash;
}

void Init_mimetexrb() {
	rb_mMimeTex = rb_define_module("MimeTex");
	rb_cMimeTexRender = rb_define_class_under(rb_mMimeTex, "Render", rb_cObject);
	rb_define_method(rb_cMimeTexRender, "initialize", MIMETEX_init, 1);
	rb_define_method(rb_cMimeTexRender, "render_bitmap", MIMETEX_bitmap, 1);
}
