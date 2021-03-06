/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995 Spencer Kimball and Peter Mattis
 *
 * gimpcurvesconfig.h
 * Copyright (C) 2007 Michael Natterer <mitch@gimp.org>
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

#ifndef __GIMP_CURVES_CONFIG_H__
#define __GIMP_CURVES_CONFIG_H__


#include "core/gimpimagemapconfig.h"


#define GIMP_TYPE_CURVES_CONFIG            (gimp_curves_config_get_type ())
#define GIMP_CURVES_CONFIG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GIMP_TYPE_CURVES_CONFIG, GimpCurvesConfig))
#define GIMP_CURVES_CONFIG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass),  GIMP_TYPE_CURVES_CONFIG, GimpCurvesConfigClass))
#define GIMP_IS_CURVES_CONFIG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GIMP_TYPE_CURVES_CONFIG))
#define GIMP_IS_CURVES_CONFIG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass),  GIMP_TYPE_CURVES_CONFIG))
#define GIMP_CURVES_CONFIG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj),  GIMP_TYPE_CURVES_CONFIG, GimpCurvesConfigClass))


typedef struct _GimpCurvesConfigClass GimpCurvesConfigClass;

struct _GimpCurvesConfig
{
  GimpImageMapConfig    parent_instance;

  GimpHistogramChannel  channel;

  GimpCurve            *curve[5];
};

struct _GimpCurvesConfigClass
{
  GimpImageMapConfigClass  parent_class;
};


GType      gimp_curves_config_get_type      (void) G_GNUC_CONST;

void       gimp_curves_config_reset_channel (GimpCurvesConfig  *config);

gboolean   gimp_curves_config_load_cruft    (GimpCurvesConfig  *config,
                                             gpointer           fp,
                                             GError           **error);
gboolean   gimp_curves_config_save_cruft    (GimpCurvesConfig  *config,
                                             gpointer           fp,
                                             GError           **error);


/*  temp cruft  */
void       gimp_curves_config_to_cruft      (GimpCurvesConfig  *config,
                                             Curves            *cruft,
                                             gboolean           is_color);


#endif /* __GIMP_CURVES_CONFIG_H__ */
