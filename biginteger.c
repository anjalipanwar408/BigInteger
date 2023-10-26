#include "biginteger.h"
#include <stdio.h>
#include <stdlib.h>

void addAtFront(struct node** head, int digit) {
    struct node* newNode = (struct node*)malloc(sizeof(struct node));
    newNode->data = digit;
    newNode->next = *head;
    *head = newNode;
}

struct BigInteger initialize(char *s) {
    struct BigInteger result;
    result.L = NULL;
    result.length = 0;
    result.sign = 1;  // Initialize the sign as positive

    int i = 0;
    if (s[0] == '-') {
        result.sign = -1;  // Set the sign to negative
        i = 1;  // Start from the next character if there's a negative sign
    }

    for (; s[i] != '\0'; i++) {
        if (s[i] >= '0' && s[i] <= '9') {
            addAtFront(&(result.L), s[i] - '0'); // Convert character to integer
            result.length++;
        }
    }

    return result;
}

void display(struct BigInteger num) {
    struct node* current = num.L;
    if (num.sign == -1) {
        printf("-");
    }

    // Create an array to store the digits in reverse order
    int* digits = (int*)malloc(num.length * sizeof(int));
    int index = 0;

    while (current != NULL) {
        digits[index] = current->data;
        current = current->next;
        index++;
    }

    // Print the digits in the original order
    for (int i = index - 1; i >= 0; i--) {
        printf("%d", digits[i]);
    }

    free(digits);
    printf("\n");
}

int compareBigIntegers(struct BigInteger a, struct BigInteger b) {
    if (a.length < b.length) {
        return -1; // a is smaller than b
    } else if (a.length > b.length) {
        return 1;  // a is larger than b
    } else {
        struct node* p1 = a.L;
        struct node* p2 = b.L;
        while (p1 != NULL && p2 != NULL) {
            if (p1->data < p2->data) {
                return -1; // a is smaller than b
            } else if (p1->data > p2->data) {
                return 1;  // a is larger than b
            }
            p1 = p1->next;
            p2 = p2->next;
        }
    }
    return 0;  // a is equal to b
}

struct BigInteger sub(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result;
    result.L = NULL;
    result.length = 0;

    if (a.sign == 1 && b.sign == -1) {
        b.sign = 1;
        result = add(a, b);
        return result;
    } else if (a.sign == -1 && b.sign == 1) {
        b.sign = -1;
        result = add(a, b);
        return result;
    }

    int compareLength = compareBigIntegers(a, b);

    if (compareLength == -1) { // If a is smaller than b, swap them
        struct BigInteger temp = a;
        a = b;
        b = temp;
        result.sign = -1;
    } else if (compareLength == 0) { // If both numbers are equal, the result is zero
        return result;
    } else {
        result.sign = 1;
    }
    int borrow = 0;
    struct node* p1 = a.L;
    struct node* p2 = b.L;
    while (p1 != NULL || p2 != NULL) {
        int x = (p1 != NULL) ? p1->data : 0;
        int y = (p2 != NULL) ? p2->data : 0;

        int diff = x - y - borrow;
        if (diff < 0) {
            diff += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        addAtFront(&(result.L), diff);
        result.length++;

        if (p1 != NULL) p1 = p1->next;
        if (p2 != NULL) p2 = p2->next;
    }

    while (result.L != NULL && result.L->data == 0) {
        struct node* temp = result.L;
        result.L = result.L->next;
        free(temp);
        result.length--;
    }

    // Reverse the result before returning
    struct BigInteger reversedResult;
    reversedResult.L = NULL;
    reversedResult.sign = result.sign;
    reversedResult.length = result.length;
    struct node* current = result.L;
    while (current != NULL) {
        addAtFront(&(reversedResult.L), current->data);
        current = current->next;
    }

    return reversedResult;
}

struct BigInteger add(struct BigInteger a, struct BigInteger b) {
    struct BigInteger result;
    result.L = NULL;
    result.length = 0;

    if (a.sign == b.sign) {
        result.sign = a.sign;
    } else {
        if (a.sign == 1) {
            b.sign = 1;
            result = sub(a, b);
        } else {
            b.sign = -1;
            result = sub(b, a);
        }
        return result;
    }

    int carry = 0;
    struct node* p1 = a.L;
    struct node* p2 = b.L;

    while (p1 != NULL || p2 != NULL || carry) {
        int sum = carry;
        if (p1 != NULL) {
            sum += p1->data;
            p1 = p1->next;
        }
        if (p2 != NULL) {
            sum += p2->data;
            p2 = p2->next;
        }

        carry = sum / 10;
        sum %= 10;

        addAtFront(&(result.L), sum);
        result.length++;
    }

    // Reverse the result before returning
    struct BigInteger reversedResult;
    reversedResult.L = NULL;
    reversedResult.sign = result.sign;
    reversedResult.length = result.length;
    struct node* current = result.L;
    while (current != NULL) {
        addAtFront(&(reversedResult.L), current->data);
        current = current->next;
    }

    return reversedResult;
}

struct BigInteger mul(const struct BigInteger a, const struct BigInteger b) {
    struct BigInteger result;
    result = initialize("0");

    if (a.sign == -1) {
        result.sign = a.sign;  // Adjust the sign for negative input 'a'
    }

    if (b.sign == -1) {
        result.sign = b.sign;  // Adjust the sign for negative input 'b'
    }

    struct node* p1 = a.L;
    int shift = 0;

    while (p1 != NULL) {
        int carry = 0;
        struct node* p2 = b.L;
        struct BigInteger temp;
        temp.L = NULL;
        temp.length = 0;
        temp.sign = 1;

        while (p2 != NULL) {
            int product = (p1->data * p2->data) + carry;
            carry = product / 10;
            product %= 10;
            addAtFront(&(temp.L), product);
            temp.length++;
            p2 = p2->next;
        }

        if (carry > 0) {
            addAtFront(&(temp.L), carry);
            temp.length++;
        }

        for (int i = 0; i < shift; i++) {
            addAtFront(&(temp.L), 0);
            temp.length++;
        }

        result = add(result, temp);
        shift++;
        p1 = p1->next;
    }

    return result;
}

struct BigInteger div1(struct BigInteger a, struct BigInteger b) {
    struct BigInteger quotient;
    quotient = initialize("0");

    int sign = (a.sign == b.sign) ? 1 : -1;
    a.sign = 1;
    b.sign = 1;

    while (compareBigIntegers(a, b) >= 0) {
        a = sub(a, b);
        quotient = add(quotient, initialize("1"));
    }

    quotient.sign = sign;

    return quotient;
}

void reverseBigInteger(struct BigInteger *num) {
    struct node *prev = NULL;
    struct node *current = num->L;
    struct node *next = NULL;

    while (current != NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }

    num->L = prev;  // Update the head of the linked list
}

struct BigInteger mod(struct BigInteger a, struct BigInteger b) {
    struct BigInteger zero;
    zero.L = NULL;
    zero.length = 1;
    zero.sign = 1;

    if (compareBigIntegers(b, zero) == 0) {
        printf("Modulus by zero is not allowed.\n");
        return zero;
    }

    // Ensure both numbers are positive for modulus
    a.sign = 1;
    b.sign = 1;

    // Calculate the division result
    struct BigInteger divisionResult = div1(a, b);
    reverseBigInteger(&divisionResult);

    // Calculate the product of division result and b
    struct BigInteger product = mul(divisionResult, b);
    reverseBigInteger(&product);

    // Calculate the modulus as a - product
    struct BigInteger result = sub(a, product);
    reverseBigInteger(&result);

    return result;
}
