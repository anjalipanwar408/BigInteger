#ifndef BIGINTEGER_H
#define BIGINTEGER_H

struct node {
    int data;
    struct node* next;
};

struct BigInteger {
    struct node* L;
    int length;
    int sign;
};

void addAtFront(struct node** head, int digit);
struct BigInteger initialize(char *s);
void display(struct BigInteger num);
int compareBigIntegers(struct BigInteger a, struct BigInteger b);
struct BigInteger sub(struct BigInteger a, struct BigInteger b);
struct BigInteger add(struct BigInteger a, struct BigInteger b);
struct BigInteger mul(const struct BigInteger a, const struct BigInteger b);
struct BigInteger div1(struct BigInteger a, struct BigInteger b);
void reverseBigInteger(struct BigInteger *num);
struct BigInteger mod(struct BigInteger a, struct BigInteger b);

#endif
