import sys
s = sys.argv[1].split("0x")
b = list()
f = str()
if len(s) == 1:
    s = s[0]
else:
    s = s[1]
for i in range(0, len(s), 2):
    b.append(s[i:i+2])
b = b[::-1]
for y in b:
    f = f + y
print("==> 0x{}".format(f))
