#include "s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list args;
  va_start(args, format);
  Flags settings = {0};
  char *str_begin = str;
  while (*format) {
    if (*format == '%') {
      format++;
      s21_memset(&settings, 0, sizeof(Flags));
      format = check_flags(format, &settings);
      format = check_width(format, &settings, args);
      format = check_precision(format, &settings, args);
      format = check_length(format, &settings);
      settings.specifier = *format;
      format++;
      char *buffer = (char *)calloc(1000, sizeof(char));
      parse_value(settings, buffer, args);
      for (int i = 0; buffer[i]; i++, str++) {
        *str = buffer[i];
      }
      free(buffer);
    } else {
      *str++ = *format++;
    }
  }
  *str = '\0';
  va_end(args);
  return str - str_begin;
}

const char *check_width(const char *format, Flags *settings, va_list args) {
  if (*format == '*') {
    format++;
    int perp = va_arg(args, int);
    if (perp < 0) {
      settings->wright = perp * -1;
    }
    settings->width = perp;
  } else if (check(*format) == 1) {
    char *temp = (char *)calloc(1000, sizeof(char));
    for (int i = 0; check(*format); i++, format++) {
      temp[i] = *format;
    }
    settings->width = s21_atoi(temp);
    free(temp);
  }
  return format;
}

int check(char c) {
  int flag;
  if (c >= '0' && c <= '9') {
    flag = 1;
  } else {
    flag = 0;
  }
  return flag;
}

const char *check_precision(const char *format, Flags *settings, va_list args) {
  if (*format == '.') {
    settings->tochka = 1;
    format++;
    if ((*format == 'f' && settings->hash == 1) || *format == 'e' ||
        *format == 'E') {
      settings->nonum = 1;
    }
  }

  if (*format == '*') {
    format++;
    settings->precision = va_arg(args, int);
  }

  if (check(*format)) {
    char *temp = (char *)calloc(1000, sizeof(char));
    for (int i = 0; check(*format); i++, format++) {
      temp[i] = *format;
    }
    settings->precision = s21_atoi(temp);
    free(temp);
  }
  return format;
}

const char *check_length(const char *format, Flags *settings) {
  switch (*format) {
    case 'h':
      settings->length = 'h';
      format++;
      break;
    case 'l':
      settings->length = 'l';
      format++;
      break;
    case 'L':
      settings->length = 'L';
      format++;
  }
  return format;
}

void parse_value(Flags settings, char *buffer, va_list args) {
  switch (settings.specifier) {
    case 'i':
    case 'd':
      settings.base = 10;
      settings.is_num = 1;
      parse_int(settings, buffer, args);
      break;
    case 'c':
      parse_char(settings, buffer, args);
      break;
    case 'f':
      parse_float(settings, buffer, args);
      break;
    case 's':
      parse_string(settings, buffer, args);
      break;
    case 'u':
      settings.is_num = 1;
      parse_unsigned(settings, buffer, args);
      break;
    case 'o':
      parse_octal(settings, buffer, args);
      break;
    case 'x':
    case 'X':
      settings.base = 16;
      settings.is_num = 1;
      parse_hex(settings, buffer, args);
      break;
    case '%':
      buffer[0] = '%';
      break;
    case 'p':
      parse_hex(settings, buffer, args);
      break;
    case 'e':
    case 'E':
      parse_mantiss(settings, buffer, args);
      break;
    case 'g':
    case 'G':
      parse_g_float(settings, buffer, args);
  }
  if (settings.specifier == 'G' || settings.specifier == 'E' ||
      settings.specifier == 'X') {
    while (*buffer) {
      if (*buffer >= 'a' && *buffer <= 'z') {
        *buffer = *buffer - 'a' + 'A';
      }
      buffer++;
    }
  }
}

void parse_int(Flags settings, char *buffer, va_list args) {
  int64_t num = va_arg(args, int64_t);
  if (settings.length == 'h') {
    num = (int16_t)num;
  } else if (settings.length == 0) {
    num = (int32_t)num;
  }
  num_to_string(num, buffer, 10);  // инт в строку
  precision_to_buffer(buffer,
                      settings);  // обработка точности и добавление в буфер
  flags_to_buffer(buffer, settings);  // обработка флагов и добавление в буфер
}
void precision_to_buffer(char *buffer, Flags settings) {
  char *temp = (char *)calloc(1000, sizeof(char));
  int sign = 0;
  int len = s21_strlen(buffer);
  if (buffer[0] == '-') {
    temp[0] = '-';
    len--;
    sign = 1;
  }
  if (settings.precision > len) {
    int index;
    for (index = sign; index < settings.precision - len + sign; index++) {
      temp[index] = '0';
    }
    for (int i = sign; buffer[i]; i++, index++) {
      temp[index] = buffer[i];
    }
    s21_strcpy(buffer, temp);
  }
  if (settings.wright) {
    for (int i = 0; i < settings.wright - 1; i++) {
      s21_strcat(buffer, " ");
    }
  }
  if (settings.tochka && settings.precision == 0 && settings.is_num &&
      buffer[0] == '0') {
    buffer[0] = '\0';
  }
  free(temp);
}

