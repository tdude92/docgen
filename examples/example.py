import time
import random

randomfluff = "for testing"
a = 1

def foo():
    #dg START FUNC
    #dg BRIEF prints "hello, world!" onto the console after 1 second.
    time.sleep(1)
    print("hello, world!")

def bar(a, b):
    #dg START FUNC
    #dg ARGS_START 
    # (int, float)a the first number
    # (int, float)b the second number
    #dg ARGS_END
    #dg RETURNS int, float
    #dg DESC_BEG
    # returns the sum of two numbers and a third random number.
    # second line just coz.
    #dg DESC_END
    return a + b + random.randint(1, 10)

class foobar:
    #dg START CLASS
    #dg DESC_BEG
    # foobar is a pretty cool class.
    # Why do foobars have age and cash?
    #dg DESC_END
    def __init__(self, dollars, age):
        #dg START FUNC
        #dg ARGS_START
        # dollars(int, float): How many dollars foobar has.
        # age(int): The age of foobar.
        #dg ARGS_END
        #dg BRIEF Default constructor.
        self.dollars = dollars
        self.age = age
    #dg END_CLASS

a = 20
string = "au revoir?"
print("coocoo")