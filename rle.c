#include <stdio.h>

#define null ((void *)0)

FILE * input = null;
FILE * output = null;

unsigned char tag = 0xff;

void print_usage_info()
{
    printf("\nUsage: rle input_file output_file\n");
    printf("Output: rle encoded version of input_file\n");
    printf("Details: this rle encoding program uses value 0xff for tag byte.\n");
}

void print_error(char *description)
{
    printf("\nError: %s\n", description);
    print_usage_info();
    printf("\n");
}

void fputrle(unsigned char count, FILE * out)
{
    fputc(tag, out);
    fputc(count, out);
}

void rle_encode(FILE *in, FILE *out)
{
    int rbyte;
    int buffer = tag;
    unsigned char buffer_count = 0;

    fputc(tag, out);

    while ((rbyte = fgetc(in)) != EOF)
    {
        if (rbyte != buffer)
        {
            if (buffer_count < 1)
            {
                fputc(rbyte, out);
                buffer = rbyte;
                buffer_count = 0;
            }
            else
            {
                fputrle(buffer_count, out);
                fputc(rbyte, out);
                buffer = rbyte;
                buffer_count = 0;
            }
        }
        else
        {
            buffer_count++;
            if (buffer_count == 255)
            {
                fputrle(buffer_count, out);
                buffer = tag;
                buffer_count = 0;
            }
        }
    }

    if (buffer_count >= 1)
    {
        fputrle(buffer_count, out);
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

    rle_encode(input, output);

    fclose(input);
    fclose(output);
    return 0;
}
