# Dynamic Function Calling
 
This is a little repository made by me to test out if it was possible to implement dynamic function calling in C and inline assembly (instead of using a mix of lone assembly files and C). This project can only be compiled with gcc and clang, since it uses features that aren't exactly standard to C. I also only have it working for windows at the moment (or for the rest of eternity) as I really do not want to spend more time on the project currently. 

Here's a little code example:
```C
#include "vm.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t add(int32_t a, int32_t b) {
    return a + b;
}

int32_t* return_int_pointer(uint64_t size, int32_t value) {
    int32_t* result = (int32_t*)malloc(size);

    *result = value;

    return result;
}

int main() {
    // a little "virtual machine" to host the values that will be passed to the called function at runtime
    VM vm;
    init_vm(&vm);

    pass_i32(&vm, 2);
    pass_i32(&vm, 4);

    printf("The output of the addition is %i\n", call_func_i32(&vm, (void*)add));

    // to clear out the values passed in from the previous function call
    clear_vm(&vm);

    pass_u64(&vm, 4);
    pass_i32(&vm, 514);

    int32_t* result = (int32_t*)call_func_ptr(&vm, (void*)return_int_pointer);

    printf("The output of the function returning the pointer is %i\n", *result);

    free(result);    
    destroy_vm(&vm);
    return 0;
}
```

The library also has the ability to construct Structure types and pass them into functions, though the ability to return structure values and reinterpret it still does not work properly.

Currently the project can handle allocating neccessary space for arguments passed through the stack, constructing complex structure types and passing the values as arguments, and mostly supporting a small subset of the win32 x86_64 abi.

Thank you for reading this and I hope you have a good day!