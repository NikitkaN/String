#include "s21_string.h"

//  Finds the first occurrence of the entire string needle
//  (not including the terminating null character)
//  which appears in the string haystack.

char *s21_strstr(const char *haystack, const char *needle) {
  char *found = s21_NULL;
  if (haystack != s21_NULL && needle != s21_NULL) {
    if (*needle) {
      s21_size_t len = s21_strlen(needle);
      for (s21_size_t i = 0; haystack[i] != '\0' && found == s21_NULL; i++) {
        if (!s21_strncmp(haystack + i, needle, len)) {
          return (char *)haystack + i;
        }
      }
    } else {
      found = (char *)haystack;
    }
  }
  return found;
}

//  Searches for the last occurrence of the character c
//  (an unsigned char) in the string pointed to by the argument str.

char *s21_strrchr(const char *str, int c) {
  char *temp = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t count = 0;
    s21_size_t len = s21_strlen(str);
    char *a = (char *)str + len;
    while ((*a != (char)c) && (count <= len)) {
      a--;
      count++;
    }
    if (count <= len) {
      temp = a;
    }
  }
  return temp;
}

//  Calculates the length of the initial segment of str1
//  which consists entirely of characters in str2.

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  unsigned int len = 0;

  if ((str1 == s21_NULL) || (str2 == s21_NULL)) return len;
  while (*str1) {
    if (s21_strchr(str2, *str1)) {
      return len;
    } else {
      str1++;
      len++;
    }
  }

  return len;
}

//  Copies the string pointed to, by src to dest.

char *s21_strcpy(char *dest, const char *src) {
  if (dest == s21_NULL) {
    return s21_NULL;
  }

  char *ptr = dest;

  while (*src != '\0') {
    *dest = *src;
    dest++;
    src++;
  }

  *dest = '\0';

  return ptr;
}

//  Compares the string pointed to, by str1 to the string pointed to by str2.

int s21_strcmp(const char *str1, const char *str2) {
  int result = 0;
  if (str1 && str2) {
    int i = 0;
    while (str1[i] && str1[i] == str2[i]) {
      i++;
    }
    result = str1[i] - str2[i];
  }
  return result;
}

//  Appends the string pointed to, by src to the end of
//  the string pointed to, by dest up to n characters long.

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  if ((dest == s21_NULL) && (src == s21_NULL)) return s21_NULL;
  char *Dest = dest;

  while (*Dest != '\0') {
    Dest++;
  }
  while (n--) {
    if (!(*Dest++ = *src++)) {
      return dest;
    }
  }
  *Dest = '\0';
  return dest;
}

//  Copies the character c (an unsigned char) to the first
//  n characters of the string pointed to, by the argument str.

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *ch = str;
  if (ch) {
    while (n--) {
      *ch++ = (unsigned char)c;
    }
  }
  return str;
}

//  Computes the length of the string str up to
//  but not including the terminating null character.

s21_size_t s21_strlen(const char *str) {
  int lenght = 0;
  for (; *(str + lenght); lenght++) {
  }

  return lenght;
}

//  Copies n characters from src to dest.

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  char *Dest = s21_NULL;
  const char *Src = (const char *)src;
  if (Src != s21_NULL) {
    Dest = (char *)dest;
  }
  if (Dest != s21_NULL) {
    for (s21_size_t i = 0; i < n; i++) {
      Dest[i] = Src[i];
    }
  }
  return Dest;
}

//  Searches for the first occurrence of the character c
//  (an unsigned char) in the first n bytes of the string
//   pointed to, by the argument str.

void *s21_memchr(const void *str, int c, s21_size_t n) {
  unsigned char *ch = (unsigned char *)str;
  unsigned char *Char_find = NULL;
  while ((str != NULL) && (n--)) {
    if (*ch != (unsigned char)c) {
      ch++;
    } else {
      Char_find = ch;
      break;
    }
  }
  return Char_find;
}

//  Compares the first n bytes of str1 and str2.

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  const char *c1 = str1;
  const char *c2 = str2;
  int dif = 0;
  for (s21_size_t i = 0; i < n; i++) {
    dif = *c1 - *c2;
    if (*c1 != *c2) break;
    c1++;
    c2++;
  }
  return dif;
}

//  Another function to copy n characters from src to dest.

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  char *c1 = (char *)dest;
  char *c2 = (char *)src;
  char *temp = (char *)calloc(n, sizeof(char));
  if (temp) {
    s21_memcpy(temp, c2, n);
    s21_memcpy(c1, temp, n);
    free(temp);
  }
  return dest;
}

//  Appends the string pointed to, by src to the end of the string pointed to by
//  dest.

char *s21_strcat(char *dest, const char *src) {
  char *result = dest + s21_strlen(dest);
  while (*src != '\0') {
    *result++ = *src++;
  }
  *result = '\0';
  return dest;
}

//  Searches for the first occurrence of the character c
//  (an unsigned char) in the string pointed to, by the argument str.

char *s21_strchr(const char *str, int c) {
  char temp = c;
  while (*str != temp && *str != '\0') {
    str++;
  }
  if (*str == temp) {
    return (char *)str;
  } else {
    return s21_NULL;
  }
}

// Compares at most the first n bytes of str1 and str2.

int s21_strncmp(const char *str1, const char *str2, s21_size_t n) {
  const char *end = str1 + n;
  int result = 0;
  for (; result == 0 && str1 != end && (*str1 || *str2);
       result = *(str1++) - *(str2++)) {
  }
  return result;
}

