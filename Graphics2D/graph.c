#include "graph.h"

void paintOutline(cairo_t *cr, float corner_radius, float aspect, float x, float y, float width2, float height2)
{

  double radius = corner_radius / aspect;
  double degrees = G_PI / 180.0;

  cairo_new_sub_path(cr);
  cairo_arc(cr, x + width2 - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_arc(cr, x + width2 - radius, y + height2 - radius, radius, 0 * degrees, 90 * degrees);
  cairo_arc(cr, x + radius, y + height2 - radius, radius, 90 * degrees, 180 * degrees);
  cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.254, 0.254, 0.254);
  cairo_fill_preserve(cr);
  cairo_set_source_rgba(cr, 0.192, 0.192, 0.192, 1);
  cairo_set_line_width(cr, 2.0);
  cairo_stroke(cr);

  cairo_new_sub_path(cr);
  cairo_arc(cr, x + width2 - radius, y + radius, radius, -90 * degrees, 0 * degrees);
  cairo_line_to(cr, x + width2, y + 50);
  cairo_line_to(cr, x, y + 50);
  cairo_arc(cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.192, 0.192, 0.192);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);
}
void drawMeterLine(cairo_t *cr, float x, float y, float width, float height, float percentage)
{
  cairo_new_sub_path(cr);
  cairo_line_to(cr, x + width, y);          //top right
  cairo_line_to(cr, x + width, y + height); //bottom right
  cairo_line_to(cr, x, y + height);         //bottom left
  cairo_line_to(cr, x, y);                  //top left
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.192, 0.192, 0.192);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);

  cairo_new_sub_path(cr);
  cairo_line_to(cr, x + width, y + height * (100 - percentage) / 100); //top right
  cairo_line_to(cr, x + width, y + height);                            //bottom right
  cairo_line_to(cr, x, y + height);                                    //bottom left
  cairo_line_to(cr, x, y + height * (100 - percentage) / 100);         //top left
  cairo_close_path(cr);

  cairo_set_source_rgb(cr, 0.925, 0.384, 0.372);
  cairo_fill_preserve(cr);
  cairo_stroke(cr);

  cairo_set_source_rgb(cr, 1, 1, 1);

  cairo_text_extents_t extents;

  char *num = calloc(4, sizeof(char));
  sprintf(num, "%.3f", percentage * 1.2 / 100);

  cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, height / 10);
  cairo_text_extents(cr, num, &extents);

  cairo_move_to(cr, x + width / 2 - extents.width / 2 - extents.x_bearing, y + height - height * percentage / 98);
  cairo_show_text(cr, num);
}
void paintPressure(GtkWidget *widget, GdkEventExpose *eev, gpointer data)
{

  gint width, height;
  gint i;
  cairo_t *cr;

  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  cr = gdk_cairo_create(gtk_widget_get_window(widget));

  /* clear background */
  //70.0/255, 70.0/255, 89.0/255
  cairo_set_source_rgb(cr, 0.321, 0.321, 0.321);
  cairo_paint(cr);

  /* a custom shape that could be wrapped in a function */
  double x = 25.0, /* parameters like cairo_rectangle */
      y = 25.0,
         width2 = width - 50,
         height2 = height - 50,
         aspect = 1.0,                /* aspect ratio */
      corner_radius = height2 / 40.0; /* and corner curvature radius */

  double radius = corner_radius / aspect;
  double degrees = G_PI / 180.0;
  paintOutline(cr, corner_radius, aspect, x, y, width2, height2);

  drawMeterLine(cr, x + width2 / 6 + width2 - (x + width2 / 6 * 5), y + 50 + height2 / 10, width2 / 15, height2 / 1.8, pressureGauge / 1.2 * 100);
  drawMeterLine(cr, x + width2 / 6 * 2 + width2 - (x + width2 / 6 * 5), y + 50 + height2 / 10, width2 / 15, height2 / 1.8, pressureNormal / 1.2 * 100);
  drawMeterLine(cr, x + width2 / 6 * 3 + width2 - (x + width2 / 6 * 5), y + 50 + height2 / 10, width2 / 15, height2 / 1.8, pressureAbs / 1.2 * 100);

  cairo_text_extents_t extents;

  cairo_set_source_rgb(cr, 1, 1, 1);

  cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, height2 / 30);
  cairo_text_extents(cr, "Gauge", &extents);

  cairo_move_to(cr, x + width2 / 6 + width2 - (x + width2 / 6 * 5) - extents.width / 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.6);
  cairo_show_text(cr, "Gauge");

  cairo_text_extents(cr, "Normal", &extents);

  cairo_move_to(cr, x + width2 / 6 * 2 + width2 - (x + width2 / 6 * 5) - extents.width / 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.6);
  cairo_show_text(cr, "Normal");

  cairo_text_extents(cr, "Absolute", &extents);

  cairo_move_to(cr, x + width2 / 6 * 3 + width2 - (x + width2 / 6 * 5) - extents.width / 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.6);
  cairo_show_text(cr, "Absolute");

  cairo_set_font_size(cr, height2 / 20);

  cairo_text_extents(cr, "0.8", &extents);

  cairo_move_to(cr, x + width2 / 6 - extents.width * 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 4);
  cairo_show_text(cr, "0.8");

  cairo_text_extents(cr, "0.9", &extents);

  cairo_move_to(cr, x + width2 / 6 - extents.width * 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 3);
  cairo_show_text(cr, "0.9");

  cairo_text_extents(cr, "1.0", &extents);

  cairo_move_to(cr, x + width2 / 6 - extents.width * 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 2);
  cairo_show_text(cr, "1.0");

  cairo_text_extents(cr, "1.1", &extents);

  cairo_move_to(cr, x + width2 / 6 - extents.width * 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 1);
  cairo_show_text(cr, "1.1");

  cairo_text_extents(cr, "1.2", &extents);

  cairo_move_to(cr, x + width2 / 6 - extents.width * 2 - extents.x_bearing + width2 / 15 / 2, y + 50 + height2 / 10);
  cairo_show_text(cr, "1.2");

  cairo_set_source_rgb(cr, 0.192, 0.192, 0.192);

  cairo_set_line_width(cr, 2);
  cairo_move_to(cr, x + width2 / 6, y + 50 + height2 / 10 + height2 / 1.8 / 4);
  cairo_line_to(cr, x + width2 / 6 * 5, y + 50 + height2 / 10 + height2 / 1.8 / 4);
  cairo_stroke(cr);

  cairo_move_to(cr, x + width2 / 6, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 2);
  cairo_line_to(cr, x + width2 / 6 * 5, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 2);
  cairo_stroke(cr);

  cairo_move_to(cr, x + width2 / 6, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 3);
  cairo_line_to(cr, x + width2 / 6 * 5, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 3);
  cairo_stroke(cr);

  cairo_move_to(cr, x + width2 / 6, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 4);
  cairo_line_to(cr, x + width2 / 6 * 5, y + 50 + height2 / 10 + height2 / 1.8 / 4 * 4);
  cairo_stroke(cr);

  cairo_move_to(cr, x + width2 / 6, y + 50 + height2 / 10);
  cairo_line_to(cr, x + width2 / 6 * 5, y + 50 + height2 / 10);
  cairo_stroke(cr);

  cairo_set_source_rgb(cr, 1, 1, 1);

  cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, height2 / 15);

  cairo_move_to(cr, (x + width2) / 2 - height2 / 6, y + height2 / 12);
  cairo_show_text(cr, "Pressure");

  cairo_destroy(cr);
}
void paintGrid(cairo_t *cr, float corner_radius, float aspect, float x, float y, float width2, float height2)
{
  char *time = calloc(5, sizeof(char));
  gettimeofday(&stop, NULL);
  cairo_text_extents_t extents;

  //vertical lines
  for (int i = 1; i < 6; i++)
  {
    cairo_set_source_rgb(cr, 0.192, 0.192, 0.192);

    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, x + width2 / 6 * i, y + 50 + height2 / 10);
    cairo_line_to(cr, x + width2 / 6 * i, y + 50 + height2 / 10 + height2 / 1.8);
    cairo_stroke(cr);

    sprintf(time, "-%.2f s", ((float)(stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 5 * (5 - i) / 1000000);
    cairo_set_source_rgb(cr, 1, 1, 1);

    cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL,
                           CAIRO_FONT_WEIGHT_NORMAL);
    cairo_set_font_size(cr, height2 / 30);
    cairo_text_extents(cr, time, &extents);

    cairo_move_to(cr, x + width2 / 6 * i - extents.width / 2 - extents.x_bearing, y + 50 + height2 / 10 + height2 / 1.6);
    cairo_show_text(cr, time);
  }
  cairo_set_source_rgb(cr, 0.192, 0.192, 0.192);

  //horizontal lines
  for (int i = 0; i < 6; i++)
  {
    cairo_set_line_width(cr, 2);
    cairo_move_to(cr, x + width2 / 6, y + 50 + height2 / 10 + height2 / 1.8 * i / 5);
    cairo_line_to(cr, x + width2 * 5 / 6, y + 50 + height2 / 10 + height2 / 1.8 * i / 5);
    cairo_stroke(cr);
  }
}
void paintTemp(GtkWidget *widget, GdkEventExpose *eev, gpointer data)
{
  gint width, height;
  gint i;
  cairo_t *cr;

  width = gtk_widget_get_allocated_width(widget);
  height = gtk_widget_get_allocated_height(widget);

  cr = gdk_cairo_create(gtk_widget_get_window(widget));

  /* clear background */
  //70.0/255, 70.0/255, 89.0/255
  cairo_set_source_rgb(cr, 0.321, 0.321, 0.321);
  cairo_paint(cr);

  /* a custom shape that could be wrapped in a function */
  double x = 25.0, /* parameters like cairo_rectangle */
      y = 25.0,
         width2 = width - 50,
         height2 = height - 50,
         aspect = 1.0,                /* aspect ratio */
      corner_radius = height2 / 40.0; /* and corner curvature radius */

  paintOutline(cr, corner_radius, aspect, x, y, width2, height2);
  paintGrid(cr, corner_radius, aspect, x, y, width2, height2);

  cairo_set_source_rgb(cr, 1, 1, 1);

  cairo_select_font_face(cr, "serif", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, height2 / 15);

  cairo_move_to(cr, (x + width2) / 2 - height2 / 6, y + height2 / 12);
  cairo_show_text(cr, "Temperature");

  cairo_destroy(cr);
}