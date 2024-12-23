#include "ft_ls.h"


void ft_putchar(char c) {
    write(1, &c, 1);
}

void ft_putstr(char *str) {
    while (*str) {
        ft_putchar(*str);
        str++;
    }
}

void ft_putnbr(int n) {
    if (n < 0) {
        ft_putchar('-');
        n = -n;
    }
    if (n >= 10) {
        ft_putnbr(n / 10);
    }
    ft_putchar(n % 10 + '0');
}

void ft_puthex(unsigned int n) {
    if (n >= 16) {
        ft_puthex(n / 16);
    }
    char hex[] = "0123456789abcdef";
    ft_putchar(hex[n % 16]);
}

void ft_printf(const char *format, ...) {
    va_list args;
    int i = 0;
    char *str;
    int num;
    unsigned int hex_num;

    va_start(args, format);
    while (format[i]) {
        if (format[i] == '%') {
            i++;
            if (format[i] == 's') {
                str = va_arg(args, char *);
                ft_putstr(str);
            } else if (format[i] == 'd') {
                num = va_arg(args, int);
                ft_putnbr(num);
            } else if (format[i] == 'x') {
                hex_num = va_arg(args, unsigned int);
                ft_puthex(hex_num);
            }
        } else {
            ft_putchar(format[i]);
        }
        i++;
    }
    va_end(args);
}