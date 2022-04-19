#ifndef PROJECT_HAIKU_H
#define PROJECT_HAIKU_H

typedef struct haiku_{
    char author[64];
    char lines[3][64];
} haiku;

typedef struct book_{
    int size;
    haiku *poems;
}book;

typedef enum category_{
    japanese,
    western
} category;

typedef struct file_category_{
    category c;
    char file[64];
} file_category;

book read_book(category c);
haiku select_random(book b);
void print_haiku(haiku h);
void free_book(book *b);

#endif //PROJECT_HAIKU_H