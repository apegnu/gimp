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

#include "libgimpbase/gimpbase.h"
#include "libgimpwidgets/gimpwidgets.h"

#include "dialogs-types.h"

#include "config/gimpguiconfig.h"

#include "core/gimp.h"

#include "widgets/gimphelp-ids.h"

#include "tips-dialog.h"
#include "tips-parser.h"

#include "gimp-intl.h"

enum
{
  RESPONSE_PREVIOUS = 1,
  RESPONSE_NEXT     = 2
};

static void  tips_dialog_set_tip  (GimpTip       *tip);
static void  tips_dialog_response (GtkWidget     *dialog,
                                   gint           response);
static void  tips_dialog_destroy  (GtkWidget     *widget,
                                   GimpGuiConfig *config);
static void  more_button_clicked  (GtkWidget     *button,
                                   Gimp          *gimp);


static GtkWidget *tips_dialog = NULL;
static GtkWidget *tip_label   = NULL;
static GtkWidget *more_button = NULL;
static GList     *tips        = NULL;
static GList     *current_tip = NULL;


GtkWidget *
tips_dialog_create (Gimp *gimp)
{
  GimpGuiConfig *config;
  GtkWidget     *vbox;
  GtkWidget     *hbox;
  GtkWidget     *button;
  GtkWidget     *image;
  gint           tips_count;

  g_return_val_if_fail (GIMP_IS_GIMP (gimp), NULL);

  if (!tips)
    {
      GError *error = NULL;
      gchar  *filename;

      filename = g_build_filename (gimp_data_directory (), "tips",
                                   "gimp-tips.xml", NULL);

      tips = gimp_tips_from_file (filename, &error);

      if (! tips)
        {
          GimpTip *tip;

          if (! error)
            {
              tip = gimp_tip_new (_("The GIMP tips file is empty!"), NULL);
            }
          else if (error->code == G_FILE_ERROR_NOENT)
            {
              tip = gimp_tip_new (_("The GIMP tips file appears to be "
                                    "missing!"),
                                  _("There should be a file called '%s'. "
                                    "Please check your installation."),
                                  gimp_filename_to_utf8 (filename));
            }
          else
            {
              tip = gimp_tip_new (_("The GIMP tips file could not be parsed!"),
                                  "%s", error->message);
            }

          tips = g_list_prepend (tips, tip);
        }
      else if (error)
        {
          g_printerr ("Error while parsing '%s': %s\n",
                      filename, error->message);
        }

      g_clear_error (&error);
      g_free (filename);
    }

  tips_count = g_list_length (tips);

  config = GIMP_GUI_CONFIG (gimp->config);

  if (config->last_tip >= tips_count || config->last_tip < 0)
    config->last_tip = 0;

  current_tip = g_list_nth (tips, config->last_tip);

  if (tips_dialog)
    return tips_dialog;

  tips_dialog = gimp_dialog_new (_("GIMP Tip of the Day"),
                                 "gimp-tip-of-the-day",
                                 NULL, 0, NULL, NULL,
                                 NULL);

  button = gtk_dialog_add_button (GTK_DIALOG (tips_dialog),
                                  _("_Previous Tip"), RESPONSE_PREVIOUS);
  gtk_button_set_image (GTK_BUTTON (button),
                        gtk_image_new_from_stock (GTK_STOCK_GO_BACK,
                                                  GTK_ICON_SIZE_BUTTON));

  button = gtk_dialog_add_button (GTK_DIALOG (tips_dialog),
                                  _("_Next Tip"), RESPONSE_NEXT);
  gtk_button_set_image (GTK_BUTTON (button),
                        gtk_image_new_from_stock (GTK_STOCK_GO_FORWARD,
                                                  GTK_ICON_SIZE_BUTTON));

  gtk_dialog_set_response_sensitive (GTK_DIALOG (tips_dialog),
                                     RESPONSE_NEXT, tips_count > 1);
  gtk_dialog_set_response_sensitive (GTK_DIALOG (tips_dialog),
                                     RESPONSE_PREVIOUS, tips_count > 1);

  g_signal_connect (tips_dialog, "response",
                    G_CALLBACK (tips_dialog_response),
                    NULL);
  g_signal_connect (tips_dialog, "destroy",
                    G_CALLBACK (tips_dialog_destroy),
                    config);

  vbox = gtk_vbox_new (FALSE, 12);
  gtk_container_set_border_width (GTK_CONTAINER (vbox), 12);
  gtk_box_pack_start (GTK_BOX (GTK_DIALOG (tips_dialog)->vbox),
                      vbox, TRUE, TRUE, 0);
  gtk_widget_show (vbox);

  hbox = gtk_hbox_new (FALSE, 6);
  gtk_container_set_border_width (GTK_CONTAINER (hbox), 6);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, TRUE, TRUE, 0);
  gtk_widget_show (hbox);

  vbox = gtk_vbox_new (FALSE, 6);
  gtk_box_pack_start (GTK_BOX (hbox), vbox, TRUE, TRUE, 0);
  gtk_widget_show (vbox);

  image = gtk_image_new_from_stock (GIMP_STOCK_INFO, GTK_ICON_SIZE_DIALOG);
  gtk_misc_set_alignment (GTK_MISC (image), 0.5, 0.0);
  gtk_box_pack_start (GTK_BOX (hbox), image, FALSE, FALSE, 0);
  gtk_widget_show (image);

  gtk_container_set_focus_chain (GTK_CONTAINER (hbox), NULL);

  tip_label = gtk_label_new (NULL);
  gtk_label_set_selectable (GTK_LABEL (tip_label), TRUE);
  gtk_label_set_justify (GTK_LABEL (tip_label), GTK_JUSTIFY_LEFT);
  gtk_label_set_line_wrap (GTK_LABEL (tip_label), TRUE);
  gtk_misc_set_alignment (GTK_MISC (tip_label), 0.5, 0.0);
  gtk_box_pack_start (GTK_BOX (vbox), tip_label, TRUE, TRUE, 0);
  gtk_widget_show (tip_label);

  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  gtk_widget_show (hbox);

  more_button = gtk_link_button_new_with_label ("http://docs.gimp.org/",
  /*  a link to the related section in the user manual  */
                                                _("Learn more"));
  gtk_widget_show (more_button);
  gtk_box_pack_start (GTK_BOX (hbox), more_button, FALSE, FALSE, 0);

  g_signal_connect (more_button, "clicked",
                    G_CALLBACK (more_button_clicked),
                    gimp);

  tips_dialog_set_tip (current_tip->data);

  return tips_dialog;
}

