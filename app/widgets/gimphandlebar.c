/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <gtk/gtk.h>

#include "libgimpmath/gimpmath.h"

#include "widgets-types.h"

#include "gimphandlebar.h"


enum
{
  PROP_0,
  PROP_ORIENTATION
};


/*  local function prototypes  */

static void      gimp_handle_bar_set_property       (GObject        *object,
                                                     guint           property_id,
                                                     const GValue   *value,
                                                     GParamSpec     *pspec);
static void      gimp_handle_bar_get_property       (GObject        *object,
                                                     guint           property_id,
                                                     GValue         *value,
                                                     GParamSpec     *pspec);

static gboolean  gimp_handle_bar_expose             (GtkWidget      *widget,
                                                     GdkEventExpose *eevent);
static gboolean  gimp_handle_bar_button_press       (GtkWidget      *widget,
                                                     GdkEventButton *bevent);
static gboolean  gimp_handle_bar_button_release     (GtkWidget      *widget,
                                                     GdkEventButton *bevent);
static gboolean  gimp_handle_bar_motion_notify      (GtkWidget      *widget,
                                                     GdkEventMotion *mevent);

static void      gimp_handle_bar_adjustment_changed (GtkAdjustment  *adjustment,
                                                     GimpHandleBar  *bar);


G_DEFINE_TYPE (GimpHandleBar, gimp_handle_bar, GTK_TYPE_EVENT_BOX)

#define parent_class gimp_handle_bar_parent_class


static void
gimp_handle_bar_class_init (GimpHandleBarClass *klass)
{
  GObjectClass   *object_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  object_class->set_property         = gimp_handle_bar_set_property;
  object_class->get_property         = gimp_handle_bar_get_property;

  widget_class->expose_event         = gimp_handle_bar_expose;
  widget_class->button_press_event   = gimp_handle_bar_button_press;
  widget_class->button_release_event = gimp_handle_bar_button_release;
  widget_class->motion_notify_event  = gimp_handle_bar_motion_notify;

  g_object_class_install_property (object_class, PROP_ORIENTATION,
                                   g_param_spec_enum ("orientation",
                                                      NULL, NULL,
                                                      GTK_TYPE_ORIENTATION,
                                                      GTK_ORIENTATION_HORIZONTAL,
                                                      GIMP_PARAM_READWRITE |
                                                      G_PARAM_CONSTRUCT_ONLY));
}

static void
gimp_handle_bar_init (GimpHandleBar *bar)
{
  gtk_widget_add_events (GTK_WIDGET (bar),
                         GDK_BUTTON_PRESS_MASK   |
                         GDK_BUTTON_RELEASE_MASK |
                         GDK_BUTTON_MOTION_MASK);

  gtk_event_box_set_visible_window (GTK_EVENT_BOX (bar), FALSE);

  bar->orientation = GTK_ORIENTATION_HORIZONTAL;
  bar->lower       = 0.0;
  bar->upper       = 1.0;
}

