/* calculator.x: RPC program for basic arithmetic operations */

struct operands {
    double a;
    double b;
};

program CALCULATOR_PROG {
    version CALCULATOR_VERS {
        double ADD(operands) = 1;
        double SUBTRACT(operands) = 2;
        double MULTIPLY(operands) = 3;
        double DIVIDE(operands) = 4;
    } = 1;
} = 0x20000001;
