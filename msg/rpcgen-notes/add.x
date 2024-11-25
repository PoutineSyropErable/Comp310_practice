/* add.x: RPC program to add two integers using a struct */

struct numbers {
    int a;
    int b;
};

program ADD_PROG {
    version ADD_VERS {
        int add(numbers) = 1;
    } = 1;
} = 0x12345;