void flags_to_buffer(char *buffer, Flags settings) {
  char *temp = (char *)calloc(1000, sizeof(char));
  if (settings.plus && settings.specifier != 'u') {
    if (buffer[0] == '-') {
      temp[0] = buffer[0];
    } else {
      temp[0] = '+';
    }
    s21_strcpy(temp + 1, buffer[0] == '-' ? buffer + 1 : buffer);
    s21_strcpy(buffer, temp);
  } else if (settings.space && buffer[0] != '-' && settings.specifier != 'u') {
    temp[0] = ' ';
    s21_strcpy(temp + 1, buffer);
    s21_strcpy(buffer, temp);
  }
  int len = s21_strlen(buffer);
  char need;
  if (settings.zero) {
    need = '0';
  } else {
    need = ' ';
  }
  if (settings.width > len) {
    int index = settings.width - len;
    if (!settings.minus) {
      s21_memset(temp, need, index);
      s21_strcpy(temp + index, buffer);
    } else {
      s21_strcpy(temp, buffer);
      s21_memset(temp + s21_strlen(temp), ' ', index);
    }
    s21_strcpy(buffer, temp);
  }
  free(temp);
}

void parse_char(Flags settings, char *buffer, va_list args) {
  if (settings.length == 'l') {
    wchar_t temp;
    temp = va_arg(args, wchar_t);
    if (!settings.minus && settings.width) {
      for (int i = 0; i < settings.width; i++) {
        buffer[i] = ' ';
        if (i == settings.width - 1) wcstombs(buffer, &temp, 1000);
      }
    } else if (settings.width) {
      wcstombs(buffer, &temp, 1000);
      for (int i = s21_strlen(buffer); i < settings.width; i++) buffer[i] = ' ';
    } else {
      wcstombs(buffer, &temp, 1000);
    }
  } else {
    char c;
    c = va_arg(args, int);
    if (!settings.minus && settings.width) {
      for (int i = 0; i < settings.width; i++) {
        buffer[i] = ' ';
        if (i == settings.width - 1) {
          buffer[i] = c;
        }
      }
    } else if (settings.width) {
      buffer[0] = c;
      for (int i = 1; i < settings.width; i++) {
        buffer[i] = ' ';
      }
    } else {
      buffer[0] = c;
    }
  }
}

void parse_float(Flags settings, char *buffer, va_list args) {
  long double c = 0;
  if (settings.length == 'L') {
    c = va_arg(args, long double);
  } else {
    c = va_arg(args, double);
  }
  if (!settings.tochka) {
    settings.precision = 6;
  }
  float_to_str(c, buffer, settings);
  flags_to_buffer(buffer, settings);
}

void float_to_str(long double c, char *returni, Flags settings) {
  if (settings.specifier == 'g') {
    settings.precision--;
  }

  int negative = 0;

  if (c < 0) {
    c = c * -1;
    negative = 1;
  }
  long double int_part = 0;
  long double float_part = modfl(c, &int_part);

  if (settings.precision == 0) {
    int_part = roundl(c);
    float_part = 0;
  }
  char *buffer = (char *)calloc(1000, sizeof(char));

  char *frac = (char *)calloc(1000, sizeof(char));
  for (int i = 0; i < settings.precision; i++) {
    float_part *= 10;
    frac[i] = (int)float_part + '0';
  }
  int index = 998;

  long long fl = roundl(float_part);
  long long in = int_part;

  if (!fl) {
    for (int i = 0; i < settings.precision; index--, i++) {
      buffer[index] = '0';
    }
  } else {
    for (int i = s21_strlen(frac); fl || i > 0; fl /= 10, index--, i--) {
      buffer[index] = (int)(fl % 10 + 0.05) + '0';
    }
  }
  if ((settings.tochka && settings.precision != 0) || (int)float_part ||
      (!settings.tochka && c == 0) || s21_strlen(frac) ||
      (settings.tochka && settings.nonum)) {
    buffer[index--] = '.';
  }
  if (!in) {
    buffer[index] = '0';
    index--;
  } else {
    for (; in; in /= 10, index--) {
      buffer[index] = (int)(in % 10) + '0';
    }
  }
  for (int i = 0; buffer[index + 1]; index++, i++) {
    if (negative && i == 0) {
      returni[i] = '-';
      i++;
    }
    returni[i] = buffer[index + 1];
  }

  char *int_str = (char *)calloc(1000, sizeof(char));
  char *float_str = (char *)calloc(1000, sizeof(char));
  num_to_string(int_part, int_str, 10);
  num_to_string(float_part, float_str, 10);

  precision_to_buffer(float_str, settings);

  s21_strcat(buffer, int_str);

  if (settings.precision || settings.hash) {
    s21_strcat(buffer, ".");
    if (settings.precision) {
      s21_strcat(buffer, float_str);
    }
  }
  free(int_str);
  free(buffer);
  free(frac);
  free(float_str);
}

