//g++ $( pkg-config --cflags gtk4 ) -o myapp gtk4.cpp $( pkg-config --libs gtk4 )
//sudo dnf install gtk4-devel    sudo apt install libgtk-4-dev
//https://docs.gtk.org/gtk4/getting_started.html    https://github.com/ToshioCP/Gtk4-tutorial
#include <gtk/gtk.h>

GtkWidget *window;
GtkWidget *grid;
GtkWidget *button;
GtkWidget *scale;
GtkWidget *label;
GtkWidget *combo;
GtkWidget *edit;
GtkWidget *check;

struct WidgetsSet
{
  GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *label;
} widgetsSet;

//https://docs.gtk.org/gtk4/class.FileChooserDialog.html
static void file_picked_cb (GObject *gobject, GAsyncResult *result, gpointer data)
{
  g_autoptr (GError) error = NULL;
  g_autoptr (GFile) file = gtk_file_dialog_open_finish (GTK_FILE_DIALOG (gobject), result, &error);
  
  if (error != NULL) {
    g_print("error\n");
    // the operation failed, or it was cancelled by the user
  } else {
    // do something with the file
    g_print("%s\n",g_file_get_path (file));
  }
}

static void on_save_response(GObject *source, GAsyncResult *result, gpointer user_data) {
  GtkFileDialog *dialog = GTK_FILE_DIALOG(source);
  GFile *file = gtk_file_dialog_save_finish(dialog, result, NULL);
  
  if (file != NULL) {
    // Here you can save your file using the GFile object
    // For example, you might write some content to the file
    g_print("%s\n", g_file_get_path (file));
    g_object_unref(file);
  }
  
  // gtk_window_destroy(GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(source))));
}

static void show_file_dialog (GtkWidget *widget, gpointer data)
{
  g_autoptr(GtkFileDialog) dialog = gtk_file_dialog_new();
  gtk_file_dialog_set_title(dialog, "Open file");
  gtk_file_dialog_open(dialog, GTK_WINDOW(data),NULL,file_picked_cb,data);
}

static void show_save_dialog (GtkWidget *widget, gpointer data)
{
  g_autoptr(GtkFileDialog) dialog = gtk_file_dialog_new();
  
  g_autoptr(GtkFileFilter) filter = gtk_file_filter_new(); // filter 1
  gtk_file_filter_set_name(filter, "Plain text");
  gtk_file_filter_add_suffix(filter, "txt");
  g_autoptr(GtkFileFilter) filter2 = gtk_file_filter_new(); // filter 2
  gtk_file_filter_set_name(filter2, "All files");
  gtk_file_filter_add_pattern(filter2, "*");
  g_autoptr(GListStore) liststore = g_list_store_new(GTK_TYPE_FILE_FILTER);
  
  g_list_store_append(liststore, filter);
  g_list_store_append(liststore, filter2);
  
  gtk_file_dialog_set_filters (GTK_FILE_DIALOG(dialog), G_LIST_MODEL(liststore));
  gtk_file_dialog_set_initial_name (GTK_FILE_DIALOG(dialog),".txt");
  
  gtk_file_dialog_save(dialog, NULL/*GTK_WINDOW(widgetsSet.window)*/, NULL, on_save_response, NULL);
}

static void scale_value_changed (GtkWidget *widget, gpointer data)
{
  int value = gtk_range_get_value (GTK_RANGE (widget));
  gchar buffer[10];
  sprintf (buffer, "%d", value);
  gtk_label_set_text (GTK_LABEL (label), buffer);
}

static void print_hello (GtkWidget *widget, gpointer   data)
{
  g_print ("Hello World\n");
}

static void new_window(GtkWidget *widget, gpointer   data)
{
  gtk_widget_set_sensitive(GTK_WIDGET(widget), FALSE);
  
  widgetsSet.label = gtk_label_new("Label");
  widgetsSet.grid = gtk_grid_new();
  widgetsSet.window = gtk_window_new();
  gtk_window_set_title(GTK_WINDOW(widgetsSet.window), "Settings");
  gtk_window_set_default_size(GTK_WINDOW(widgetsSet.window), 350, 130);
  
  gtk_window_set_child (GTK_WINDOW (widgetsSet.window), widgetsSet.grid);
  
  gtk_grid_attach(GTK_GRID(widgetsSet.grid), widgetsSet.label, 0, 0, 1, 1);
  
  gtk_window_present(GTK_WINDOW(widgetsSet.window));
  g_signal_connect(widgetsSet.window, "destroy", G_CALLBACK(+[](GtkWidget *widget, gpointer data) {
    gtk_widget_set_sensitive(GTK_WIDGET(data), TRUE);
  }),
  widget);
}

static void get_entry (GtkWidget *widget, gpointer   data)
{
  const char* output = gtk_entry_buffer_get_text (gtk_entry_get_buffer(GTK_ENTRY(edit)));
  g_print ("%s %i\n", output, gtk_check_button_get_active (GTK_CHECK_BUTTON(check)));
}

