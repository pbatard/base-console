/*
 * base-console - Because sometimes I want to release a win32 console
 * utility in a hurry, and I like to have it set up properly.
 *
 * Copyright © 2020-2026 Pete Batard <pete@akeo.ie>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef _MSC_VER
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include <windows.h>
#else
#include <libgen.h>
#endif
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define _STRINGIFY(x) #x
#define STRINGIFY(x) _STRINGIFY(x)

#ifndef APP_VERSION
#define APP_VERSION_STR "[DEV]"
#else
#define APP_VERSION_STR STRINGIFY(APP_VERSION)
#endif

#ifdef _MSC_VER
static __inline char* basename(const char* path)
{
	static char basename[128];
	_splitpath_s(path, NULL, 0, NULL, 0, basename, sizeof(basename), NULL, 0);
	return basename;
}
#endif

#ifdef _MSC_VER
int main_utf8(int argc, char** argv)
#else
int main(int argc, char** argv)
#endif
{
	fprintf(stderr, "%s %s © 2020-2026 Pete Batard <pete@akeo.ie>\n\n", basename(argv[0]), APP_VERSION_STR);
	fprintf(stderr, "This program is free software; you can redistribute it and/or modify it under \n");
	fprintf(stderr, "the terms of the GNU General Public License as published by the Free Software \n");
	fprintf(stderr, "Foundation; either version 3 of the License or any later version.\n\n");
	fprintf(stderr, "Official project and latest downloads at: https://github.com/pbatard/base-console\n\n");

	fprintf(stdout, "Hello world!\n");

	return 0;
}

#ifdef _MSC_VER
static __inline char* wchar_to_utf8(const wchar_t* wstr)
{
	int size = 0;
	char* str = NULL;

	if (wstr[0] == 0)
		return (char*)calloc(1, 1);

	size = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	if (size <= 1)
		return NULL;

	if ((str = (char*)calloc(size, 1)) == NULL)
		return NULL;
	if (WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, size, NULL, NULL) != size) {
		free(str);
		return NULL;
	}

	return str;
}

int wmain(int argc, wchar_t** argv16)
{
	SetConsoleOutputCP(CP_UTF8);
	char** argv = calloc(argc, sizeof(char*));
	if (argv == NULL)
		return -1;
	for (int i = 0; i < argc; i++)
		argv[i] = wchar_to_utf8(argv16[i]);
	int r = main_utf8(argc, argv);
	for (int i = 0; i < argc; i++)
		free(argv[i]);
	free(argv);
#ifdef _DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return r;
}
#endif
