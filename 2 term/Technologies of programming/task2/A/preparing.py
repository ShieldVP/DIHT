import sys
if __name__ == '__main__':
    with open(sys.argv[1], 'w') as gen:
        gen.write('const int A = 5;')
