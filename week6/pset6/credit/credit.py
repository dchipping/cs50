'''
Example No.
10 98 76 54 32 10
 2 42 13 60 20 34

American Express uses 15-digit numbers,
MasterCard uses 16-digit numbers,
Visa uses 13- and 16-digit numbers.

American Express numbers start with 34 or 37;
MasterCard numbers start with 51, 52, 53, 54, or 55
Visa numbers start with 4.
'''


def main():
    n = get_number()
    if isCheckSumValid(n):
        cardType(n)
    else:
        print("INVALID")


def isCheckSumValid(n):
    l = length(n)

    # If the length is not 13, 15, 16 false
    if l != 13 and l != 15 and l != 16:
        return False

    sum1 = 0
    for i in range(1, l, 2):
        digit = accessDigit(n, i) * 2
        for j in range(length(digit)):
            sum1 += accessDigit(digit, j)

    sum2 = 0
    for k in range(0, l, 2):
        digit = accessDigit(n, k)
        sum2 += digit

    return ((sum1 + sum2) % 10 == 0)


def cardType(n):
    l = length(n)

    first = accessDigit(n, l-1)
    second = accessDigit(n, l-2)
    startDigits = (first * 10) + second

    if (l == 13 or l == 16) and first == 4:
        print("VISA")
    elif l == 15 and (startDigits == 34 or startDigits == 37):
        print("AMEX")
    elif l == 16 and (startDigits >= 51 and startDigits <= 55):
        print("MASTERCARD")
    else:
        print("INVALID")


def length(n):
    l = 0
    while n > 0:
        n //= 10
        l += 1

    return l


def accessDigit(n, p):
    return (n // pow(10, p)) % 10


def get_number():
    n = 0
    while n <= 0:
        try:
            n = int(input("Number: "))
        except:
            pass

    return n


if __name__ == "__main__":
    main()
