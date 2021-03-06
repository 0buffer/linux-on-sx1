/******************************************************************************
 *	
 *    generate the "if(){} else if ..." structure of ucs2fontmap()
 *
 *                 Copyright (c) 2000  O'ksi'D
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 *
 *   Author : Jean-Marc Lienher ( http://oksid.ch )
 *
 ******************************************************************************/

#include <wchar.h>
#include <stdio.h>
char buffer[1000000];

int main(int argc, char **argv)
{
	char buf[80];
	int len;
	char *encode[256];
	int encode_number = 0;
	unsigned int i = 0;
	unsigned char *ptr;
	unsigned char *lst = "";
	size_t nb;
	int nbb = 0;
	len = fread(buffer, 1, 1000000, stdin);

	puts("  ");
	puts("   /*************** conv_gen.c ************/");
	buffer[len] = '\0';
	ptr = buffer;
	
	printf("const int ucs2fontmap"
		"(char *s, unsigned int ucs, int enc)\n");
	printf("{\n");
	printf("  switch(enc) {\n");
	printf("  case 0:\n");
	printf("    s[0] = (char) ((ucs & 0xFF00) >> 8);\n");
	printf("    s[1] = (char) (ucs & 0xFF);\n");
	printf("    return 0;");
	while (len > 0) {
		unsigned char *p = ptr;
		unsigned char *f, *t;
		
		while (*p != ']') {
			i++;
			p++;
		}
		*(p - 1) = '\0';
		*(p - 6) = '\0';
		f = p - 5;
		while (*p != '+') { i++; p++;}
		p++;
		t = p;
		*(p + 4) = '\0';
		if (strcmp(lst, ptr)) {
			encode_number++;
			encode[encode_number] = ptr;
			printf("\n    break;");
			printf("\n  case %d:\n", encode_number);
			printf("    ");
		} else {
			printf(" else ");
		}
		lst = ptr;
		printf("if (ucs <= 0x%s) {\n", t);
		printf("      if (ucs >= 0x%s) {\n", f);
		if (*(f - 3) == '2') {
			printf("        int i = (ucs - 0x%s) * 2;\n", f);
			printf("        s[0] = %s_%s[i++];\n", ptr, f, f);
			printf("        s[1] = %s_%s[i];\n", ptr, f, f);
			printf("        if (s[0] || s[1]) return %d;\n", 
				encode_number);
		} else {
			printf("        s[0] = 0;\n");
			printf("        s[1] = %s_%s[ucs - 0x%s];\n", 
				ptr, f, f);
			printf("        if (s[1]) return %d;\n", encode_number);
		}
		printf("      }\n");
		printf("    }");
		while (*ptr != '\n') {
			ptr++;
			len--;
		} 
		ptr++;
		len--;
	}
	printf("\n    break;\n");
	printf("\n  default:\n");
	printf("    break;\n");
	printf("  };\n");
	printf("  return -1;\n");
	printf("};\n\n");

	printf("const int encoding_number(const char *enc)\n{\n");
	printf("  if (!enc || !strcmp(enc, \"iso10646-1\")) {\n");
	printf("    return 0;\n");
	i = 1;
	while (i <= encode_number) {
		int l;
		char *ptr;
		l = strlen(encode[i]) - 3;
		ptr = encode[i] + l;
		*(ptr) = '\0';
		ptr--;
		while (ptr != encode[i]) {
			if (*ptr == '_') {
				*ptr = '-';
				ptr--;
				break;
			}
			ptr--;
		}
		while (ptr != encode[i]) {
			if (*ptr == '_') {
				*ptr = '.';
			}
			ptr--;
		}
		printf("  } else if (!strcmp(enc, \"%s\")", encode[i] +11);

		if (!strcmp(encode[i] + 11, "big5-0")) { 
			printf(" || !strcmp(enc, \"big5.eten-0\")");
		} else if (!strcmp(encode[i] + 11, "dingbats")) {
			printf(" || !strcmp(enc, \"zapfdingbats\")");
			printf(" || !strcmp(enc, \"zapf dingbats\")");
			printf(" || !strcmp(enc, \"itc zapf dingbats\")");
		} else if (!strcmp(encode[i] + 11, "jisx0208.1983-0")) {
			printf(" || !strcmp(enc, \"jisx0208.1990-0\")");
		}

		printf(") {\n");
		printf("    return %d;\n", i);
		i++;
	}
	printf("  };\n");	
	printf("  return -1;\n");	
	printf("};\n\n");	


	printf("/*\n");	
	printf("const char *encoding_name(int num)\n{\n");
	printf("  switch (num) {\n");
	i = 1;
	while (i <= encode_number) {
		printf("  case %d:\n", i);
		printf("    return \"%s\";\n", encode[i] + 11);
		i++;
	}
	printf("  };\n");
	printf("  return \"iso10646-1\";\n");
	printf("};\n\n");
	printf("*/\n");	
	return 0;
}
