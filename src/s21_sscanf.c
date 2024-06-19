#include "s21_string.h"

struct specifiers {
  char flag;
  int width;
  int acc;
  char length;
  char spec;
} specifiers;

int s21_sscanf(const char* str, const char* format, ...) {
  va_list string;
  int counte = 0, c = 0;
  int j = 0;
  va_start(string, format);
  for (int i = 0; *(format + i) != '\0'; ++i) {
    if (*(format + i) == '%') {
      fill((char*)(format), &string, &i);
      c += read(str, &j, &string, &counte);
    }
  }
  va_end(string);
  return c;
}

void fill(char* str, va_list* string, int* i) {
  (*i)++;
  int sign = 1;
  if (*(str + *i) == '-') {
    sign = -1;
    (*i)++;
  } else if (*(str + *i) == '+') {
    (*i)++;
  }
  (specifiers).width = sign * number(str, i);
  if (*(str + *i) == '*') {
    (specifiers).width = va_arg(*string, int);
    (*i)++;
  }
  if (length(str + *i) == 1) {
    (specifiers).length = *(str + *i);
    (*i)++;
  } else {
    (specifiers).length = 'e';
  }
  (specifiers).spec = *(str + *i);
  if (*(str + *i) == 'c' && (specifiers).width == -1) (specifiers).width = 1;
}

int read(const char* str, int* j, va_list* string, int* counter) {
  int r = 0;
  if (specifiers.spec == 'c')
    r = read_s(str, j, string, counter);
  else if (specifiers.spec == 'd')
    r = read_d(str, j, string, counter);
  else if ((specifiers.spec == 'e') || (specifiers.spec == 'E') ||
           (specifiers.spec == 'f') || (specifiers.spec == 'g') ||
           (specifiers.spec == 'G'))
    r = read_f(str, j, string, counter);
  else if (specifiers.spec == 'o')
    r = read_o(str, j, string, counter);
  else if (specifiers.spec == 's')
    r = read_s(str, j, string, counter);
  else if (specifiers.spec == 'n')
    r = read_n(*counter, string);
  else if (specifiers.spec == '%')
    r = read_c_pr(str, j, string, counter);
  return r;
}

int read_n(int coun, va_list* string) {
  int* n = va_arg(*string, int*);
  *n = coun;
  return 1;
}

int read_s(const char* str, int* j, va_list* string, int* counter) {
  char copy[256];
  int i = 0, k = 1;
  if (specifiers.spec == 's') {
    skip(str, j, counter);
    if (specifiers.width != -1)
      *(counter) += copy_str(specifiers.width, str, copy, j);
    else
      *(counter) += copy_str(1000, str, copy, j);
  } else {
    if (specifiers.width != -1)
      *(counter) += copy_str_c(specifiers.width, str, copy, j);
    else
      *(counter) += copy_str_c(1000, str, copy, j);
  }
  read_c(copy, &i, string);
  if (specifiers.width == 0 || specifiers.width < -1) k = 0;
  return k;
}

int read_o(const char* str, int* j, va_list* string, int* counter) {
  skip(str, j, counter);
  long o_str = 0;
  int k = 1;
  if (*(str + *j) == '-' || *(str + *j) == '+') (*j)++;
  if (specifiers.width == 0 || specifiers.width < -1) {
    k = 0;
  } else {
    if (specifiers.width != -1) {
      char copy[256];
      copy_str(specifiers.width, str, copy, j);
      int i = 0;
      o_str = oct_number(copy, &i, counter);
    } else {
      o_str = oct_number(str, j, counter);
    }
  }
  if (write_int(o_str, string) != 1) k = 0;
  return k;
}

int read_f(const char* str, int* j, va_list* string, int* counter) {
  skip(str, j, counter);
  int sign = 1, k = 1;
  long double f_str = 0;
  if (*(str + *j) == '-') {
    sign = -1;
    (*j)++;
  }
  if (*(str + *j) == '+') (*j)++;
  if (specifiers.width == 0 || specifiers.width < -1) {
    k = 0;
  } else {
    if (specifiers.width != -1) {
      char copy[256];
      copy_str(specifiers.width, str, copy, j);
      int i = 0;
      f_str = read_f_convert(copy, &i, counter);
    } else {
      f_str = read_f_convert(str, j, counter);
    }
    f_str *= sign;
  }
  if (write_float(f_str, string) != 1) k = 0;
  return k;
}

int read_d(const char* str, int* j, va_list* string, int* counter) {
  int i = 1;
  skip(str, j, counter);
  int sign = 1;
  long d_str = 0;
  if (*(str + *j) == '-') {
    sign = -1;
    (*j)++;
  }
  if (*(str + *j) == '+') (*j)++;
  if (specifiers.width == 0 || specifiers.width < -1) {
    i = 0;
  } else {
    if (specifiers.width != -1) {
      char copy[256];
      (*counter) += copy_str(specifiers.width, str, copy, j);
      int k = 0;
      d_str = number(copy, &k);
    } else {
      d_str = number(str, j);
      (*counter) += count(d_str);
    }
    d_str *= sign;
  }
  if (write_int(d_str, string) != 1) i = 0;
  return i;
}

