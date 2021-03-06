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

#include <string.h>

#include <gtk/gtk.h>

#include "libgimpwidgets/gimpwidgets.h"

#include "actions-types.h"

#include "widgets/gimpactiongroup.h"
#include "widgets/gimpcontainerview.h"
#include "widgets/gimpcontainerview-utils.h"
#include "widgets/gimpdialogfactory.h"
#include "widgets/gimpdock.h"
#include "widgets/gimpdockable.h"
#include "widgets/gimpdockbook.h"
#include "widgets/gimpdocked.h"
#include "widgets/gimphelp-ids.h"

#include "dialogs-actions.h"
#include "dockable-actions.h"
#include "dockable-commands.h"

#include "gimp-intl.h"


static const GimpActionEntry dockable_actions[] =
{
  { "dockable-popup", NULL,
    N_("Dialogs Menu"), NULL, NULL, NULL,
    GIMP_HELP_DOCK },

  { "dockable-menu",               GTK_STOCK_MISSING_IMAGE, "" },
  { "dockable-add-tab-menu",       NULL, N_("_Add Tab")        },
  { "dockable-preview-size-menu",  NULL, N_("_Preview Size")   },
  { "dockable-tab-style-menu",     NULL, N_("_Tab Style")      },

  { "dockable-close-tab", GTK_STOCK_CLOSE,
    N_("_Close Tab"), "", NULL,
    G_CALLBACK (dockable_close_tab_cmd_callback),
    GIMP_HELP_DOCK_TAB_CLOSE },

  { "dockable-detach-tab", GIMP_STOCK_DETACH,
    N_("_Detach Tab"), "", NULL,
    G_CALLBACK (dockable_detach_tab_cmd_callback),
    GIMP_HELP_DOCK_TAB_DETACH }
};

#define VIEW_SIZE(action,label,size) \
  { "dockable-preview-size-" action, NULL, \
    (label), NULL, NULL, \
    (size), \
    GIMP_HELP_DOCK_PREVIEW_SIZE }
#define TAB_STYLE(action,label,style) \
  { "dockable-tab-style-" action, NULL, \
    (label), NULL, NULL, \
    (style), \
    GIMP_HELP_DOCK_TAB_STYLE }

static const GimpRadioActionEntry dockable_view_size_actions[] =
{
  VIEW_SIZE ("tiny",        N_("_Tiny"),        GIMP_VIEW_SIZE_TINY),
  VIEW_SIZE ("extra-small", N_("E_xtra Small"), GIMP_VIEW_SIZE_EXTRA_SMALL),
  VIEW_SIZE ("small",       N_("_Small"),       GIMP_VIEW_SIZE_SMALL),
  VIEW_SIZE ("medium",      N_("_Medium"),      GIMP_VIEW_SIZE_MEDIUM),
  VIEW_SIZE ("large",       N_("_Large"),       GIMP_VIEW_SIZE_LARGE),
  VIEW_SIZE ("extra-large", N_("Ex_tra Large"), GIMP_VIEW_SIZE_EXTRA_LARGE),
  VIEW_SIZE ("huge",        N_("_Huge"),        GIMP_VIEW_SIZE_HUGE),
  VIEW_SIZE ("enormous",    N_("_Enormous"),    GIMP_VIEW_SIZE_ENORMOUS),
  VIEW_SIZE ("gigantic",    N_("_Gigantic"),    GIMP_VIEW_SIZE_GIGANTIC)
};

static const GimpRadioActionEntry dockable_tab_style_actions[] =
{
  TAB_STYLE ("icon",         N_("_Icon"),           GIMP_TAB_STYLE_ICON),
  TAB_STYLE ("preview",      N_("Current _Status"), GIMP_TAB_STYLE_PREVIEW),
  TAB_STYLE ("name",         N_("_Text"),           GIMP_TAB_STYLE_NAME),
  TAB_STYLE ("icon-name",    N_("I_con & Text"),    GIMP_TAB_STYLE_ICON_NAME),
  TAB_STYLE ("preview-name", N_("St_atus & Text"),  GIMP_TAB_STYLE_PREVIEW_NAME)
};

#undef VIEW_SIZE
#undef TAB_STYLE


