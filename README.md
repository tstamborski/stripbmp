# stripbmp
Command line tool to convert BMP images to plain video memory data in mode 13h.

## Downloads
Here you can find version for download: [stripbmp.zip](https://github.com/tstamborski/stripbmp/releases/download/v0.4/stripbmp.zip).
Zip package contains executables for 64-bit Linux and Windows. Simplest way to use it is to just copy your executable somewhere in the
PATH environment variable (and use it from command line).

## Description
This tool extracts pixel data from indexed mode BMP files, swap lines (images in BMP format are
held upside-down) and optionally slice your spritesheet into sequentially held slices of desired
size. **So output files are very handy to embed in assembly source code or something like that.**
```
Usage: stripbmp [slice_size] bmp_file
```

Recently I have added second, very similiar, tool for creating BIOS fonts from indexed BMP files.
BIOS fonts are 1 bit per pixel images. My "mkbiosfont" tool threat every not-zero pixel in BMP file
as highlighted pixel in output file. Everything else is very similiar to stripbmp:
```
Usage: mkbiosfont font_size bmp_file
```
Just remember that mkbiosfont will accept only 8x8, 8x14 and 8x16 font sizes. Slicing works the same
way as in stripbmp, so if you want a font for all 256 chars and 8x8 font size you should create 128x128
(16\*8=128) or 2048x8 (256\*8=2048) bitmap. Or any other size which will give 256 characters. It is also
possible to create smaller charsets but not bigger.

## Usage Info
```
Usage: stripbmp [slice_size] bmp_file
Output: bmp_file.dat with plain video memory data for VGA mode 13h
Details: bmp_file should be a bmp file in indexed color mode.
         slice_size is size of a single frame in the spritesheet.
         slice_size should be in a format like "32x32" or "16x8".
```
