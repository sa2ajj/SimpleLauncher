// This file is a part of Simple Launcher
//
// Copyright (C) 2006, 2007, Mikhail Sobolev
//
// Simple Launcher is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License version 2 as published by
// the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
// more details.
//
// You should have received a copy of the GNU General Public License along with
// this program; if not, write to the Free Software Foundation, Inc., 51
// Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#include <gtk/gtkcheckbutton.h>
#include <gtk/gtkcombobox.h>

#include "dialog-entry.h"

SettingsDialogBooleanEntry::SettingsDialogBooleanEntry(GConfBooleanOption& option, const std::string& name): SettingsDialogEntry(option, name) {
	myWidget = gtk_check_button_new();

	gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(myWidget), option.value());
}

void SettingsDialogBooleanEntry::updateValue() {
	((GConfBooleanOption&)myOption).setValue(gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(myWidget)));
}

SettingsDialogIntegerEntry::SettingsDialogIntegerEntry(GConfIntegerOption& option, const std::string& name, int minValue, int maxValue): SettingsDialogEntry(option, name) {
	mySpinBox = HILDON_NUMBER_EDITOR(hildon_number_editor_new(minValue, maxValue));
	hildon_number_editor_set_value(mySpinBox, option.value());
}

void SettingsDialogIntegerEntry::updateValue() {
	((GConfIntegerOption&)myOption).setValue(hildon_number_editor_get_value(mySpinBox));
}

#if 0
SettingsDialogChoiceEntry::SettingsDialogChoiceEntry(GConfIntegerOption& option, const std::string& name): SettingsDialogEntry(option, name) {
	myWidget = gtk_combo_box_new_text();
}
#endif

///

static struct {
	int value;
	const char *name;
} IconSizes[] = {
	{ 26, "Small (26)" },
	{ 32, "Medium (32)" },
	{ 48, "Large (48)" },
	{ 64, "Extra Large (64)" },
	{ -1, NULL }
};

SettingsDialogIconSizeEntry::SettingsDialogIconSizeEntry(GConfIntegerOption& option, const std::string& name): SettingsDialogEntry(option, name) {
	myWidget = gtk_combo_box_new_text();

	int active = -1;

	for (int i = 0; IconSizes[i].value != -1; ++i) {
		gtk_combo_box_append_text(GTK_COMBO_BOX(myWidget), IconSizes[i].name);

		if (IconSizes[i].value == option.value()) {
			active = i;
		}
	}
}

void SettingsDialogIconSizeEntry::updateValue() {
	gint index = gtk_combo_box_get_active(GTK_COMBO_BOX(myWidget));

	if (index != -1) {
		((GConfIntegerOption&)myOption).setValue(IconSizes[index].value);
	}
}

///