int read_c(const char* str, const int* j, va_list* string) {
  int res = 0;
  if (specifiers.length == 'l') {
    wchar_t* ch = va_arg(*string, wchar_t*);
    int i;
    for (i = 0; *(str + *j + i) != '\0'; ++i) ch[i] = str[*j + i];
    if (specifiers.spec == 's') ch[i] = '\0';
  } else if (specifiers.length == 'e') {
    char* ch = va_arg(*string, char*);
    int i = 0;
    for (i = 0; *(str + *j + i) != '\0'; ++i) ch[i] = str[*j + i];
    if (specifiers.spec == 's') ch[i] = '\0';
  } else {
    res = 1;
  }
  return res;
}

int read_c_pr(const char* str, int* j, va_list* string, int* counter) {
  skip(str, j, counter);
  if (specifiers.length == 'l') {
    wchar_t* ch = va_arg(*string, wchar_t*);
    *ch = *(str + *j);
    (*j)++;
  } else {
    char* ch = va_arg(*string, char*);
    *ch = *(str + *j);
    (*j)++;
  }
  if (*(str + *j - 1) == '%') {
    (*counter)++;
  }
  return 0;
}

int copy_str(const int width, const char* str, char* copy, int* j) {
  int k;
  for (k = 0; k < width && *(str + *j) != '\0' && *(str + *j) != ' ';
       ++k, (*j)++)
    copy[k] = *(str + *j);
  copy[k] = '\0';
  return k - 1;
}

int copy_str_c(const int width, const char* str, char* copy, int* j) {
  int k;
  for (k = 0; k < width && *(str + *j) != '\0'; ++k, (*j)++)
    copy[k] = *(str + *j);
  copy[k] = '\0';
  return k - 1;
}

int oct_len(const char* str, const int* i) {
  int res = 0;
  while (oct(str + *i + res)) res++;
  return res;
}

int oct_number(const char* str, int* i, int* counter) {
  int res = 0, octe = 1;
  (*i)++;
  int len = oct_len(str, i) - 1;
  while (len >= 0) {
    res += (*(str + *i + len) - '0') * octe;
    octe *= 8;
    len--;
    (*counter)++;
  }
  (*i) += len;
  return res;
}

int write_int(long d_str, va_list* string) {
  int res = 0;
  if (specifiers.length == 'e') {
    int* result = va_arg(*string, int*);
    *result = d_str;
  } else if (specifiers.length == 'l') {
    long* result = va_arg(*string, long*);
    *result = d_str;
  } else if (specifiers.length == 'h') {
    short* result = va_arg(*string, short*);
    *result = d_str;
  } else {
    res = 1;
  }
  return res;
}

void skip(const char* str, int* i, int* counter) {
  while (*(str + *i) == ' ') {
    (*i)++;
    (*counter)++;
  }
}

long double read_f_convert(const char* str, int* j, int* counter) {
  long double res = number(str, j);
  (*counter) += count(res);
  if (*(str + *j) == '.') {
    (*j)++;
    (*counter)++;
    long double k = 0.1;
    while (numb(str + *j)) {
      res += k * (*(str + *j) - '0');
      k *= 0.1;
      (*j)++;
      (*counter)++;
    }
  }
  if (*(str + *j) == 'e' || *(str + *j) == 'E') {
    if (*(str + *j) == '-' || *(str + *j) == '+') {
      char c = *(str + *j);
      (*j)++;
      (*counter)++;
      int pow = number(str, j), i = 1;
      while (pow > 0) {
        i *= 10;
        pow--;
      }
      if (c == '+')
        res *= i;
      else if (c == '-')
        res /= i;
    }
  }
  return res;
}

int write_float(long double d_str, va_list* string) {
  int res = 0;
  if (specifiers.length == 'e') {
    float* result = va_arg(*string, float*);
    *result = d_str;
  } else if (specifiers.length == 'L') {
    long double* result = va_arg(*string, long double*);
    *result = d_str;
  } else {
    res = 1;
  }
  return res;
}

int number(const char* str, int* i) {
  int res = 0;
  int k = 0;
  while (numb(str + *i)) {
    res *= 10;
    res += *(str + *i) - '0';
    (*i)++;
    k++;
  }
  if (k == 0) res = -1;
  return res;
}

int count(int p) {
  int res = 0;
  while (p > 0) {
    res++;
    p /= 10;
  }
  return res;
}

int length(const char* str) {
  return (*str == 'L' || *str == 'l' || *str == 'h');
}

int oct(const char* str) { return (*str >= '0' && *str <= '7'); }
int numb(const char* str) { return (*str >= '0' && *str <= '9'); }
