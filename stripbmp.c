#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if __STDC_VERSION__ < 199901L
#define uint32_t unsigned long
#define int32_t long
#else
#include <stdint.h>
#endif

#define null ((void *)0)

#define GET_LONG(f) (((uint32_t)fgetc(f)) | ((uint32_t)fgetc(f)) << 8 | ((uint32_t)fgetc(f)) << 16 | ((uint32_t)fgetc(f)) << 24)
#define MAX(n,m) ((n > m) ? n : m)

FILE *in_file = null;
FILE *out_file = null;

char filename_buffer[256];

struct CONVERSION_PARAMS
{
    uint32_t data_offset;
    int32_t bmp_width, bmp_height;
    int slice_width, slice_height;
} params;

void print_version_info()
{
    printf("\nSTRIPBMP ver. 0.4\n");
    printf("Copyright (c) 2024 by Tobiasz Stamborski\n");
}

void print_usage_info()
{
    printf("\nUsage: stripbmp [slice_size] bmp_file\n");
    printf("Output: bmp_file.dat with plain video memory data for VGA mode 13h\n");
    printf("Details: bmp_file should be a bmp file in indexed color mode.\n");
    printf("         slice_size is size of a single frame in the spritesheet.\n");
    printf("         slice_size should be in a format like \"32x32\" or \"16x8\".\n");
}

void print_info()
{
    print_version_info();
    print_usage_info();
    printf("\n");
}

void print_error(char *description)
{
    printf("\nError: %s\n", description);
    print_usage_info();
}

int is_microsoft_bmp(FILE *file) {
    fseek(file, 0, SEEK_SET);

    if (fgetc(file) != 'B')
        return 0;
    if (fgetc(file) != 'M')
        return 0;

    return 1;
}

int is_uncompressed_bmp(FILE *file) {
    fseek(file, 0x1e, SEEK_SET);
    return fgetc(file) == 0 ? 1 : 0;
}

int is_indexed_bmp(FILE *file) {
    fseek(file, 0x1c, SEEK_SET);
    return fgetc(file) == 8 ? 1 : 0;
}

void read_bmp_params(FILE *file, struct CONVERSION_PARAMS *params)
{
    fseek(file, 0x0a, SEEK_SET);
    params->data_offset = GET_LONG(file);

    fseek(file, 0x12, SEEK_SET);
    params->bmp_width = GET_LONG(file);
    params->bmp_height = GET_LONG(file);
}

void err_exit(char *error_desc)
{
    print_error(error_desc);

    if (in_file != null)
        fclose(in_file);
    if (out_file != null)
        fclose(out_file);

    exit(-1);
}

void convert(FILE *input, FILE *output, struct CONVERSION_PARAMS *params)
{
    int byte;
    int i, j, x, y;
    int cx_slice = params->bmp_width / params->slice_width;
    int cy_slice = params->bmp_height / params->slice_height;
    int slice_offset = params->slice_height*params->bmp_width;

    for (i = cy_slice - 1; i >= 0; i--)
    {
        for (j = 0; j < cx_slice; j++)
        {
            /* na poczatek ostatniej linii danego slice */
            fseek(input, params->data_offset + (i+1)*slice_offset + j*params->slice_width - params->bmp_width, SEEK_SET);

            for (y = 0; y < params->slice_height; y++)
            {
                for (x = 0; x < params->slice_width; x++)
                {
                    byte = fgetc(input);
                    fputc(byte, output);
                }
                fseek(input, -(params->bmp_width + params->slice_width), SEEK_CUR); /* na poczatek poprzedniej linii */
            }
        }
    }
}

int last_index_of(char *string, char to_find) {
    int i, index;

    index = -1;
    for (i = 0; i < strlen(string); i++)
        if (filename_buffer[i] == to_find)
            index = i;

    return index;
}

char *get_out_filename(char *in_filename)
{
    int len;
    int ext_index, sep_index;

    if (strlen(in_filename) < 252)
    {
        strcpy(filename_buffer, in_filename);

        ext_index = last_index_of(filename_buffer, '.');
        sep_index = MAX(
            last_index_of(filename_buffer, '\\'),
            last_index_of(filename_buffer, '/')
        );

        if (ext_index > 0 && ext_index > sep_index)
        {
        filename_buffer[ext_index] = '.';
        filename_buffer[ext_index+1] = 'd';
        filename_buffer[ext_index+2] = 'a';
        filename_buffer[ext_index+3] = 't';
        filename_buffer[ext_index+4] = '\0';
        }
        else
        {
            len = strlen(filename_buffer);

            filename_buffer[len] = '.';
            filename_buffer[len+1] = 'd';
            filename_buffer[len+2] = 'a';
            filename_buffer[len+3] = 't';
            filename_buffer[len+4] = '\0';
        }
    }
    else
    {
        strcpy(filename_buffer, "output.dat");
    }

    return filename_buffer;
}

int main(int argc, char **argv)
{
    if (argc < 2 || argc > 3)
    {
        print_info();
        return 0;
    }

    in_file = fopen(argv[argc-1], "r");
    if (in_file == NULL)
    {
        print_error("Cannot open given file!");
        return -1;
    }
    if (!is_microsoft_bmp(in_file))
        err_exit("This file is not a Microsoft BMP!");
    if (!is_uncompressed_bmp(in_file))
        err_exit("This is an unsupported compressed bitmap!");
    if (!is_indexed_bmp(in_file))
        err_exit("This bitmap is not in the indexed color mode!");

    read_bmp_params(in_file, &params);
    if (argc > 2)
    {
        sscanf(argv[1], "%dx%d", &params.slice_width, &params.slice_height);
    }
    else
    {
        params.slice_width = params.bmp_width;
        params.slice_height = params.bmp_height;
    }

    out_file = fopen(get_out_filename(argv[argc-1]), "w");
    convert(in_file, out_file, &params);

    fclose(in_file);
    fclose(out_file);
    return 0;
}
