# stripbmp
Command line tool to convert BMP images to plain video memory data in mode 13h.

## Downloads
Here you can find version for download: [stripbmp.zip](https://github.com/tstamborski/stripbmp/releases/download/v0.4/stripbmp.zip).
Zip package contains executables for 64-bit Linux and Windows. Simplest way to use it is to just copy your executable somewhere in the
PATH environment variable (and use it from command line).

## Description
This tool extracts pixel data from indexed mode BMP files, swap lines (images in BMP format are
held upside-down) and optionally slice your spritesheet into sequentially held slices of desired
size.

## Usage Info
```
Usage: stripbmp [slice_size] bmp_file
Output: bmp_file.dat with plain video memory data for VGA mode 13h
Details: bmp_file should be a bmp file in indexed color mode.
         slice_size is size of a single frame in the spritesheet.
         slice_size should be in a format like \"32x32\" or \"16x8\".
```
