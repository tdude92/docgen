import time
import random

randomfluff = "for testing"
a = 1

def foo():
    #dg START FUNC foo
    #dg DESC prints "hello, world!" onto the console after 1 second.
    #dg END
    time.sleep(1)
    print("hello, world!")

def bar(a, b):
    #dg START FUNC bar
    #dg ARGS a(int, float), b(int, float)
    #dg DESC returns the sum of two numbers and a third random number.
    #dg DESC second line just coz.
    #dg END
    return a + b + random.randint(1, 10)

a = 20
string = "au revoir?"
print("coocoo")