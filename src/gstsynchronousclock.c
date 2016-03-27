/*
 * GStreamer
 * Copyright (C) 2005 Thomas Vander Stichele <thomas@apestaart.org>
 * Copyright (C) 2005 Ronald S. Bultje <rbultje@ronald.bitfreak.net>
 * Copyright (C) 2016 Rodrigo Costa <rodrigocosta@telemidia.puc-rio.br>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Alternatively, the contents of this file may be used under the
 * GNU Lesser General Public License Version 2.1 (the "LGPL"), in
 * which case the following provisions apply instead of the ones
 * mentioned above:
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * SECTION:element-synchronousclock
 *
 * FIXME:Describe synchronousclock here.
 *
 * <refsect2>
 * <title>Example launch line</title>
 * |[
 * gst-launch -v -m fakesrc ! synchronousclock ! fakesink silent=TRUE
 * ]|
 * </refsect2>
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gst/gst.h>
#include <stdio.h>
#include "gstsynchronousclock.h"

#define LOCK_CLOCK(p)    g_mutex_lock(&p->priv->mutex);
#define UNLOCK_CLOCK(p)  g_mutex_unlock(&p->priv->mutex);

GST_DEBUG_CATEGORY_STATIC (gst_synchronous_clock_debug);
#define GST_CAT_DEFAULT gst_synchronous_clock_debug

static const char *short_description = "A deterministic clock";

enum
{
  PROP_0,
  PROP_SILENT
};

struct _GstSynchronousClockPrivate 
{
  uint64_t cur_time;
  GMutex mutex;
};

G_DEFINE_TYPE (GstSynchronousClock, gst_synchronous_clock,
    GST_TYPE_SYSTEM_CLOCK)

static void gst_synchronous_clock_set_property (GObject *, guint,
    const GValue *, GParamSpec *);
static void gst_synchronous_clock_get_property (GObject *, guint,GValue *,
    GParamSpec *);
static GstClockTime synchronous_clock_get_internal_time (GstClock *);
static void synchronous_clock_finalize (GObject *);

/* GObject vmethod implementations */

/* initialize the synchronousclock's class */
static void
gst_synchronous_clock_class_init (GstSynchronousClockClass * klass)
{
  GObjectClass *gobject_class;
  GstClockClass *clock_class;

  gobject_class = (GObjectClass *) klass;
  clock_class = (GstClockClass *) klass;

  gobject_class->finalize = synchronous_clock_finalize;
  gobject_class->set_property = gst_synchronous_clock_set_property;
  gobject_class->get_property = gst_synchronous_clock_get_property;

  clock_class->get_internal_time = synchronous_clock_get_internal_time;
}

static GstClockTime 
synchronous_clock_get_internal_time (GstClock *clock)
{
  GstSynchronousClock *myclock = GST_SYNCHRONOUSCLOCK (clock);
  GstClockTime time;
 
  LOCK_CLOCK (myclock);
  time = myclock->priv->cur_time;
  UNLOCK_CLOCK (myclock);
  return time;
}

/* initialize the new element
 * instantiate pads and add them to element
 * set pad calback functions
 * initialize instance structure
 */
static void
gst_synchronous_clock_init (GstSynchronousClock * self)
{
  self->priv = g_new0 (GstSynchronousClockPrivate, 1);
  g_mutex_init (&self->priv->mutex);
}

static void
synchronous_clock_finalize (GObject * object)
{
  GstSynchronousClock *self = GST_SYNCHRONOUSCLOCK (object);
  g_free (self->priv);
  g_mutex_clear (&self->priv->mutex);
}

static void
gst_synchronous_clock_set_property (GObject * object, guint prop_id,
    const GValue * value, GParamSpec * pspec)
{
  GstSynchronousClock *clock = GST_SYNCHRONOUSCLOCK (object);
  (void) clock;

  switch (prop_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
gst_synchronous_clock_get_property (GObject * object, guint prop_id,
    GValue * value, GParamSpec * pspec)
{
  GstSynchronousClock *clock = GST_SYNCHRONOUSCLOCK (object);
  (void) clock;

  switch (prop_id)
  {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

/* GstElement vmethod implementations */

/* entry point to initialize the plug-in
 * initialize the plug-in itself
 * register the element factories and other features
 */
static gboolean
synchronousclock_init (GstPlugin * myclock)
{
  /* debug category for fltering log messages
   *
   * exchange the string 'Template synchronousclock' with your description
   */
  GST_DEBUG_CATEGORY_INIT (gst_synchronous_clock_debug, "synchronousclock",
      0, short_description);
  return TRUE;
}

GstClock *
gst_synchronous_clock_new ()
{
  GstClock *ret;
  ret = g_object_new (GST_TYPE_SYNCHRONOUSCLOCK, NULL);
  return ret;
}

gboolean
gst_synchronous_clock_advance_time (GstClock *clock, uint64_t time)
{
  GstSynchronousClock *my_clock;
  g_return_val_if_fail (GST_IS_SYNCHRONOUSCLOCK(clock), FALSE);
  my_clock = GST_SYNCHRONOUSCLOCK (clock);
  LOCK_CLOCK (my_clock);
  my_clock->priv->cur_time += time;
  GST_DEBUG ("%" GST_TIME_FORMAT "\n", 
      GST_TIME_ARGS (my_clock->priv->cur_time));
  UNLOCK_CLOCK (my_clock);

  return TRUE;
}

/* PACKAGE: this is usually set by autotools depending on some _INIT macro
 * in configure.ac and then written into and defined in config.h, but we can
 * just set it ourselves here in case someone doesn't use autotools to
 * compile this code. GST_PLUGIN_DEFINE needs PACKAGE to be defined.
 */
#ifndef PACKAGE
#define PACKAGE "myfirstsynchronousclock"
#endif

/* gstreamer looks for this structure to register synchronousclocks
 *
 * exchange the string 'Template synchronousclock' with your synchronousclock description
 */
GST_PLUGIN_DEFINE (
    GST_VERSION_MAJOR,
    GST_VERSION_MINOR,
    myclock,
    "A deterministic clock",
    synchronousclock_init,
    VERSION,
    "LGPL",
    "GStreamer",
    "http://gstreamer.net/"
)