gboolean time_handler(GtkWidget *widget) {
  GDateTime *now = g_date_time_new_now_local();
  gchar *my_time = g_date_time_format(now, "%H:%M:%S");
  g_print ("%s\n", my_time);
  
  //g_sprintf(buf, "%s", my_time);
  
  g_free(my_time);
  g_date_time_unref(now);
  
  return TRUE;
}

static void instance_changed (GtkDropDown *combo)
{
  const char *text;
  text = gtk_string_list_get_string (GTK_STRING_LIST (gtk_drop_down_get_model (combo)),
                                     gtk_drop_down_get_selected (combo));
  g_print ("%i %s\n",gtk_drop_down_get_selected(combo), text);
}

static void activate (GtkApplication *app, gpointer user_data)
{
  /* create a new window, and set its title */
  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Test app");
  
  g_timeout_add(1000, (GSourceFunc) time_handler, (gpointer) window);
  
  /* Here we construct the container that is going pack our buttons */
  grid = gtk_grid_new ();
  
  /* Pack the container in the window */
  gtk_window_set_child (GTK_WINDOW (window), grid);
  
  button = gtk_button_new_with_label ("Button 1");
  g_signal_connect (button, "clicked", G_CALLBACK (print_hello), NULL);
  
  /* Place the first button in the grid cell (0, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
  
  button = gtk_button_new_with_label ("Get entry");
  g_signal_connect (button, "clicked", G_CALLBACK (get_entry), NULL);
  
  /* Place the second button in the grid cell (1, 0), and make it fill
   * just 1 cell horizontally and vertically (ie no spanning)
   */
  gtk_grid_attach (GTK_GRID (grid), button, 1, 0, 1, 1);
  
  edit = gtk_entry_new();
  gtk_grid_attach (GTK_GRID (grid), edit, 0, 1, 2, 1);
  
  
  //scaling the contents of the window to size changes
  gtk_widget_set_hexpand(edit, TRUE);
  
  scale = gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, 0, 100, 1);
  gtk_grid_attach (GTK_GRID (grid), scale, 0, 2, 2, 1);
  g_signal_connect (scale, "value-changed", G_CALLBACK (scale_value_changed), NULL);
  
  label = gtk_label_new ("0");
  gtk_grid_attach (GTK_GRID (grid), label, 0, 3, 1, 1);
  
  //https://github.com/GNOME/gtk/blob/main/demos/gtk-demo/font_features.c
  GtkStringList *strings;
  strings = gtk_string_list_new(NULL);
  gtk_string_list_append (strings, "Aaaa");
  gtk_string_list_append (strings, "Bbbb");
  gtk_string_list_append (strings, "Cccc");
  
  //https://github.com/GNOME/gtk/blob/main/demos/gtk-demo/font_features.c
  combo = gtk_drop_down_new(G_LIST_MODEL (strings), NULL);
  gtk_grid_attach (GTK_GRID (grid), combo, 1, 3, 1, 1);
  g_signal_connect (combo, "notify::selected-item", G_CALLBACK (instance_changed), NULL);
  
  //https://github.com/steshaw/gtk-examples/blob/master/ch04.button.edit.combo/checkbox.c
  check = gtk_check_button_new_with_label ("checkox");
  gtk_check_button_set_active (GTK_CHECK_BUTTON(check), TRUE);
  gtk_grid_attach (GTK_GRID (grid), check, 0, 4, 1, 1);
  g_signal_connect (check, "toggled", G_CALLBACK (get_entry), NULL);
  
  button = gtk_button_new_with_label ("Open file");
  g_signal_connect (button, "clicked", G_CALLBACK (show_file_dialog), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 5, 1, 1);
  
  button = gtk_button_new_with_label ("Save as");
  g_signal_connect (button, "clicked", G_CALLBACK (show_save_dialog), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 1, 5, 1, 1);
  
  button = gtk_button_new_with_label ("New window");
  g_signal_connect (button, "clicked", G_CALLBACK (new_window), NULL);
  gtk_grid_attach (GTK_GRID (grid), button, 0, 6, 1, 1);
  
  button = gtk_button_new_with_label ("Quit");
  g_signal_connect_swapped (button, "clicked", G_CALLBACK (gtk_window_destroy), window);
  
  /* Place the Quit button in the grid cell (0, 1), and make it
   * span 2 columns.
   */
  gtk_grid_attach (GTK_GRID (grid), button, 1, 6, 1, 1);
  
  
  g_autoptr(GtkCssProvider) provider = gtk_css_provider_new ();
  gtk_css_provider_load_from_string(provider, R"rawliteral(#entry1 {background-color: red;  font-weight: bold; color:red;})rawliteral");
  GdkDisplay *display = gdk_display_get_default ();
  gtk_style_context_add_provider_for_display (display,
                                              GTK_STYLE_PROVIDER (provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_FALLBACK);
  gtk_widget_set_name(edit, "entry1");
  
  gtk_window_present(GTK_WINDOW(window));
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  
  app = gtk_application_new ("org.gtk.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  
  return status;
}
