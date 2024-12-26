# Mini-LISP-Interpreter

This is an mini lisp interpreter.

## How to run

You can just run the code below in your shell. The script would compile the file and run it.

```
./run.sh
```

Or, you can compile `src/main.l` and `src/main.y` by yourself to run it.

## Todo

- [x] Syntax Validation Print “syntax error” when parsing invalid syntax
- [x] Print Implement print-num statement
- [x] Numerical Operations Implement all numerical operations
- [x] Logical Operations Implement all logical operations
- [x] if Expression Implement if expression
- [x] Variable Definition Able to define a variable
- [x] Function Able to declare and call an anonymous function
- [x] Named Function Able to declare and call a named function
- [x] Recursion Support recursive function call
- [ ] Type Checking Print error messages for type errors
- [ ] Nested Function Nested function (static scope)
- [ ] First-class Function Able to pass functions, support closure

## Main Concept

This project uses `yacc` to parse input and construct an Abstract Syntax Tree (AST) using `Node*`.
After building the AST, the `eval()` function is called to traverse the tree and evaluate the code.

### Data Structures

- **Element**: Stores the value of a node in the AST.
- **Node**: Represents a single node in the AST, with pointers to its left and right child nodes.

```c
typedef struct Element {
    int type;     // Type of the element (e.g., integer, string, etc.)
    int ival;     // Integer value, if applicable
    char* cval;   // Character/string value, if applicable
} Element;

typedef struct Node {
    Element* val;      // Value of the node
    struct Node* left; // Pointer to the left child
    struct Node* right;// Pointer to the right child
} Node;
```

## EQUAL_OP

The EQUAL operator is designed to evaluate expressions involving multiple values, considering both subtrees and their relationships.
This requires analyzing specific conditions depending on the structure of the input.

### Example

For an input like `(= a b c d)`, the parsing result would resemble the tree structure below:

```
    EQUAL
  /     \
 a       EXPRS
        /   \
     EXPRS   d
    /   \
    b    c
```

The evaluation logic ensures that all elements are compared in sequence, such that the result of `(= a b c d)` is equivalent to:
`(a == d) && ((d == c) && (b == c))` which is equivalent to `(a == d == c == b)`.
