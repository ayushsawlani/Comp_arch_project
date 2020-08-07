import random
import math
def convert(x):
    s=""
    while(x>0):
        s=s+(str(x%2))
        x=x/2
    while(len(s)<32):
        s=s+str(random.randint(0,2)%2)
    return s
    
file=open("memory.txt","w")
x=int(pow(10,6))
for i in range(x):
    file.write(convert(random.randint(0,x)))
    file.write("\n")
file.close()