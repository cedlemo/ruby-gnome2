/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgdkimage.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:01 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

#define _SELF(i) GDK_IMAGE(RVAL2GOBJ(i))

#ifdef GDK_ENABLE_BROKEN
static VALUE
gdkimage_s_newbmap(klass, visual, data, w, h)
    VALUE klass, visual, data, w, h;
{
    int width = NUM2INT(w);
    int height = NUM2INT(h);
    Check_Type(data, T_STRING);
    if (RSTRING(data)->len < (width * height)) {
        rb_raise(rb_eArgError, "data too short");
    }
    return GOBJ2RVAL(gdk_image_new_bitmap(GDK_VISUAL(RVAL2GOBJ(visual)),
                                          RSTRING(data)->ptr,
                                          width, height));
}
#endif

static VALUE
gdkimage_initialize(self, type, visual, w, h)
    VALUE self, type, visual, w, h;
{
    G_INITIALIZE(self, gdk_image_new((GdkImageType)NUM2INT(type),
                                          GDK_VISUAL(RVAL2GOBJ(visual)),
                                          NUM2INT(w), NUM2INT(h)));
    return Qnil;
}

static VALUE
gdkimage_s_get(klass, win, x, y, w, h)
    VALUE klass, win, x, y, w, h;
{
    return GOBJ2RVAL(gdk_image_get(GDK_DRAWABLE(RVAL2GOBJ(win)),
                         NUM2INT(x), NUM2INT(y),
                         NUM2INT(w), NUM2INT(h)));
}

static VALUE
gdkimage_put_pixel(self, x, y, pix)
    VALUE self, x, y, pix;
{
    gdk_image_put_pixel(_SELF(self),
                        NUM2INT(x), NUM2INT(y), NUM2INT(pix));
    return self;
}

static VALUE
gdkimage_get_pixel(self, x, y)
    VALUE self, x, y;
{
    return INT2NUM(gdk_image_get_pixel(_SELF(self),
                                       NUM2INT(x), NUM2INT(y)));
}

static VALUE
gdkimage_destroy(self)
    VALUE self;
{
    gdk_image_destroy(_SELF(self));
    DATA_PTR(self) = 0;
    return Qnil;
}

static VALUE
gdkimage_width(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->width);
}

static VALUE
gdkimage_height(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->height);
}

static VALUE
gdkimage_depth(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->depth);
}

static VALUE
gdkimage_bpp(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->bpp);
}

static VALUE
gdkimage_bpl(self)
    VALUE self;
{
    return INT2NUM((_SELF(self))->bpl);
}

void 
Init_gtk_gdk_image()
{
    VALUE gdkImage = G_DEF_CLASS(GDK_TYPE_IMAGE, "Image", mGdk);

#ifdef GDK_ENABLE_BROKEN
    rb_define_singleton_method(gdkImage, "new_bitmap", gdkimage_s_newbmap, 4);
#endif
    rb_define_singleton_method(gdkImage, "get", gdkimage_s_get, 5);

    rb_define_method(gdkImage, "initialize", gdkimage_initialize, 4);
    rb_define_method(gdkImage, "put_pixel", gdkimage_put_pixel, 3);
    rb_define_method(gdkImage, "get_pixel", gdkimage_get_pixel, 2);
    rb_define_method(gdkImage, "destroy", gdkimage_destroy, 0);
    rb_define_method(gdkImage, "width", gdkimage_width, 0);
    rb_define_method(gdkImage, "height", gdkimage_height, 0);
    rb_define_method(gdkImage, "depth", gdkimage_depth, 0);
    rb_define_method(gdkImage, "bpp", gdkimage_bpp, 0);
    rb_define_method(gdkImage, "bpl", gdkimage_bpl, 0);
}
