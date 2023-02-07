# Memory Pool Exercise

A memory pool design and implementation in C++ code.
The algorithm is inspried by boost::SimpleSegregatedStorage.

# Performance
| Data Type                  | System Memory Management Time | Memory Pool Time | Speed Up  |
| -------------------------- | ----------------------------- | ---------------- | --------- |
| ByteType                   | 0.000044387                   | 0.000018622      | 58.0463%  |
| PointerType                | 0.000031069                   | 0.000020163      | 35.1025%  |
| FixedStringType[256]       | 0.000048720                   | 0.000088923      | -82.5185% |
| struct   Point             | 0.000025300                   | 0.000014434      | 42.9486%  |
| class Base1                | 0.000026345                   | 0.000018855      | 28.4304%  |
| class Derived              | 0.000030076                   | 0.000021254      | 29.3324%  |
| class NoDefaultConstructor | 0.000024622                   | 0.000015146      | 38.4859%  |

# Directories
- example: Test with `ExampleClasses.h` and test the performance of each type.
- include: Code implementation of memory pool.
- test: Unit tests of the implementation.