void parse_string(Flags settings, char *buffer, va_list args) {
  if (settings.length == 'l') {
    wchar_t *wstr = va_arg(args, wchar_t *);
    char tmp[1000] = {'\0'};
    char str[1000] = {'\0'};

    wcstombs(str, wstr, 1000);
    s21_strcpy(tmp, str);
    if (settings.tochka) tmp[settings.precision] = '\0';

    int shift = settings.width - s21_strlen(tmp);
    int len = s21_strlen(tmp);

    if (settings.minus && shift > 0) {
      s21_strcpy(buffer, tmp);
      s21_memset(buffer + len, ' ', shift);
    } else if (shift > 0) {
      s21_memset(buffer, ' ', shift);
      s21_strcpy(buffer + shift, tmp);
    } else {
      s21_strcpy(buffer, tmp);
    }
  } else {
    char *str = va_arg(args, char *);
    char *temp = (char *)calloc(1000, sizeof(char));

    s21_strcpy(temp, str);
    if (settings.tochka == 1) {
      temp[settings.precision] = '\0';
    }
    int shift = settings.width - s21_strlen(temp);
    int len = s21_strlen(temp);

    if (settings.minus && shift > 0) {
      s21_strcpy(buffer, temp);
      s21_memset(buffer + len, ' ', shift);
    } else if (shift > 0) {
      s21_memset(buffer, ' ', shift);
      s21_strcpy(buffer + shift, temp);
    } else {
      s21_strcpy(buffer, temp);
    }
    free(temp);
  }
}

void parse_unsigned(Flags settings, char *buffer, va_list args) {
  long long num;
  switch (settings.length) {
    case 'h':
      num = va_arg(args, int);
      num = (unsigned short int)num;
      break;
    case 'l':
      num = va_arg(args, unsigned long);
      break;
    case 0:
      num = va_arg(args, unsigned int);
      break;
  }
  num_to_string(num, buffer, 10);
  precision_to_buffer(buffer, settings);
  flags_to_buffer(buffer, settings);
}
void parse_octal(Flags settings, char *buffer, va_list args) {
  buffer[0] = '0';
  num_to_string(va_arg(args, long), buffer + settings.hash, 8);
  precision_to_buffer(buffer, settings);
  flags_to_buffer(buffer, settings);
}

void parse_hex(Flags settings, char *buffer, va_list args) {
  long long num = va_arg(args, long);
  num_to_string(num, buffer, 16);
  int allzero = 0;

  for (int i = 0; buffer[i]; i++) {
    if (buffer[i] != '0') {
      allzero = 1;
    }
  }

  precision_to_buffer(buffer, settings);
  if (settings.hash || settings.specifier == 'p') {
    if (allzero == 1 || settings.specifier == 'p') {
      s21_memmove(buffer + 2, buffer, s21_strlen(buffer));
      buffer[0] = '0';
      buffer[1] = 'x';
    }
  }
  flags_to_buffer(buffer, settings);
}

void parse_mantiss(Flags settings, char *buffer, va_list args) {
  long double c = 0;
  if (settings.length == 'L') {
    c = va_arg(args, long double);
  } else {
    c = va_arg(args, double);
  }

  int pow = 0;
  char sign = (int)c == 0 ? '-' : '+';

  if ((int)c - c) {
    while ((int)c == 0) {
      pow++;
      c *= 10;
    }
  } else {
    sign = '+';
  }

  while ((int)c / 10 != 0) {
    pow++;
    c /= 10;
  }

  if (!settings.tochka) {
    settings.precision = 6;
  }
  float_to_str(c, buffer, settings);
  prepmatriss(buffer, pow, sign);
  flags_to_buffer(buffer, settings);
}

