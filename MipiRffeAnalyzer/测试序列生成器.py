from random import randint
X = 0 
C = 0
Res = []
def paritybit(bits):
    b = bits
    count = 0
    while b > 0:
        count += b % 2
        b = b//2
    if count % 2:
        return 0
    else:
        return 1 

def datagen():
    while True:
        yield randint(0, 255)
          
def usidgen():
    x = 0
    while True:
        yield x 
        x += 1
        if x > 15:
            x = 0        

data = datagen()
usid = usidgen()

def frame80():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(1, 1)
    addbits(7)
    addparity(12)
    addbpc() 
    Res.append((X, C),)

def frame60():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(3, 3)
    addbits(5)
    addparity(12)
    addbpc() 
    addbits(8) 
    addparity(8) 
    addbpc() 
    Res.append((X, C),)
    
    
def addbits(n):
    global X 
    global C 
    X = X << n 
    d = next(data) % 2**n
    X += d
    C += n 
    return d 
    
def addparity(n):
    global X
    global C 
    p = paritybit(X%2**n)
    X = (X << 1) + p
    C += 1 
    
def addbpc():
    global X
    global C 
    X = X << 1 
    X += 0
    C += 1 
    
def addnum(x, n):
    global X
    global C 
    X = X << n 
    X += x
    C += n 
    
def frame40():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(2, 3)
    addbits(5)
    addparity(12) 
    addbits(8) 
    addparity(8) 
    addbpc() 
    Res.append((X, C),) 

def frame38():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(7, 5)
    addnum(0, 2)
    bc = addbits(1) + 1
    addparity(12) 
    addbits(8) 
    addparity(8) 
    addbits(8)
    addparity(8)
    addbpc() 
    for i in range(bc):
        addbits(8)
        addparity(8)
    addbpc()    
    Res.append((X, C),)       
    
def frame30():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(6, 5)
    addnum(0, 2)
    bc = addbits(1) + 1
    addparity(12) 
    addbits(8) 
    addparity(8) 
    addbits(8)
    addparity(8)
    for i in range(bc):
        addbits(8)
        addparity(8)
    addbpc()    
    Res.append((X, C),)     
    
def frame20():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(2, 4)
    addnum(0, 2)
    bc = addbits(2) + 1
    addparity(12) 
    addbits(8) 
    addparity(8) 
    addbpc() 
    for i in range(bc):
        addbits(8)
        addparity(8)
    addbpc()    
    Res.append((X, C),) 
        
def frame1e():        
    global X, C 
    X = 0 
    C = 0 
    addnum(0, 2)
    addbits(2) 
    addnum(0x1e, 8)
    addparity(12)
    addbpc()
    addnum(4, 3)
    addbits(2)
    addnum(0, 3)
    addparity(8)
    addbpc()    
    Res.append((X, C),)
    
def frame1d():
    global X, C
    X = 0 
    C = 0 
    addnum(0, 2)
    addbits(2) 
    addnum(0x1d, 8)
    addparity(12)
    #addbpc()
    addbits(8)
    addparity(8)
    addbits(8)
    addparity(8)
    addbits(8)
    addparity(8)
    addbpc()    
    Res.append((X, C),)
    
def frame1c():
    global X, C
    X = 0 
    C = 0 
    addnum(0, 2)
    addbits(2) 
    addnum(0x1c, 8)
    addparity(12)
    addbits(8)
    addparity(8)
    addbpc()
    addbits(8)
    addparity(8)
    addbits(8)
    addparity(8)
    addbpc()    
    Res.append((X, C),)    
    
def frame1b():
    global X, C
    X = 0 
    C = 0 
    addnum(0, 2)
    addbits(2) 
    addnum(0x1b, 8)
    addparity(12)
    #addbpc()
    addnum(0, 7)
    bc = addbits(1) + 1
    addparity(8)
    addbits(8)
    addparity(8)
    for i in range(bc):
        addbits(8)
        addparity(8)

    addbpc()    
    Res.append((X, C),)  

def frame1a():
    global X, C
    X = 0 
    C = 0 
    addnum(0, 2)
    addbits(2) 
    addnum(0x1a, 8)
    addparity(12)
    addnum(0, 7)
    bc = addbits(1) + 1
    addparity(8)
    addbits(8)
    addparity(8)
    addbpc()
    for i in range(bc):
        addbits(8)
        addparity(8)
    addbpc()    
    Res.append((X, C),)   

def frame19():
    global X, C
    X = 0 
    C = 0 
    addbits(4) 
    addnum(0x19, 8)
    addparity(12)
    addbits(8)
    addparity(8)
    addbits(8)
    addparity(8)
    addbits(8)
    addparity(8)
    addbpc()    
    Res.append((X, C),)

def frame10():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(randint(0x10, 0x18), 8)
    addparity(12)
    addbpc() 
    Res.append((X, C),)
    
def frame00():
    global X, C 
    X = 0 
    C = 0 
    addbits(4) 
    addnum(0, 4)
    addnum(0, 2)
    bc = addbits(2) + 1
    addparity(12) 
    addbits(8) 
    addparity(8)
    for i in range(bc):
        addbits(8)
        addparity(8)
    addbpc()    
    Res.append((X, C),)

  
for i in range(7):
    frame80()
    frame60()
    frame40()
    frame38()
    frame00()
    frame30()
    frame20()
    frame19()
    frame1c()
    frame1d()
    frame1e()
    frame1a()
    frame1b()

frame10()  # 类型错误  
Res.append((0x2f92, 14),) # 校验错误  
Res.append((0xa63800, 24),) # 无回应错误 

with open('seq.csv','w') as fp:
    for x, c in Res:
        print(x, c)
        fp.write('0x%X,%d\n' %(x,c))
    