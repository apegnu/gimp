/* GIMP - The GNU Image Manipulation Program
 * Copyright (C) 1995-2003 Spencer Kimball and Peter Mattis
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

/* NOTE: This file is auto-generated by pdbgen.pl. */

#include "config.h"

#include <glib-object.h>

#include "pdb-types.h"

#include "core/gimpguide.h"
#include "core/gimpimage-guides.h"
#include "core/gimpimage-undo-push.h"
#include "core/gimpimage.h"
#include "core/gimpparamspecs.h"

#include "gimppdb.h"
#include "gimpprocedure.h"
#include "internal-procs.h"


static GValueArray *
image_add_hguide_invoker (GimpProcedure      *procedure,
                          Gimp               *gimp,
                          GimpContext        *context,
                          GimpProgress       *progress,
                          const GValueArray  *args,
                          GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 yposition;
  gint32 guide = 0;

  image = gimp_value_get_image (&args->values[0], gimp);
  yposition = g_value_get_int (&args->values[1]);

  if (success)
    {
      if (yposition <= gimp_image_get_height (image))
        {
          GimpGuide *g;

          g = gimp_image_add_hguide (image, yposition, TRUE);
          guide = gimp_guide_get_ID (g);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    g_value_set_uint (&return_vals->values[1], guide);

  return return_vals;
}

static GValueArray *
image_add_vguide_invoker (GimpProcedure      *procedure,
                          Gimp               *gimp,
                          GimpContext        *context,
                          GimpProgress       *progress,
                          const GValueArray  *args,
                          GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 xposition;
  gint32 guide = 0;

  image = gimp_value_get_image (&args->values[0], gimp);
  xposition = g_value_get_int (&args->values[1]);

  if (success)
    {
      if (xposition <= gimp_image_get_width (image))
        {
          GimpGuide *g;

          g = gimp_image_add_vguide (image, xposition, TRUE);
          guide = gimp_guide_get_ID (g);
        }
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    g_value_set_uint (&return_vals->values[1], guide);

  return return_vals;
}

static GValueArray *
image_delete_guide_invoker (GimpProcedure      *procedure,
                            Gimp               *gimp,
                            GimpContext        *context,
                            GimpProgress       *progress,
                            const GValueArray  *args,
                            GError            **error)
{
  gboolean success = TRUE;
  GimpImage *image;
  gint32 guide;

  image = gimp_value_get_image (&args->values[0], gimp);
  guide = g_value_get_uint (&args->values[1]);

  if (success)
    {
      GimpGuide *g = gimp_image_get_guide (image, guide);

      if (g)
        gimp_image_remove_guide (image, g, TRUE);
      else
        success = FALSE;
    }

  return gimp_procedure_get_return_values (procedure, success,
                                           error ? *error : NULL);
}

static GValueArray *
image_find_next_guide_invoker (GimpProcedure      *procedure,
                               Gimp               *gimp,
                               GimpContext        *context,
                               GimpProgress       *progress,
                               const GValueArray  *args,
                               GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 guide;
  gint32 next_guide = 0;

  image = gimp_value_get_image (&args->values[0], gimp);
  guide = g_value_get_uint (&args->values[1]);

  if (success)
    {
      GimpGuide *g = gimp_image_get_next_guide (image, guide, &success);

      if (g)
        next_guide = gimp_guide_get_ID (g);
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    g_value_set_uint (&return_vals->values[1], next_guide);

  return return_vals;
}

static GValueArray *
image_get_guide_orientation_invoker (GimpProcedure      *procedure,
                                     Gimp               *gimp,
                                     GimpContext        *context,
                                     GimpProgress       *progress,
                                     const GValueArray  *args,
                                     GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 guide;
  gint32 orientation = 0;

  image = gimp_value_get_image (&args->values[0], gimp);
  guide = g_value_get_uint (&args->values[1]);

  if (success)
    {
      GimpGuide *g = gimp_image_get_guide (image, guide);

      if (g)
        orientation = gimp_guide_get_orientation (g);
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    g_value_set_enum (&return_vals->values[1], orientation);

  return return_vals;
}

static GValueArray *
image_get_guide_position_invoker (GimpProcedure      *procedure,
                                  Gimp               *gimp,
                                  GimpContext        *context,
                                  GimpProgress       *progress,
                                  const GValueArray  *args,
                                  GError            **error)
{
  gboolean success = TRUE;
  GValueArray *return_vals;
  GimpImage *image;
  gint32 guide;
  gint32 position = 0;

  image = gimp_value_get_image (&args->values[0], gimp);
  guide = g_value_get_uint (&args->values[1]);

  if (success)
    {
      GimpGuide *g = gimp_image_get_guide (image, guide);

      if (g)
        position = gimp_guide_get_position (g);
      else
        success = FALSE;
    }

  return_vals = gimp_procedure_get_return_values (procedure, success,
                                                  error ? *error : NULL);

  if (success)
    g_value_set_int (&return_vals->values[1], position);

  return return_vals;
}

void
register_guides_procs (GimpPDB *pdb)
{
  GimpProcedure *procedure;

  /*
   * gimp-image-add-hguide
   */
  procedure = gimp_procedure_new (image_add_hguide_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-image-add-hguide");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-image-add-hguide",
                                     "Add a horizontal guide to an image.",
                                     "This procedure adds a horizontal guide to an image. It takes the input image and the y-position of the new guide as parameters. It returns the guide ID of the new guide.",
                                     "Adam D. Moss",
                                     "Adam D. Moss",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("yposition",
                                                      "yposition",
                                                      "The guide's y-offset from top of image",
                                                      0, G_MAXINT32, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_uint ("guide",
                                                      "guide",
                                                      "The new guide",
                                                      1, G_MAXUINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-image-add-vguide
   */
  procedure = gimp_procedure_new (image_add_vguide_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-image-add-vguide");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-image-add-vguide",
                                     "Add a vertical guide to an image.",
                                     "This procedure adds a vertical guide to an image. It takes the input image and the x-position of the new guide as parameters. It returns the guide ID of the new guide.",
                                     "Adam D. Moss",
                                     "Adam D. Moss",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_int32 ("xposition",
                                                      "xposition",
                                                      "The guide's x-offset from left of image",
                                                      0, G_MAXINT32, 0,
                                                      GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_uint ("guide",
                                                      "guide",
                                                      "The new guide",
                                                      1, G_MAXUINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-image-delete-guide
   */
  procedure = gimp_procedure_new (image_delete_guide_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-image-delete-guide");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-image-delete-guide",
                                     "Deletes a guide from an image.",
                                     "This procedure takes an image and a guide ID as input and removes the specified guide from the specified image.",
                                     "Adam D. Moss",
                                     "Adam D. Moss",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_uint ("guide",
                                                  "guide",
                                                  "The ID of the guide to be removed",
                                                  1, G_MAXUINT32, 1,
                                                  GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-image-find-next-guide
   */
  procedure = gimp_procedure_new (image_find_next_guide_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-image-find-next-guide");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-image-find-next-guide",
                                     "Find next guide on an image.",
                                     "This procedure takes an image and a guide ID as input and finds the guide ID of the successor of the given guide ID in the image's guide list. If the supplied guide ID is 0, the procedure will return the first Guide. The procedure will return 0 if given the final guide ID as an argument or the image has no guides.",
                                     "Adam D. Moss",
                                     "Adam D. Moss",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_uint ("guide",
                                                  "guide",
                                                  "The ID of the current guide (0 if first invocation)",
                                                  1, G_MAXUINT32, 1,
                                                  GIMP_PARAM_READWRITE | GIMP_PARAM_NO_VALIDATE));
  gimp_procedure_add_return_value (procedure,
                                   g_param_spec_uint ("next-guide",
                                                      "next guide",
                                                      "The next guide's ID",
                                                      1, G_MAXUINT32, 1,
                                                      GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-image-get-guide-orientation
   */
  procedure = gimp_procedure_new (image_get_guide_orientation_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-image-get-guide-orientation");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-image-get-guide-orientation",
                                     "Get orientation of a guide on an image.",
                                     "This procedure takes an image and a guide ID as input and returns the orientations of the guide.",
                                     "Adam D. Moss",
                                     "Adam D. Moss",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_uint ("guide",
                                                  "guide",
                                                  "The guide",
                                                  1, G_MAXUINT32, 1,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_enum ("orientation",
                                                         "orientation",
                                                         "The guide's orientation",
                                                         GIMP_TYPE_ORIENTATION_TYPE,
                                                         GIMP_ORIENTATION_HORIZONTAL,
                                                         GIMP_PARAM_READWRITE));
  gimp_param_spec_enum_exclude_value (GIMP_PARAM_SPEC_ENUM (procedure->values[0]),
                                      GIMP_ORIENTATION_UNKNOWN);
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);

  /*
   * gimp-image-get-guide-position
   */
  procedure = gimp_procedure_new (image_get_guide_position_invoker);
  gimp_object_set_static_name (GIMP_OBJECT (procedure),
                               "gimp-image-get-guide-position");
  gimp_procedure_set_static_strings (procedure,
                                     "gimp-image-get-guide-position",
                                     "Get position of a guide on an image.",
                                     "This procedure takes an image and a guide ID as input and returns the position of the guide relative to the top or left of the image.",
                                     "Adam D. Moss",
                                     "Adam D. Moss",
                                     "1998",
                                     NULL);
  gimp_procedure_add_argument (procedure,
                               gimp_param_spec_image_id ("image",
                                                         "image",
                                                         "The image",
                                                         pdb->gimp, FALSE,
                                                         GIMP_PARAM_READWRITE));
  gimp_procedure_add_argument (procedure,
                               g_param_spec_uint ("guide",
                                                  "guide",
                                                  "The guide",
                                                  1, G_MAXUINT32, 1,
                                                  GIMP_PARAM_READWRITE));
  gimp_procedure_add_return_value (procedure,
                                   gimp_param_spec_int32 ("position",
                                                          "position",
                                                          "The guide's position relative to top or left of image",
                                                          G_MININT32, G_MAXINT32, 0,
                                                          GIMP_PARAM_READWRITE));
  gimp_pdb_register_procedure (pdb, procedure);
  g_object_unref (procedure);
}
