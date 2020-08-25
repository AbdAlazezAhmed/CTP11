#include "GL/3DView.h"
#include "Graphics2D/graph.h"
#include "Serial/Serial.h"
#define ITEM_SIZE 400
GtkWidget *graph2;

void func(gpointer data)
{
  currentTemp.numberOfReadings++;
  g_timeout_add(100,
                G_CALLBACK(func),
                NULL);
  gtk_widget_queue_draw(graph2);
}
static void activate(GtkApplication *app, gpointer user_data)
{
  GtkWidget *window;
  gettimeofday(&start, NULL);
  pressureGauge = 0.9;
  pressureNormal = 1.1;
  pressureAbs = 1.243;
  currentTemp.numberOfReadings = 0;
  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Window");
  gtk_window_set_default_size(GTK_WINDOW(window), 1280, 720);
  // create a GtkGLArea instance
  GtkWidget *gl_area = gtk_gl_area_new();
  GtkWidget *graph1 = gtk_drawing_area_new();
  GtkWidget *graph3 = gtk_drawing_area_new();

  graph2 = gtk_drawing_area_new();
  GtkWidget *gl_frame = gtk_aspect_frame_new("3D view", 0.5, 0.5, 1, FALSE);

  GtkWidget *grid = gtk_grid_new();
  gtk_container_add(GTK_CONTAINER(gl_frame), gl_area);
  gtk_grid_set_row_spacing(grid, 10);
  gtk_grid_set_column_spacing(grid, 3);
  gtk_container_add(GTK_CONTAINER(window), grid);
  gtk_widget_set_size_request(gl_area, ITEM_SIZE, ITEM_SIZE);
  g_signal_connect(G_OBJECT(graph1), "draw",
                   G_CALLBACK(paintPressure),
                   NULL /*< here we can pass a pointer to a custom data structure */
  );
  g_signal_connect(G_OBJECT(graph2), "draw",
                   G_CALLBACK(paintTemp),
                   NULL /*< here we can pass a pointer to a custom data structure */
  );
  g_signal_connect(G_OBJECT(graph3), "draw",
                   G_CALLBACK(paintTemp),
                   NULL /*< here we can pass a pointer to a custom data structure */
  );

  gtk_widget_set_vexpand(gl_area, TRUE);
  gtk_widget_set_hexpand(gl_area, TRUE);
  gtk_widget_set_vexpand(graph1, TRUE);
  gtk_widget_set_hexpand(graph1, TRUE);
  gtk_widget_set_vexpand(graph2, TRUE);
  gtk_widget_set_hexpand(graph2, TRUE);
  gtk_widget_set_vexpand(graph3, TRUE);
  gtk_widget_set_hexpand(graph3, TRUE);

  // connect to the "render" signal
  //  char command[1024];
  //  float longitude = 30,latitude = 30,zoom=12;
  //  char* resolution = "400x400",*accessToken="حط الاكسس بتاعتك يا خلبوص";
  //  snprintf(command,1024,"wget -O snapshot.jpg \"https://api.mapbox.com/styles/v1/mapbox/streets-v11/static/geojson(%%7B%%22type%%22%%3A%%22Point%%22%%2C%%22coordinates%%22%%3A%%5B%f%%2C%f%%5D%%7D)/%f,%f,%f/%s?access_token=%s\" ",longitude,latitude,longitude,latitude,zoom,resolution,accessToken);
  // system(command);
  gtk_widget_set_size_request(graph1, ITEM_SIZE, ITEM_SIZE);
  gtk_widget_set_size_request(graph2, ITEM_SIZE, ITEM_SIZE);
  gtk_widget_set_size_request(graph3, ITEM_SIZE, ITEM_SIZE);

  GtkWidget *image = gtk_image_new_from_file("snapshot.jpg");

  gtk_grid_set_row_homogeneous(grid, TRUE);
  gtk_grid_set_column_homogeneous(grid, TRUE);
  gtk_grid_attach(grid, gl_frame, 0, 0, 1, 1);
  gtk_grid_attach(grid, image, 1, 0, 1, 1);
  gtk_grid_attach(grid, graph1, 0, 1, 1, 1);
  gtk_grid_attach(grid, graph3, 1, 1, 1, 1);
  gtk_grid_attach(grid, graph2, 2, 0, 2, 2);

  GdkRGBA color;
  color.alpha = 1;
  color.red = 0.321;
  color.green = 0.321;
  color.blue = 0.321;
  gtk_widget_override_background_color(grid, GTK_STATE_NORMAL, &color);

  g_signal_connect(gl_area, "realize", G_CALLBACK(on_realize), NULL);
  g_signal_connect(gl_area, "render", G_CALLBACK(render), NULL);

  gtk_widget_show_all(window);
  g_timeout_add(100,
                G_CALLBACK(func),
                NULL);
}
int main(int argc, char **argv)
{
  openSerialPort("YESTTY", B9600);

  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);
  return status;
}