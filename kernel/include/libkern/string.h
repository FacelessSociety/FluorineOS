#pragma once

#include <stddef.h>

// put this somewhere else
// note: thanks for glibc
size_t strlen(const char *str){
  const char *char_ptr;
  const unsigned long int *longword_ptr;
  unsigned long int longword, himagic, lomagic;

  for (char_ptr = str; ((unsigned long int) char_ptr
			& (sizeof (longword) - 1)) != 0;
       ++char_ptr)
    if (*char_ptr == '\0')
      return char_ptr - str;

  longword_ptr = (unsigned long int *) char_ptr;

  himagic = 0x80808080L;
  lomagic = 0x01010101L;
  if (sizeof (longword) > 4){
		himagic = ((himagic << 16) << 16) | himagic;
		lomagic = ((lomagic << 16) << 16) | lomagic;
  }
  if (sizeof (longword) > 8)
    return 0;

  for (;;) {
		longword = *longword_ptr++;

		if (((longword - lomagic) & ~longword & himagic) != 0){
			const char *cp = (const char *) (longword_ptr - 1);

			if (cp[0] == 0)
				return cp - str;
			if (cp[1] == 0)
				return cp - str + 1;
			if (cp[2] == 0)
				return cp - str + 2;
			if (cp[3] == 0)
				return cp - str + 3;
			if (sizeof (longword) > 4){
				if (cp[4] == 0) return cp - str + 4;
				if (cp[5] == 0)	return cp - str + 5;
				if (cp[6] == 0)	return cp - str + 6;
				if (cp[7] == 0) return cp - str + 7;
			}
		}
  }
}



// convert to a certain base
char* convert(unsigned int num, int base){
	static char rep[] = "0123456789ABCDEF";
	static char buf[50];
	char* ptr;

	ptr = &buf[49];
	*ptr = '\0';
	do {
		*--ptr = rep[num % base];
		num /= base;
	} while(num != 0);
	return ptr;
}