static const GimpToggleActionEntry dockable_toggle_actions[] =
{
  { "dockable-lock-tab", NULL,
    N_("Loc_k Tab to Dock"), NULL,
    N_("Protect this tab from being dragged with the mouse pointer"),
    G_CALLBACK (dockable_lock_tab_cmd_callback),
    FALSE,
    GIMP_HELP_DOCK_TAB_LOCK },

  { "dockable-show-button-bar", NULL,
    N_("Show _Button Bar"), NULL, NULL,
    G_CALLBACK (dockable_show_button_bar_cmd_callback),
    TRUE,
    GIMP_HELP_DOCK_SHOW_BUTTON_BAR }
};

static const GimpRadioActionEntry dockable_view_type_actions[] =
{
  { "dockable-view-type-list", NULL,
    N_("View as _List"), NULL, NULL,
    GIMP_VIEW_TYPE_LIST,
    GIMP_HELP_DOCK_VIEW_AS_LIST },

  { "dockable-view-type-grid", NULL,
    N_("View as _Grid"), NULL, NULL,
    GIMP_VIEW_TYPE_GRID,
    GIMP_HELP_DOCK_VIEW_AS_GRID }
};


void
dockable_actions_setup (GimpActionGroup *group)
{
  gimp_action_group_add_actions (group,
                                 dockable_actions,
                                 G_N_ELEMENTS (dockable_actions));

  gimp_action_group_add_toggle_actions (group,
                                        dockable_toggle_actions,
                                        G_N_ELEMENTS (dockable_toggle_actions));

  gimp_action_group_add_string_actions (group,
                                        dialogs_dockable_actions,
                                        n_dialogs_dockable_actions,
                                        G_CALLBACK (dockable_add_tab_cmd_callback));

  gimp_action_group_add_radio_actions (group,
                                       dockable_view_size_actions,
                                       G_N_ELEMENTS (dockable_view_size_actions),
                                       NULL,
                                       GIMP_VIEW_SIZE_MEDIUM,
                                       G_CALLBACK (dockable_view_size_cmd_callback));

  gimp_action_group_add_radio_actions (group,
                                       dockable_tab_style_actions,
                                       G_N_ELEMENTS (dockable_tab_style_actions),
                                       NULL,
                                       GIMP_TAB_STYLE_PREVIEW,
                                       G_CALLBACK (dockable_tab_style_cmd_callback));

  gimp_action_group_add_radio_actions (group,
                                       dockable_view_type_actions,
                                       G_N_ELEMENTS (dockable_view_type_actions),
                                       NULL,
                                       GIMP_VIEW_TYPE_LIST,
                                       G_CALLBACK (dockable_toggle_view_cmd_callback));
}

