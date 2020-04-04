#include <madosix/kernel.h>

static int i2s(char *buffer, int buffer_len, long integer, int base, int case_, int lead, int alignment, int prefix, int width) 
{
	int count = 0;
	unsigned long m;
	char *buf = buffer;
	char *buffer_end = buffer + buffer_len;

	if (!buffer || buffer_len <= 0 || width < 0 || buffer_len < width)
		return 0;

	*--buffer_end = 0;
    m = integer < 0 && base == 10 ? -integer : integer;

	do {
		*--buffer_end = "0123456789abcdef0123456789ABCDEF"[m % base + case_ * 16];
		m /= base;
		count++;
	} while (m && buffer_end >= buffer);
	if (prefix  && buffer_end > buffer && ((alignment == 1 && !lead) || alignment == -1))  {
		if (base == 2) {
			*--buffer_end = case_ ? 'B' : 'b';
			*--buffer_end = '0';
			count += 2;
		} else if (base == 16) {
			*--buffer_end = case_ ? 'X' : 'x';
			*--buffer_end = '0';
			count += 2;
		} else if (base == 8) {
			*--buffer_end = '0';
			count++;
		}
	}
	if (base == 10 && integer < 0  && buffer_end >= buffer && ((alignment == 1 && !lead) || alignment == -1)) {
			*--buffer_end = '-';
			count++;
	}

	if (buffer_end < buffer)
		return count;

	if (alignment == 1) { 
		if (base == 10 && integer < 0 && lead) {
			*buf++ = '-';
			count++;
		}
		if (prefix && lead)  {
			if (base == 2) {
				*buf++ = '0';
				*buf++ = case_ ? 'B' : 'b';
				count += 2;
			} else if (base == 16) {
				*buf++ = '0';
				*buf++ = case_ ? 'X' : 'x';
				count += 2;
			} else if (base == 8) {
				*buf++ = '0';
				count++;
			}
		}
		while (count < width) {
			*buf++ = lead ? '0' : ' ';
			count++;
		}

		while (*buffer_end)
			*buf++ = *buffer_end++;
		*buf = 0;
	} else if (alignment == -1) {
		while (*buffer_end)
			*buf++ = *buffer_end++;

		while (count < width) {
			*buf++ = ' ';
			count++;
		}
		*buf = 0;
	}

	return count;
}

static int d2s(char *buffer, int buffer_len, double number, int lead, int alignment, int width1, int width2) 
{
	int count = 0, i;
	double m;
	unsigned long m1, m2;
	char *buf = buffer;
	char *buffer_end = buffer + buffer_len;

	if (!buffer || buffer_len <= 0 || width1 < 0 || width2 < 0 || buffer_len < width1 || buffer_len < width2)
		return 0;
 
	*--buffer_end = 0;

    m = number < 0  ? -number : number;
	m1 = m;
	m = m - (int)m;
	for (i = 0; i < width2 + 1; i++)
		m *= 10;
	m2 = m;
	m2 = (m2 + 5) / 10;

	do {
		*--buffer_end = "0123456789"[m2 % 10];
		m2 /= 10;
		count++;
	} while (m2 && buffer_end >= buffer);
	if (buffer_end >= buffer) {
		*--buffer_end = '.';
		count++;
	}
	do {
		*--buffer_end = "0123456789"[m1 % 10];
		m1 /= 10;
		count++;
	} while (m1 && buffer_end >= buffer);	

	if (number < 0  && buffer_end >= buffer && !lead) {
			*--buffer_end = '-';
			count++;
	}

	if (buffer_end < buffer)
		return count;

	if (alignment == 1) { 
		if (number < 0 && lead) {
			*buf++ = '-';
			count++;
		}
		
		while (count < width1) {
			*buf++ = lead ? '0' : ' ';
			count++;
		}
		
		while (*buffer_end)
			*buf++ = *buffer_end++;
		*buf = 0;
	} else if (alignment == -1) {
		while (*buffer_end)
			*buf++ = *buffer_end++;

		while (count < width1) {
			*buf++ = ' ';
			count++;
		}
		*buf = 0;
	}

	return count;
}

