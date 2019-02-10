prompt = 'Please enter the first number: ';
num1 = input(prompt, 's')
prompt = 'Please enter the second number: ';
num2 = input(prompt, 's')

num1 = pad(num1, strlength(num2), 'left', '0')
num2 = pad(num2, strlength(num2), 'left', '0')

first = [];
next = [];

for i = strlength(num1):1
    first = cat(2, first, num1(i))
    next = cat(2, first, num2(i))
end