#include <stdio.h>

#define null ((void *)0)

FILE * input = null;
FILE * output = null;

unsigned char tag = 0xff;

void print_usage_info()
{
    printf("\nUsage: rld input_file output_file\n");
    printf("Output: decoded version of input_file\n");
    printf("Details: this program decodes rle encoded binary files.\n");
}

void print_error(char *description)
{
    printf("\nError: %s\n", description);
    print_usage_info();
    printf("\n");
}

void rle_decode(FILE *in, FILE *out)
{
    int rbyte, wbyte;
    int i, count;

    tag = (unsigned char)fgetc(in);
    while ((rbyte = fgetc(in)) != EOF)
    {
        if (rbyte != tag)
        {
            wbyte = rbyte;
            fputc(wbyte, out);
        }
        else
        {
            count = fgetc(in);
            for (i = 0; i < count; i++)
            {
                fputc(wbyte, out);
            }
        }
    }
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        print_usage_info();
        printf("\n");
        return 0;
    }

    input = fopen(argv[1], "r");
    if (input == null)
    {
        print_error("Cannot open input file! Access denied!");
        return -1;
    }
    output = fopen(argv[2], "w");
    if (output == null)
    {
        print_error("Cannot open output file! Access denied!");
        if (input != null)
            fclose(input);
        return -1;
    }

    rle_decode(input, output);

    fclose(input);
    fclose(output);
    return 0;
}

