import time
import random

randomfluff = "for testing"
a = 1

def foo():
    #dg START FUNC
    #dg BRIEF prints "hello, world!" onto the console after 1 second.
    #dg END FUNC
    time.sleep(1)
    print("hello, world!")

def bar(a, b):
    #dg START FUNC
    #dg START ARGS
    # (int float) a: the first number
    # (int float) b
    #dg END ARGS
    #dg RETURNS int float
    #dg START DESC
    # returns the sum of two numbers and a third random number.
    # second line just coz.
    #dg END DESC
    #dg END FUNC
    return a + b + random.randint(1, 10)

class foobar:
    #dg START CLASS
    #dg START DESC
    # foobar is a pretty cool class.
    # Why do foobars have age and cash?
    #dg END DESC
    def __init__(self, dollars, age):
        #dg START FUNC
        #dg START ARGS
        # (int float) dollars: How many dollars foobar has.
        # (int) age: The age of foobar.
        #dg END ARGS
        #dg BRIEF Default constructor.
        #dg END FUNC
        self.dollars = dollars
        self.age = age
    #dg END CLASS

a = 20
string = "au revoir?"
print("coocoo")