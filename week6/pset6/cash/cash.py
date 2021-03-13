from decimal import Decimal


def main():
    n = get_cash()
    coins = 0

    a = Decimal('0.25')
    while n - a >= 0:
        n -= a
        coins += 1

    b = Decimal('0.10')
    while n - b >= 0:
        n -= b
        coins += 1

    c = Decimal('0.05')
    while n - c >= 0:
        n -= c
        coins += 1

    d = Decimal('0.01')
    while n - d >= 0:
        n -= d
        coins += 1

    print(coins)


def get_cash():
    c = 0
    while c <= 0:
        try:
            c = Decimal(input("Change owed: "))
        except:
            pass

    return c


if __name__ == "__main__":
    main()