//  Copies up to n characters from the string pointed to, by src to dest.

char *s21_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i;
  for (i = 0; i < n && src[i] != '\0'; i++) {
    dest[i] = src[i];
  }
  for (; i < n; i++) {
    dest[i] = 0;
  }
  return dest;
}

//  Searches an internal array for the error number errnum and returns
//  a pointer to an error message string. You need to declare macros
//  containing arrays of error messages for mac and linux operating systems.
//  Error descriptions are available in the original library. Checking
//  the current OS is carried out using directives.

char *s21_strerror(int errnum) {
  static char result[101];
  s21_memset(result, 0, 101);
  if (errnum < 0 || errnum > MAX_ERRLIST - 1) {
#ifdef __APPLE__
    sprintf(result, "Unknown error: %d", errnum);
#elif defined __linux__
    sprintf(result, "Unknown error %d", errnum);
#endif
  } else {
    static char *arr[] = MASS;
    s21_memcpy(result, arr[errnum], s21_strlen(arr[errnum]));
  }
  return result;
}

//  Finds the first character in the string str1 that matches any character
//  specified in str2.

char *s21_strpbrk(const char *str1, const char *str2) {
  if ((str1 == s21_NULL) || (str2 == s21_NULL)) {
    return s21_NULL;
  }
  char *result = s21_NULL;
  while (*str1 && !result) {
    if (s21_strchr(str2, *str1)) {
      result = (char *)str1;
    } else {
      str1++;
    }
  }
  return result;
}

//  Calculates the length of the initial segment of str1 which consists entirely
//  of characters in str2.

s21_size_t s21_strspn(const char *str1, const char *str2) {
  if ((str1 == s21_NULL) || (str2 == s21_NULL)) {
    return 0;
  }
  size_t count = 0;
  while (*str1) {
    if (!(s21_strchr(str2, *str1))) {
      break;
    } else {
      str1++;
      count++;
    }
  }
  return count;
}

//  Breaks string str into a series of tokens separated by delim.

char *s21_strtok(char *str, const char *delim) {
  char *result = s21_NULL;
  s21_size_t i = 0;
  static char *buffer = s21_NULL;
  if (str) {
    buffer = str;
  }
  if (buffer && s21_strlen(buffer) == 0) {
    buffer = s21_NULL;
  }
  if (buffer && s21_strchr(delim, buffer[i])) {
    while (s21_strchr(delim, buffer[i])) {
      buffer++;
    }
  }
  while (buffer && !result) {
    if (buffer[i] == '\0') {
      result = buffer;
      buffer = s21_NULL;
    } else if (s21_strchr(delim, buffer[i])) {
      buffer[i] = '\0';
      result = buffer;
      buffer += i + 1;
    }
    i++;
  }
  return result;
}

//  Returns a copy of string (str) converted to uppercase. In case of any error,
//  return NULL

void *s21_to_upper(const char *str) {
  char *str_result = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t str_len = s21_strlen(str);

    str_result = calloc(str_len + 1, sizeof(char));
    if (str_result) {
      for (s21_size_t i = 0; i < str_len; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
          str_result[i] = (str[i] - 'a') + 'A';
        } else {
          str_result[i] = str[i];
        }
      }
      str_result[str_len] = '\0';
    }
  }

  return str_result;
}

//  Returns a copy of string (str) converted to lowercase. In case of any error,
//  return NULL

void *s21_to_lower(const char *str) {
  char *str_result = s21_NULL;
  if (str != s21_NULL) {
    s21_size_t str_len = s21_strlen(str);

    str_result = calloc(str_len + 1, sizeof(char));
    if (str_result) {
      for (s21_size_t i = 0; i < str_len; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
          str_result[i] = (str[i] - 'A') + 'a';
        } else {
          str_result[i] = str[i];
        }
      }
      str_result[str_len] = '\0';
    }
  }

  return str_result;
}

//  Returns a new string in which a specified string (str)
//  is inserted at a specified index position (start_index)
//  in the given string (src). In case of any error, return NULL

void *s21_insert(const char *src, const char *str, s21_size_t start_index) {
  char *result = s21_NULL;
  if (src && str) {
    s21_size_t src_len = s21_strlen(src);
    s21_size_t str_len = s21_strlen(str);
    if (src_len >= start_index) {
      result = calloc((src_len + str_len + 1), sizeof(char));
    }
    if (result) {
      s21_strncpy(result, src, start_index);
      result[start_index] = '\0';
      s21_strcat(result, str);
      s21_strcat(result, src + start_index);
    }
  }
  return result;
}

//  Returns a new string in which all leading and trailing
//  occurrences of a set of specified characters (trim_chars)
//  from the given string (src) are removed. In case of any error, return NULL

void *s21_trim(const char *src, const char *trim_chars) {
  char *result = s21_NULL;
  if (!trim_chars || s21_strlen(trim_chars) == 0) {
    trim_chars = " \n\t\v\f\r";
  }
  if (src) {
    if (trim_chars && *trim_chars) {
      s21_size_t len = s21_strlen(src);
      char *left = (char *)src;
      char *right = (char *)src + len;
      for (; *left && s21_strchr(trim_chars, *left); left++) {
      }
      for (; right != left && s21_strchr(trim_chars, *(right - 1)); right--) {
      }
      result = calloc((right - left + 1), sizeof(char));
      if (result) {
        s21_strncpy(result, left, right - left);
        *(result + (right - left)) = '\0';
      }
    }
  }
  return result;
}
