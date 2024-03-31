#include <stdarg.h>
#include <unistd.h>

int putchar(int c) {
    return write(1, &c, 1);
}

int print_str(const char *str) {
    int count = 0;
    while (*str) {
        putchar(*str++);
        count++;
    }
    return count;
}

int print_int(int val) {
    char buffer[12];
    int count = 0;
    if (val == 0) {
        putchar('0');
        return 1;
    }

    if (val < 0) {
        putchar('-');
        val = -val;
        count++;
    }

    int len = 0;
    while (val) {
        buffer[len++] = '0' + (val % 10);
        val /= 10;
    }

    for (int i = len - 1; i >= 0; i--) {
        putchar(buffer[i]);
        count++;
    }
    return count;
}

int printf(char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    int count = 0;

    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1) != '\0') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    char *str = va_arg(args, char *);
                    count += print_str(str);
                    break;
                }
                case 'd': {
                    int val = va_arg(args, int);
                    count += print_int(val);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);  
                    putchar(c);
                    count++;
                    break;
                }
                default:
                    putchar('%');
                    putchar(*fmt);
                    count += 2;
                    break;
            }
        } else {
            putchar(*fmt);
            count++;
        }
        fmt++;
    }

    va_end(args);
    return count;
}
int main(){
    printf("hello%c%s%d\n",',',"world",2023);
    return 0;
}