static int s2s(char *buffer, int buffer_len, const char *str, int alignment, int width1) 
{
	int count = 0, i;
	char *buf = buffer;
	const char *_str = str;

	if (!buffer || buffer_len <= 0 || width1 < 0  || buffer_len < width1)
		return 0;

	if (alignment == -1) { 
		while (*str && count < buffer_len - 1) {
			*buf++ = *str++;
			count++;
		}
		while (count < width1) {
			*buf++ = ' ';
			count++;
		}
		*buf = 0;
	} else if (alignment == 1) {
		int m;
		while (*str)
			str++;

		m = str - _str;
		for (i = 0; i < width1 - m; i++) {
			*buf++ = ' ';
			count++;
		}

		while (*_str && count < buffer_len - 1) {
			*buf++ = *_str++;
			count++;
		}
		*buf = 0;
	}

	return count;
}

static int _vsnprintf(char *buf, int buf_len, const char *format, va_list ap)
{
	int n = 0, len = 0;

	for(; *format != '\0' && (!buf_len || len < buf_len); format++) {
		int case_ = 0, base = 0, type = 0;
		int lead = 0, prefix = 0, width1 = 0, width2 = 0, alignment = 1;

	    if (*format != '%' && len < buf_len) {
			*buf++ = *format;
			len++;
			continue;
		}
			
		format++;
		if (*format == '#') {
			prefix = 1;
			format++;	
		}
		if (*format == '-') {
			alignment = -1;
			format++;	
		}
		if (*format == '0') {
			lead = 1;
			format++;	
		}
		
		while (*format >= '0' && *format <= '9') {
			width1 *= 10;
			width1 += (*format - '0');
			format++;
		}

		if (*format == '.') {
			format++;
			while (*format >= '0' && *format <= '9') {
				width2 *= 10;
				width2 += (*format - '0');
				format++;
			}
		}
		
		switch (*format) {
			case 'b': base = 2;  case_ = 0; type = 2; break;
			case 'B': base = 2;  case_ = 1; type = 2; break;
			case 'c': base = 0;  case_ = 0; type = 4; break;
			case 'd': base = 10; case_ = 0; type = 1; break;
			case 'f': base = 0;  case_ = 0; type = 3; break;
			case 'o': base = 8;  case_ = 0; type = 2; break;
			case 's': base = 0;  case_ = 0; type = 5; break;		 
			case 'u': base = 10; case_ = 0; type = 2; break;
			case 'x': base = 16; case_ = 0; type = 2; break;
			case 'X': base = 16; case_ = 1; type = 2; break;
			 					
			default : base = 0;  case_ = 0; type = 6; break;
		}

		switch (type) {
		case 1:
			n = i2s(buf, buf_len - len, va_arg(ap, int), base, case_, lead, alignment, prefix, width1);
			len += n;
			buf += n;
			break;
		case 2:
			n = i2s(buf, buf_len - len, va_arg(ap, unsigned int), base, case_, lead, alignment, prefix, width1);
			len += n;
			buf += n;
			break;
		case 3:
			width2 = width2 ? width2 : 6;
			n = d2s(buf, buf_len - len, va_arg(ap, double), lead, alignment, width1, width2);
			len += n;
			buf += n;
			break;
		case 4:
			*buf++ = va_arg(ap, int);
			len++;
			break;
		case 5:
			n = s2s(buf, buf_len - len, va_arg(ap, char *), alignment, width1);
			len += n;
			buf += n;
			break;
		default:
			*buf++ = *format;
			len++;
			break;
		}
	}
	*buf = 0;
	return len;
}

int vsnprintf(char *buf, int buf_len, const char *format, va_list ap) 
{
	if (buf_len <= 0)
		return 0;

	return _vsnprintf(buf, buf_len, format, ap);
}

int snprintf(char *buf, int n, const char *format, ...)
{
	va_list ap;

	if (n <= 0)
		return 0;

	va_start(ap, format);
	n = _vsnprintf(buf, n, format, ap);
	va_end(ap);

	return n;
}

int vsprintf(char *buf, const char *format, va_list ap) 
{
	return _vsnprintf(buf, 0, format, ap);
}

int sprintf(char *buf, const char *format, ...)
{
	int n;
	va_list ap;

	va_start(ap, format);
	n = _vsnprintf(buf, 0, format, ap);
	va_end(ap);

	return n;
}
