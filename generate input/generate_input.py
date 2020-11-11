m = 100
n = 100
b = (m+n-2)*2
input_file_name = f'{m}x{n}.txt'

with open(input_file_name, 'w') as f:
    f.write(f'{m} {n} {b}')
    f.write('\n')
    f.write('1' + 'R' + '1'*(n-2) )
    f.write('\n')
    for i in range(m-2):
        f.write('1')
        f.write('0'*(n-2))
        f.write('1')
        f.write('\n')
    f.write('1'*n)
