all: build/win64/stripbmp.exe build/linux64/stripbmp build/win64/mkbiosfont.exe build/linux64/mkbiosfont

build/win64/%.exe: %.c
	if [ ! -d build ]; then mkdir build; fi
	if [ ! -d build/win64 ]; then mkdir build/win64; fi
	x86_64-w64-mingw32-gcc -o $@ $<

build/linux64/%: %.c
	if [ ! -d build ]; then mkdir build; fi
	if [ ! -d build/linux64 ]; then mkdir build/linux64; fi
	gcc -o $@ $<

clean:
	if [ -d build/linux64 ]; then rm build/linux64/*; fi
	if [ -d build/win64 ]; then rm build/win64/*; fi