void prepmatriss(char *buffer, int pow, char sign) {
  int len = s21_strlen(buffer);
  buffer[len] = 'e';
  buffer[len + 1] = sign;
  buffer[len + 3] = pow % 10 + '0';
  pow /= 10;
  buffer[len + 2] = pow % 10 + '0';
  buffer[len + 4] = '\0';
}

void parse_g_float(Flags settings, char *buffer, va_list args) {
  long double c = 0;
  if (settings.length == 'L') {
    c = va_arg(args, long double);
  } else {
    c = va_arg(args, double);
  }

  if (!settings.tochka) {
    settings.precision = 6;
  }

  if (settings.precision == 0) {
    settings.precision = 1;
  }
  int prec = settings.precision;
  long double b = c;
  int exp = 0;
  if ((int)c - c) {
    while ((int)b == 0) {
      exp++;
      b *= 10;
    }
  }

  if (exp > 4) {
    settings.precision = 0;
    float_to_str(b, buffer, settings);
  } else {
    settings.precision = 10;
    float_to_str(c, buffer, settings);
  }

  int s = 0;
  s21_size_t len = s21_strlen(buffer);
  int z = 0;
  for (s21_size_t i = 0; i < s21_strlen(buffer); i++) {
    if ((buffer[i] == '0' && !z) || buffer[i] == '.') {
      continue;
    } else {
      z = 1;
    }

    if (check(buffer[i]) && z) {
      s++;
    }
    if (s == prec && i + 1 < len) {
      int next = buffer[i + 1] == '.' ? 2 : 1;
      buffer[i] =
          buffer[i + next] - '0' > 5 ? (char)(buffer[i] + 1) : buffer[i];
      buffer[i + 1] = '\0';
      break;
    }
  }

  if (exp > 4) {
    prepmatriss(buffer, exp, '-');
  }

  int lens = s21_strlen(buffer);
  char *dot = s21_strchr(buffer, '.');
  if (dot) {
    for (int i = lens - 1; buffer[i] != '.'; i--) {
      if (buffer[i] == '0')
        buffer[i] = '\0';
      else
        break;
    }
    if (dot[1] == '\0') dot[0] = '\0';
  }

  flags_to_buffer(buffer, settings);
}

const char *check_flags(const char *format, Flags *settings) {
  while (*format == '-' || *format == '+' || *format == ' ' || *format == '0' ||
         *format == '#') {
    switch (*format) {
      case '-':
        settings->minus = 1;
        break;
      case '+':
        settings->plus = 1;
        break;
      case ' ':
        settings->space = 1;
        break;
      case '0':
        settings->zero = 1;
        break;
      case '#':
        settings->hash = 1;
        break;
    }
    format++;
  }
  return format;
}

void num_to_string(int64_t num, char *buff, int base) {
  int negative = 0;
  if (num < 0) {
    negative = 1;
    num = num * -1;
  }
  switch (base) {
    case 8:
      oct_dec_hex_to_str(num, negative, buff, 8);
      break;
    case 10:
      oct_dec_hex_to_str(num, negative, buff, 10);
      break;
    case 16:
      oct_dec_hex_to_str(num, negative, buff, 16);
      break;
  }
}

void *oct_dec_hex_to_str(long long int a, int negative, char *buff, int base) {
  char *temp = (char *)calloc(1000, sizeof(char));
  int index = 444;

  if (a == 0) {
    temp[index] = '0';
  }

  while (a > 0) {
    index--;
    temp[index] = "0123456789abcdef"[a % base];
    a = a / base;
  }
  if (negative) {
    buff[0] = '-';
  }

  for (int i = negative ? 1 : 0; temp[index]; index++, i++) {
    buff[i] = temp[index];
  }
  free(temp);
  return 0;
}

int s21_atoi(const char *str) {
  int result = 0;
  int sign = 1;
  int overflow = 0;

  while (*str == ' ') str++;
  if (*str == '-') {
    str++;
    sign = -1;
  }
  if (*str == '+') {
    str++;
  }
  while (*str && *str >= '0' && *str <= '9') {
    result = result * 10 + (*str - '0');
    if (result < 0) {
      overflow = 1;
      break;
    }
    str++;
  }
  if (overflow)
    result = sign > 0 ? INT32_MAX : INT32_MIN;
  else
    result *= sign;

  return result;
}
