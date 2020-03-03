#ifndef _IMAGES_H
#define _IMAGES_H

#include <stdbool.h>
#include <cairo/cairo.h>

bool verify_image(const char *image_path);
bool image_surface_from_file(const char* path, cairo_surface_t **surface_out);

#endif
