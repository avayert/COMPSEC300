"""
03_multiplication.py

Takes in two decimal numbers, multiplies them, and prints the resulting output.

The calculation is done using integers so there is no loss of precision.
Python has arbitrary-size integers, which makes it so we do not have to worry about overflow.

Only valid floating numbers are allowed as input.
"""

def is_float(s):
    # use this monstrosity over try-except and float()
    # to not support the X.Ye±N notation, inf and nan.
    if s.startswith('-'):
        s = s[1:]

    a, sep, b = s.partition('.')
    if not sep:
        return False

    if not (a.isdigit() and b.isdigit()):
        return False

    return True



while True:
    response = input('Input two decimal numbers to multiply: ').strip()

    a, sep, b = response.partition(' ')
    if not sep:
        print('Invalid input. Please enter two decimal numbers with a space as the delimiter.')
        continue

    # we also manually loop instead of using any() to tell the user
    # which number failed.
    for val in (a, b):
        if not is_float(val):
            print(val, 'is not a valid decimal number.')
            break
    else:
        break

# TODO what a mess
values = [val.strip().partition('.') for val in (a, b)]
(a_pre, _, a_post), (b_pre, _, b_post) = values
n = str(int(a_pre + a_post) * int(b_pre + b_post))
n_decimal = len(a_post) + len(b_post)

print(n[:-n_decimal], '.', n[-n_decimal:].rstrip('0.'), sep='')
