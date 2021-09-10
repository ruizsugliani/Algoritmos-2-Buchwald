#include "hfns.h"
#include "limits.h"
#define BASE (256)

/* ******************************************************************
 *               FUNCIONES DE HASH UTILIZADAS PARA CMS
 * *****************************************************************/

size_t hash_1(const char *word) {
    size_t hash = 0;

    for (size_t i = 0; word[i] != '\0'; i++) {
        size_t c = (unsigned char)word[i];
        hash = (hash << 3) + (hash >> (sizeof(hash) * CHAR_BIT - 3)) + c;
    }
    return hash;
}

size_t hash_2(const char *word){
    size_t hash = 0;
    for (int i = 0; word[i] != '\0'; i++){
        hash = 31 * hash + word[i];
    }
    return hash;
}

size_t hash_3(const char *s){
    size_t hashval;

    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 97*hashval;
    return hashval;
}

size_t hash_4(const char *str) {
  size_t i = 0;
  size_t hash4 = 0;
  size_t length = strlen(str);
  while (i != length) {
    hash4 += str[i++];
    hash4 += hash4 << 10;
    hash4 ^= hash4 >> 6;
  }
  hash4 += hash4 << 3;
  hash4 ^= hash4 >> 11;
  hash4 += hash4 << 15;
  return hash4;
}

size_t hash_5(const char * str) {
	size_t hash5 = 401;

	while (*str != '\0') {
		hash5 = ((hash5 << 4) + (size_t)(*str));
		str++;
	}

	return hash5;
}