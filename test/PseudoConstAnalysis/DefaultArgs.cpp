// RUN: %clang_cc1 %s -fsyntax-only -verify

void mutator(int & i, int * j = 0) {
    if (j) {
        int const k = i;

        *j = i;
        i = k;
    } else {
        i = 0;
    }
}

void test_1() {
    int i = 10;
    mutator(i);
}

void test_2() {
    int i = 10;
    mutator(i, &i);
}