static void
tips_dialog_destroy (GtkWidget     *widget,
                     GimpGuiConfig *config)
{
  /* the last-shown-tip is saved in sessionrc */
  config->last_tip = g_list_position (tips, current_tip);

  tips_dialog = NULL;
  current_tip = NULL;

  gimp_tips_free (tips);
  tips = NULL;
}

static void
tips_dialog_response (GtkWidget *dialog,
                      gint       response)
{
  switch (response)
    {
    case RESPONSE_PREVIOUS:
      current_tip = current_tip->prev ? current_tip->prev : g_list_last (tips);
      tips_dialog_set_tip (current_tip->data);
      break;

    case RESPONSE_NEXT:
      current_tip = current_tip->next ? current_tip->next : tips;
      tips_dialog_set_tip (current_tip->data);
      break;

    default:
      gtk_widget_destroy (dialog);
      break;
    }
}

static void
tips_dialog_set_tip (GimpTip *tip)
{
  g_return_if_fail (tip != NULL);

  gtk_label_set_markup (GTK_LABEL (tip_label), tip->text);

  /*  set the URI to unset the "visited" state  */
  gtk_link_button_set_uri (GTK_LINK_BUTTON (more_button),
                           "http://docs.gimp.org/");

  gtk_widget_set_sensitive (more_button, tip->help_id != NULL);
}

static void
more_button_clicked (GtkWidget *button,
                     Gimp      *gimp)
{
  GimpTip *tip = current_tip->data;

  if (tip->help_id)
    gimp_help (gimp, NULL, NULL, tip->help_id);
}
