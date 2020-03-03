#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "images.h"

static bool verify_png_image(const char *image_path) {
    if (!image_path) {
        return false;
    }

    /* Check file exists and has correct PNG header */
    FILE *png_file = fopen(image_path, "r");
    if (png_file == NULL) {
        fprintf(stderr, "Image file path \"%s\" cannot be opened: %s\n", image_path, strerror(errno));
        return false;
    }
    unsigned char png_header[8];
    memset(png_header, '\0', sizeof(png_header));
    int bytes_read = fread(png_header, 1, sizeof(png_header), png_file);
    fclose(png_file);
    if (bytes_read != sizeof(png_header)) {
        fprintf(stderr, "Could not read PNG header from \"%s\"\n", image_path);
        return false;
    }

    // Check PNG header according to the specification, available at:
    // https://www.w3.org/TR/2003/REC-PNG-20031110/#5PNG-file-signature
    static unsigned char PNG_REFERENCE_HEADER[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    if (memcmp(PNG_REFERENCE_HEADER, png_header, sizeof(png_header)) != 0) {
        fprintf(stderr, "File \"%s\" does not start with a PNG header. i3lock currently only supports loading PNG files.\n", image_path);
        return false;
    }
    return true;
}

static bool verify_ppm_image(const char *image_path) {
    if (!image_path) {
        return false;
    }

    /* Check file exists and has correct PNG header */
    FILE *ppm_file = fopen(image_path, "r");
    if (ppm_file == NULL) {
        fprintf(stderr, "Image file path \"%s\" cannot be opened: %s\n", image_path, strerror(errno));
        return false;
    }

    // Check for "P3" or "P6", which is the magic number for colored PPM images
    // http://netpbm.sourceforge.net/doc/ppm.html
    char c = (char) getc(ppm_file);
    if (c != 'P') {
        fprintf(stderr, "File \"%s\" with PPM extension is not in a PPM format.\n", image_path);
        return false;
    }
    c = (char) getc(ppm_file);
    if (c != '3' || c != '6') {
        fprintf(stderr, "Greyscale PPM format is not supported.\n");
        return false;
    }

    return true;
}

/*
 * Checks for the image extension and calls the correct function
 */
bool verify_image(const char *image_path) {
    if (!image_path) {
        return false;
    }

    char path_copy[1024];
    strncpy(path_copy, image_path, 1024 * sizeof(char));
    char *extension = strrchr(basename(path_copy), '.');
    if(!extension || extension == image_path) {
        return false;
    }

    extension++;
    if (!strncmp(extension, "png", 3)) {
        return verify_png_image(image_path);
    } else if (!strncmp(extension, "ppm", 3)) {
        return verify_ppm_image(image_path);
    }

    return false;
}
