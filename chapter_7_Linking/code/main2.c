// main.c
extern int shared;
void foo(void);

int main(void) {
    foo();
    return shared;
}
