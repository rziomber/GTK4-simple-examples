 #include <gtk/gtk.h>

 static void button_clicked (GtkWidget *widget, gpointer data)
 {
     GtkWidget *label = GTK_WIDGET (data);
     gtk_label_set_text (GTK_LABEL (label), "The world has been changed!");
 }

 static void activate (GtkApplication* app, gpointer user_data)
 {
     GtkWidget *window;

     window = gtk_application_window_new (app);
     gtk_window_set_title (GTK_WINDOW (window), "Change the world");
     gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
     GtkWidget *label = gtk_label_new ("");
     GtkWidget *button = gtk_button_new_with_label ("Change the world");
     g_signal_connect (button, "clicked", G_CALLBACK (button_clicked), label);

     GtkWidget *grid = gtk_grid_new();
     /* Pack the container in the window */
     gtk_window_set_child (GTK_WINDOW (window), grid);
     gtk_grid_attach (GTK_GRID (grid), button, 0, 0, 1, 1);
     gtk_grid_attach (GTK_GRID (grid), label, 0, 1, 1, 1);
     gtk_widget_show (window);
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
