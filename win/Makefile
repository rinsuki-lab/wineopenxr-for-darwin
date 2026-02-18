wineopenxr.dll: main.c wine_unixcall.c
	x86_64-w64-mingw32-gcc -shared -o $@ $^ -L/Applications/Wine\ Staging.app/Contents/Resources/wine/lib/wine/x86_64-windows -lntdll -luser32 -lucrt
