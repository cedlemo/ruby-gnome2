/* -*- c-file-style: "ruby"; indent-tabs-mode: nil -*- */
/************************************************

  rbgtkmenu.c -

  $Author: mutoh $
  $Date: 2002/09/12 19:06:02 $

  Copyright (C) 1998-2000 Yukihiro Matsumoto,
                          Daisuke Kanda,
                          Hiroshi Igarashi
************************************************/

#include "global.h"

static ID call;

static VALUE
menu_initialize(self)
    VALUE self;
{
    RBGTK_INITIALIZE(self, gtk_menu_new());
    return Qnil;
}

static VALUE
menu_append(self, child)
    VALUE self, child;
{
    gtk_menu_append(GTK_MENU(RVAL2GOBJ(self)), 
					GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
menu_prepend(self, child)
    VALUE self, child;
{
    gtk_menu_prepend(GTK_MENU(RVAL2GOBJ(self)), 
					 GTK_WIDGET(RVAL2GOBJ(child)));
    return self;
}

static VALUE
menu_insert(self, child, pos)
    VALUE self, child, pos;
{
    gtk_menu_insert(GTK_MENU(RVAL2GOBJ(self)),
					GTK_WIDGET(RVAL2GOBJ(child)), NUM2INT(pos));
    return self;
}

static void
menu_pos_func(menu, px, py, data)
    GtkMenu *menu;
    gint *px, *py;
    gpointer data;
{
    VALUE arr = rb_funcall((VALUE)data, call, 3, GOBJ2RVAL(menu), 
						   INT2FIX(*px), INT2FIX(*py));
    Check_Type(arr, T_ARRAY);
    if (RARRAY(arr)->len != 2) {
		rb_raise(rb_eTypeError, "wrong number of result (%d for 2)", 
				 RARRAY(arr)->len);
    }
    *px = NUM2INT(RARRAY(arr)->ptr[0]);
    *py = NUM2INT(RARRAY(arr)->ptr[1]);
}

static VALUE
menu_popup(self, pshell, pitem, func, button, activate_time)
    VALUE self, pshell, pitem, func, button, activate_time;
{
    GtkWidget *gpshell = NULL;
    GtkWidget *gpitem = NULL;
    GtkMenuPositionFunc pfunc = NULL;
    gpointer data = NULL;

    if (!NIL_P(func)) {
		pfunc = menu_pos_func;
		data = (gpointer)func;
		G_RELATIVE(self, func);
    }
    if (!NIL_P(pshell)){
		gpshell = GTK_WIDGET(RVAL2GOBJ(pshell));
    }
    if (!NIL_P(pitem)) {
		gpitem = GTK_WIDGET(RVAL2GOBJ(pitem));
    }

    gtk_menu_popup(GTK_MENU(RVAL2GOBJ(self)),
				   gpshell, gpitem,
				   pfunc,
				   data,
				   NUM2INT(button),
				   NUM2INT(activate_time));
    return self;
}

static VALUE
menu_popdown(self)
    VALUE self;
{
    gtk_menu_popdown(GTK_MENU(RVAL2GOBJ(self)));
    return self;
}

static VALUE
menu_get_active(self)
    VALUE self;
{
    GtkWidget *mitem = gtk_menu_get_active(GTK_MENU(RVAL2GOBJ(self)));

    return (mitem == NULL) ? Qnil : GOBJ2RVAL(mitem);
}

static VALUE
menu_set_active(self, active)
    VALUE self, active;
{
    gtk_menu_set_active(GTK_MENU(RVAL2GOBJ(self)), NUM2INT(active));
    return self;
}

void 
Init_gtk_menu()
{
    VALUE gMenu = G_DEF_CLASS(GTK_TYPE_MENU, "Menu", mGtk);

    rb_define_method(gMenu, "initialize", menu_initialize, 0);
    rb_define_method(gMenu, "append", menu_append, 1);
    rb_define_method(gMenu, "prepend", menu_prepend, 1);
    rb_define_method(gMenu, "insert", menu_insert, 2);
    rb_define_method(gMenu, "popup", menu_popup, 5);
    rb_define_method(gMenu, "popdown", menu_popdown, 0);
    rb_define_method(gMenu, "get_active", menu_get_active, 0);
    rb_define_method(gMenu, "set_active", menu_set_active, 1);

    call = rb_intern("call");
}