void
dockable_actions_update (GimpActionGroup *group,
                         gpointer         data)
{
  GimpDockable           *dockable;
  GimpDockbook           *dockbook;
  GimpDocked             *docked;
  GimpDialogFactoryEntry *entry;
  GimpContainerView      *view;
  GimpViewType            view_type           = -1;
  gboolean                list_view_available = FALSE;
  gboolean                grid_view_available = FALSE;
  gboolean                locked              = FALSE;
  GimpViewSize            view_size           = -1;
  GimpTabStyle            tab_style           = -1;
  gint                    n_pages             = 0;
  gint                    n_books             = 0;

  if (GIMP_IS_DOCKBOOK (data))
    {
      gint page_num;

      dockbook = GIMP_DOCKBOOK (data);

      page_num = gtk_notebook_get_current_page (GTK_NOTEBOOK (dockbook));

      dockable = (GimpDockable *)
        gtk_notebook_get_nth_page (GTK_NOTEBOOK (dockbook), page_num);
    }
  else if (GIMP_IS_DOCKABLE (data))
    {
      dockable = GIMP_DOCKABLE (data);
      dockbook = dockable->dockbook;
    }
  else
    {
      return;
    }

  docked = GIMP_DOCKED (gtk_bin_get_child (GTK_BIN (dockable)));

  gimp_dialog_factory_from_widget (GTK_WIDGET (dockable), &entry);

  if (entry)
    {
      gchar *identifier;
      gchar *substring = NULL;

      identifier = g_strdup (entry->identifier);

      if ((substring = strstr (identifier, "grid")))
        view_type = GIMP_VIEW_TYPE_GRID;
      else if ((substring = strstr (identifier, "list")))
        view_type = GIMP_VIEW_TYPE_LIST;

      if (substring)
        {
          memcpy (substring, "list", 4);
          if (gimp_dialog_factory_find_entry (dockbook->dock->dialog_factory,
                                              identifier))
            list_view_available = TRUE;

          memcpy (substring, "grid", 4);
          if (gimp_dialog_factory_find_entry (dockbook->dock->dialog_factory,
                                              identifier))
            grid_view_available = TRUE;
        }

      g_free (identifier);
    }

  view = gimp_container_view_get_by_dockable (dockable);

  if (view)
    view_size = gimp_container_view_get_view_size (view, NULL);

  tab_style = dockable->tab_style;

  n_pages = gtk_notebook_get_n_pages (GTK_NOTEBOOK (dockbook));
  n_books = g_list_length (dockbook->dock->dockbooks);

#define SET_ACTIVE(action,active) \
        gimp_action_group_set_action_active (group, action, (active) != 0)
#define SET_VISIBLE(action,active) \
        gimp_action_group_set_action_visible (group, action, (active) != 0)
#define SET_SENSITIVE(action,sensitive) \
        gimp_action_group_set_action_sensitive (group, action, (sensitive) != 0)


  locked = gimp_dockable_is_locked (dockable);

  SET_SENSITIVE ("dockable-detach-tab", (! locked &&
                                         (n_pages > 1 || n_books > 1)));

  SET_ACTIVE ("dockable-lock-tab", locked);

  SET_VISIBLE ("dockable-preview-size-menu", view_size != -1);

  if (view_size != -1)
    {
      if (view_size >= GIMP_VIEW_SIZE_GIGANTIC)
        {
          SET_ACTIVE ("dockable-preview-size-gigantic", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_ENORMOUS)
        {
          SET_ACTIVE ("dockable-preview-size-enormous", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_HUGE)
        {
          SET_ACTIVE ("dockable-preview-size-huge", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_EXTRA_LARGE)
        {
          SET_ACTIVE ("dockable-preview-size-extra-large", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_LARGE)
        {
          SET_ACTIVE ("dockable-preview-size-large", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_MEDIUM)
        {
          SET_ACTIVE ("dockable-preview-size-medium", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_SMALL)
        {
          SET_ACTIVE ("dockable-preview-size-small", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_EXTRA_SMALL)
        {
          SET_ACTIVE ("dockable-preview-size-extra-small", TRUE);
        }
      else if (view_size >= GIMP_VIEW_SIZE_TINY)
        {
          SET_ACTIVE ("dockable-preview-size-tiny", TRUE);
        }
    }

  SET_VISIBLE ("dockable-tab-style-menu", n_pages > 1);

  if (n_pages > 1)
    {
      GimpDockedInterface *docked_iface = GIMP_DOCKED_GET_INTERFACE (docked);

      if (tab_style == GIMP_TAB_STYLE_ICON)
        SET_ACTIVE ("dockable-tab-style-icon", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_PREVIEW)
        SET_ACTIVE ("dockable-tab-style-preview", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_NAME)
        SET_ACTIVE ("dockable-tab-style-name", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_ICON_NAME)
        SET_ACTIVE ("dockable-tab-style-icon-name", TRUE);
      else if (tab_style == GIMP_TAB_STYLE_PREVIEW_NAME)
        SET_ACTIVE ("dockable-tab-style-preview-name", TRUE);

      SET_SENSITIVE ("dockable-tab-style-preview",
                     docked_iface->get_preview);
      SET_SENSITIVE ("dockable-tab-style-preview-name",
                     docked_iface->get_preview);
    }

  SET_VISIBLE ("dockable-view-type-grid", view_type != -1);
  SET_VISIBLE ("dockable-view-type-list", view_type != -1);

  if (view_type != -1)
    {
      if (view_type == GIMP_VIEW_TYPE_LIST)
        SET_ACTIVE ("dockable-view-type-list", TRUE);
      else
        SET_ACTIVE ("dockable-view-type-grid", TRUE);

      SET_SENSITIVE ("dockable-view-type-grid", grid_view_available);
      SET_SENSITIVE ("dockable-view-type-list", list_view_available);
    }

  SET_VISIBLE ("dockable-show-button-bar", gimp_docked_has_button_bar (docked));
  SET_ACTIVE ("dockable-show-button-bar",
              gimp_docked_get_show_button_bar (docked));

#undef SET_ACTIVE
#undef SET_VISIBLE
#undef SET_SENSITIVE
}