static void
gimp_handle_bar_set_property (GObject      *object,
                              guint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
  GimpHandleBar *bar = GIMP_HANDLE_BAR (object);

  switch (property_id)
    {
    case PROP_ORIENTATION:
      bar->orientation = g_value_get_enum (value);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static void
gimp_handle_bar_get_property (GObject    *object,
                              guint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
  GimpHandleBar *bar = GIMP_HANDLE_BAR (object);

  switch (property_id)
    {
    case PROP_ORIENTATION:
      g_value_set_enum (value, bar->orientation);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
      break;
    }
}

static gboolean
gimp_handle_bar_expose (GtkWidget      *widget,
                        GdkEventExpose *eevent)
{
  GimpHandleBar *bar = GIMP_HANDLE_BAR (widget);
  cairo_t       *cr;
  gint           x, y;
  gint           width, height;
  gint           i;

  x = y = gtk_container_get_border_width (GTK_CONTAINER (widget));

  width  = widget->allocation.width  - 2 * x;
  height = widget->allocation.height - 2 * y;

  if (GTK_WIDGET_NO_WINDOW (widget))
    {
      x += widget->allocation.x;
      y += widget->allocation.y;
    }

  cr = gdk_cairo_create (widget->window);

  gdk_cairo_region (cr, eevent->region);
  cairo_clip (cr);

  cairo_set_line_width (cr, 1.0);
  cairo_translate (cr, 0.5, 0.5);

  for (i = 0; i < 3; i++)
    {
      bar->slider_pos[i] = -1;

      if (bar->slider_adj[i])
        {
          bar->slider_pos[i] = ROUND ((gdouble) width *
                                      (bar->slider_adj[i]->value - bar->lower) /
                                      (bar->upper - bar->lower + 1));

          cairo_set_source_rgb (cr, 0.5 * i, 0.5 * i, 0.5 * i);

          cairo_move_to (cr,
                         x + bar->slider_pos[i],
                         y);
          cairo_line_to (cr,
                         x + bar->slider_pos[i] - (height - 1) / 2,
                         y + height - 1);
          cairo_line_to (cr,
                         x + bar->slider_pos[i] + (height - 1) / 2,
                         y + height - 1);
          cairo_line_to (cr,
                         x + bar->slider_pos[i],
                         y);

          cairo_fill_preserve (cr);

          cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);

          cairo_stroke (cr);
        }
    }

  cairo_destroy (cr);

  return FALSE;
}

static gboolean
gimp_handle_bar_button_press (GtkWidget      *widget,
                              GdkEventButton *bevent)
{
  GimpHandleBar *bar    = GIMP_HANDLE_BAR (widget);
  gint           border = gtk_container_get_border_width (GTK_CONTAINER (widget));
  gint           width  = widget->allocation.width - 2 * border;
  gdouble        value;
  gint           min_dist;
  gint           i;

  if (width < 1)
    return FALSE;

  min_dist = G_MAXINT;
  for (i = 0; i < 3; i++)
    if (bar->slider_pos[i] != -1)
      {
        gdouble dist = bevent->x - bar->slider_pos[i] + border;

        if (fabs (dist) < min_dist ||
            (fabs (dist) == min_dist && dist > 0))
          {
            bar->active_slider = i;
            min_dist = fabs (dist);
          }
      }

  value = ((gdouble) (bevent->x - border) /
           (gdouble) width *
           (bar->upper - bar->lower + 1));

  gtk_adjustment_set_value (bar->slider_adj[bar->active_slider], value);

  return FALSE;
}

static gboolean
gimp_handle_bar_button_release (GtkWidget      *widget,
                                GdkEventButton *bevent)
{
  return FALSE;
}

static gboolean
gimp_handle_bar_motion_notify (GtkWidget      *widget,
                               GdkEventMotion *mevent)
{
  GimpHandleBar *bar    = GIMP_HANDLE_BAR (widget);
  gint           border = gtk_container_get_border_width (GTK_CONTAINER (widget));
  gint           width  = widget->allocation.width - 2 * border;
  gdouble        value;

  if (width < 1)
    return FALSE;

  value = ((gdouble) (mevent->x - border) /
           (gdouble) width *
           (bar->upper - bar->lower + 1));

  gtk_adjustment_set_value (bar->slider_adj[bar->active_slider], value);

  return FALSE;
}


/*  public functions  */

/**
 * gimp_handle_bar_new:
 * @orientation: whether the bar should be oriented horizontally or
 *               vertically
 *
 * Creates a new #GimpHandleBar widget.
 *
 * Return value: The new #GimpHandleBar widget.
 **/
GtkWidget *
gimp_handle_bar_new (GtkOrientation  orientation)
{
  return g_object_new (GIMP_TYPE_HANDLE_BAR,
                       "orientation", orientation,
                       NULL);
}

void
gimp_handle_bar_set_adjustment (GimpHandleBar  *bar,
                                gint            handle_no,
                                GtkAdjustment  *adjustment)
{
  g_return_if_fail (GIMP_IS_HANDLE_BAR (bar));
  g_return_if_fail (handle_no >= 0 && handle_no <= 2);
  g_return_if_fail (adjustment == NULL || GTK_IS_ADJUSTMENT (adjustment));

  if (adjustment == bar->slider_adj[handle_no])
    return;

  if (bar->slider_adj[handle_no])
    {
      g_signal_handlers_disconnect_by_func (bar->slider_adj[handle_no],
                                            gimp_handle_bar_adjustment_changed,
                                            bar);
      g_object_unref (bar->slider_adj[handle_no]);
      bar->slider_adj[handle_no] = NULL;
    }

    bar->slider_adj[handle_no] = adjustment;

    if (bar->slider_adj[handle_no])
      {
        g_object_ref (bar->slider_adj[handle_no]);

        g_signal_connect (bar->slider_adj[handle_no], "value-changed",
                          G_CALLBACK (gimp_handle_bar_adjustment_changed),
                          bar);
      }

    if (bar->slider_adj[0])
      bar->lower = bar->slider_adj[0]->lower;
    else
      bar->lower = bar->slider_adj[handle_no]->lower;

    if (bar->slider_adj[2])
      bar->upper = bar->slider_adj[2]->upper;
    else
      bar->upper = bar->slider_adj[handle_no]->upper;

    gimp_handle_bar_adjustment_changed (bar->slider_adj[handle_no], bar);
}


/*  private functions  */

static void
gimp_handle_bar_adjustment_changed (GtkAdjustment *adjustment,
                                    GimpHandleBar *bar)
{
  gtk_widget_queue_draw (GTK_WIDGET (bar));
}
