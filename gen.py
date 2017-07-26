# -*- coding: utf-8 -*-
from random import randint

f = open("test/500mb_uni","w",encoding='utf-8')
for i in range(int(10000/3*2*500)):
	s = ""
	n = randint(1,100);
	for j in range(n):
		s += chr(randint(48,53248))
	f.write(s + "\n")
	# print(s)
f.close()