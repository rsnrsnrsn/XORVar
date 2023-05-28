# XORVar
 Runtime xor protection for values, pointers and functions with ease of use in mind.

## example usage
```cpp
    // Testing generic number
    XV::XORVar<unsigned> xorNumber;
    printf("%u\n", xorNumber());
    xorNumber = 10;
    printf("%u\n", xorNumber());
    xorNumber -= 1;
    printf("%u\n", xorNumber());

    // Testing pointer
    XV::XORVar<TestClass*> xorPointer = new TestClass();
    xorPointer->var = 20;
    xorPointer->printVar();

    // Testing function
    XV::XORFunc<decltype(testPrint)*> xorFunction(testPrint);
    xorFunction("test\n");
```

As seen in the example XORVar objects can be used nearly identically to the type stored within them, the enc/dec happens on access automatically. 
