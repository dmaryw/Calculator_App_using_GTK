#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

GtkWidget *entry;
gchar current_input[100] = "";

void on_button_clicked(GtkWidget *button, gpointer data) {
	const gchar *label = gtk_button_get_label(GTK_BUTTON(button));

	if (g_strcmp0(label, "C") == 0) {
		current_input[0] = '\0';
	} else if (g_strcmp0(label, "=") == 0) {
		int num1, num2, result;
		char op;
		if (sscanf(current_input, "%d%c%d", &num1, &op, &num2) == 3) {
			switch (op) {
				case '+': result = num1 + num2; break;
				case '-': result = num1 - num2; break;
				case '*': result = num1 * num2; break;
				case '/': 
					  if (num2 != 0) result = num1 / num2;
					  else {
						  gtk_entry_set_text(GTK_ENTRY(entry), "Error: Div by 0");
						  return;
					  }
					  break;
				case '%':
					  if (num2 != 0) result = num1 % num2;
					  else {
						  gtk_entry_set_text(GTK_ENTRY(entry), "Error: Mod by 0");
						  return;
					  }
					  break;
				default:
					  gtk_entry_set_text(GTK_ENTRY(entry), "Invalid Op");
					  return;
			}
			snprintf(current_input, sizeof(current_input), "%d", result);
		}
	} else {
		strcat(current_input, label);
	}

	gtk_entry_set_text(GTK_ENTRY(entry), current_input);
}

GtkWidget* create_button(const gchar *label, const gchar *css_class) {
	GtkWidget *button = gtk_button_new_with_label(label);
	GtkStyleContext *context = gtk_widget_get_style_context(button);
	gtk_style_context_add_class(context, css_class);
	g_signal_connect(button, "clicked", G_CALLBACK(on_button_clicked), NULL);
	return button;
}

int main(int argc, char *argv[]) {
	gtk_init(&argc, &argv);

	// Create window
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "CALCULATOR");
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 250);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Create CSS provider
	GtkCssProvider *provider = gtk_css_provider_new();
	const gchar *css_data =
		".number-button { background: #03A9F4; color: white; font-weight: bold; font-size: 16px; border-radius: 10px; }"
		".operator-button { background: #FF9800; color: white; font-weight: bold; font-size: 16px; border-radius: 10px; }"
		".special-button { background: #4CAF50; color: white; font-weight: bold; font-size: 16px; border-radius: 10px; }"
		".clear-button { background: #F44336; color: white; font-weight: bold; font-size: 16px; border-radius: 10px; }"
		"entry { background: #FFEB3B; color: black; font-size: 18px; font-weight: bold; }";
	gtk_css_provider_load_from_data(provider, css_data, -1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
			GTK_STYLE_PROVIDER(provider),
			GTK_STYLE_PROVIDER_PRIORITY_USER);

	// Create vertical box
	GtkWidget *vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	// Entry
	entry = gtk_entry_new();
	gtk_editable_set_editable(GTK_EDITABLE(entry), FALSE);
	gtk_style_context_add_class(gtk_widget_get_style_context(entry), "entry");
	gtk_box_pack_start(GTK_BOX(vbox), entry, FALSE, FALSE, 5);

	// Grid
	GtkWidget *grid = gtk_grid_new();
	gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
	gtk_grid_set_row_spacing(GTK_GRID(grid), 5);
	gtk_grid_set_column_spacing(GTK_GRID(grid), 5);
	gtk_box_pack_start(GTK_BOX(vbox), grid, TRUE, TRUE, 5);

	// First row
	gtk_grid_attach(GTK_GRID(grid), create_button("C", "clear-button"), 0, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("/", "operator-button"), 1, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("*", "operator-button"), 2, 0, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("-", "operator-button"), 3, 0, 1, 1);

	// Second row
	gtk_grid_attach(GTK_GRID(grid), create_button("7", "number-button"), 0, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("8", "number-button"), 1, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("9", "number-button"), 2, 1, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("+", "operator-button"), 3, 1, 1, 1);

	// Third row
	gtk_grid_attach(GTK_GRID(grid), create_button("4", "number-button"), 0, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("5", "number-button"), 1, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("6", "number-button"), 2, 2, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("%", "operator-button"), 3, 2, 1, 1);

	// Fourth row
	gtk_grid_attach(GTK_GRID(grid), create_button("1", "number-button"), 0, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("2", "number-button"), 1, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("3", "number-button"), 2, 3, 1, 1);
	gtk_grid_attach(GTK_GRID(grid), create_button("=", "special-button"), 3, 3, 1, 2);

	// Fifth row
	gtk_grid_attach(GTK_GRID(grid), create_button("0", "number-button"), 0, 4, 3, 1);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

