#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"

/*
    Windows abi:
        rcx - first integer argument
        rdx - second integer argument
        r8 - third integer argument
        r9 - fourth integer argument
        the stack is used for the rest, which can be controlled via the stack pointer `rsp`
*/

/*
    SysV abi:
        rdi - first integer argument
        rsi - second integer argument
        rdx - third integer argument
        rcx - fourth integer argument
        r8  - fifth integer argument
        r9  - sixth integer argument

*/

int add(int a, int b) {
    return a + b;
}

int add3(int a, int b, int c) {
    return a + b + c;
}

int add4(int a, int b, int c, int d) {
    return a + b + c + d;
}

int add5(int a, int b, int c, int d, int e) {
    return a + b + c + d + e;
}

int add6(int a, int b, int c, int d, int e, int f) {
    return a + b + c + d + e + f;
}

int add7(int a, int b, int c, int d, int e, int f, int g) {
    return a + b + c + d + e + f + g;
}

int add_float(float a, float b) {
    return a + b;
}

int add_float7(float a, float b, float c, float d, float e, float f, float g) {
    return a + b + c + d + e + f + g;
}


/*
    reference: "int result = ((int(*)())fptr)();"

        mov     rdx, QWORD PTR [rbp-32]
        mov     eax, 0
        call    rdx
        mov     DWORD PTR [rbp-4], eax

*/

/*
    reference: "int result = ((int(*)())fptr)(a1, a2);"

        mov     rcx, QWORD PTR [rbp-32]
        mov     edx, DWORD PTR [rbp-24]
        mov     eax, DWORD PTR [rbp-20]
        mov     esi, edx
        mov     edi, eax
        mov     eax, 0
        call    rcx
        mov     DWORD PTR [rbp-4], eax
*/

// DO NOT OPTIMIZE THIS HIGHER THAN 0
// I need the behavior to stay the EXACT same, since the optimization wouldn't account for many other things!
__attribute__((optimize(0)))
int caller(int a1, int a2, void* fptr) {
    register int32_t ires32 __asm__("eax");

    register int32_t move_register1 __asm__("edx");
    register int32_t move_register2 __asm__("eax");

    register int32_t i1arg32 __asm__("ecx");
    register int32_t i2arg32 __asm__("edx");

    i1arg32 = a2;
    i2arg32 = a1;

    // __asm__("call %P0" : : "i"(add));

    ((int(*)())fptr)();
    int r = ires32;
    // int(*result)() = ((int(*)())fptr);

    // move_register1 = a1;
    // move_register2 = a2;

    // i1arg32 = move_register1;
    // i2arg32 = move_register2;

    // int r = result();


    // int result = ires32;
    ires32 = 0;

    return r;
}

typedef struct Value {
    union {
        int32_t i32;
        float f32;
        double f64;
    } as;

    enum {
        I32,
        F32,
        F64,
    } is;
} Value;

typedef struct VM {
    Value* values;
    int len;
    int size;
} VM;

void init_vm(VM* vm) {
    vm->values = malloc(256 * sizeof(Value));
    vm->len = 0;
    vm->size = 256;
}

void clear_vm(VM* vm) {
    vm->len = 0;
}

void destroy_vm(VM* vm) {
    free(vm->values);
    vm->len = 0;
    vm->size = 0;
}

__attribute__((optimize(0)))
void pass_int(VM* vm, int a) {
    vm->values[vm->len] = (Value) {
        .as.i32 = a,
        .is = I32,
    };
    vm->len++;
}

__attribute__((optimize(0)))
void pass_float(VM* vm, float a) {
    vm->values[vm->len] = (Value) {
        .as.f32 = a,
        .is = F32,
    };
    vm->len++;
}

__attribute__((optimize(0)))
void pass_double(VM* vm, double a) {
    vm->values[vm->len] = (Value) {
        .as.f64 = a,
        .is = F64,
    };
    vm->len++;
}

// working on the x86 win32 version of the abi matching right now, then I will change it to work on linux as well.
__attribute((optimize(0)))
int call_func(VM* vm, void* fptr) {
    // probably going to have to do pure assembly here, well fuck.

    register int32_t ires32 __asm__("eax");
    register int32_t i1 __asm__("rcx");
    register int32_t i2 __asm__("rdx");
    register int32_t i3 __asm__("r8");
    register int32_t i4 __asm__("r9");
    register float f1 __asm__("xmm0");
    register float f2 __asm__("xmm1");
    register float f3 __asm__("xmm2");
    register float f4 __asm__("xmm3");
    register size_t sp __asm__("rsp");
    const register size_t bp __asm__("rbp");

    #define WIN32_ASSIGN_VALUE(value, integer_register, float_register)                     \
        if (value.is == I32) {                                                              \
            integer_register = value.as.i32;                                                \
        } else if (value.is == F32) {                                                       \
            float_register = value.as.f32;                                                  \
        } else if (value.is == F64) {                                                       \
            float_register = value.as.f64;                                                  \
        }


    size_t total_memory = 0;

    switch (vm->len) {
        default:
            if (vm->len == 0) break;
            {
                // total_memory = 32;
                int len = vm->len - 4;

                for (int i = 0; i < len; i++) {
                    // TODO: Add the checks for other types once I add them
                    if (vm->values[4+i].is == I32) {
                        total_memory += sizeof(int32_t);
                    } else if (vm->values[4+i].is == F32) {
                        total_memory += sizeof(float);
                    }
                }

                // sp -= total_memory;
                // printf("SP %i\n", sp);
                void* sptr = alloca(total_memory);
                // printf("SP %i\n", sp);

                // sp += total_memory;

                #define DEFAULT_X86_64_ALIGNMENT 8

                for (int i = 0; i < len; i++) {
                    if (vm->values[4+i].is == I32) {
                        volatile int32_t* ptr = (volatile int32_t*)sptr;

                        *ptr = vm->values[4+i].as.i32;

                        sptr += DEFAULT_X86_64_ALIGNMENT;
                    } else if (vm->values[4+i].is == F32) {
                        volatile float* ptr = (volatile float*)sptr;

                        *ptr = vm->values[4+i].as.f32;

                        sptr += DEFAULT_X86_64_ALIGNMENT;
                    }
                }

                #undef DEFAULT_X86_64_ALIGNMENT
            }

        case 4:
            WIN32_ASSIGN_VALUE(vm->values[3], i4, f4);
        case 3:
            WIN32_ASSIGN_VALUE(vm->values[2], i3, f3);
        case 2:
            WIN32_ASSIGN_VALUE(vm->values[1], i2, f2);
        case 1:
            WIN32_ASSIGN_VALUE(vm->values[0], i1, f1);
            break;
    }

    // i1 = vm->values[0].as.i32;
    // i2 = vm->values[1].as.i32;
    // i3 = vm->values[2].as.i32;
    // i4 = vm->values[3].as.i32;

    int r = ((int(*)())fptr)();

    // sp += total_memory;

    #undef WIN32_ASSIGN_VALUE

    return r;
}

int main() {
    printf("Hello, World!\n");

    printf("Hello %i\n", caller(3, 9, (void*)add));

    // pass_int1(2);
    // pass_int2(5);

    // printf("HELLO AGAIN %i\n", call_func((void*)add));

    VM vm;
    init_vm(&vm);

    pass_int(&vm, 2);
    pass_int(&vm, 3);

    printf("HELLOOOOOOO %i\n", call_func(&vm, (void*)add));

    clear_vm(&vm);

    pass_int(&vm, 3);
    pass_int(&vm, 2);
    pass_int(&vm, 5);
    pass_int(&vm, 6);

    printf("HELLOO AGAIN %i\n", call_func(&vm, (void*)add4));

    clear_vm(&vm);

    pass_int(&vm, 4);
    pass_int(&vm, 5);
    pass_int(&vm, 4);

    printf("HELLO3 %i\n", call_func(&vm, (void*)add3));

    clear_vm(&vm);

    pass_int(&vm, 1);
    pass_int(&vm, 2);
    pass_int(&vm, 3);
    pass_int(&vm, 4);
    pass_int(&vm, 19);
    pass_int(&vm, 9);
    pass_int(&vm, 9);

    printf("Hello5 %i\n", call_func(&vm, (void*)add7));

    clear_vm(&vm);

    pass_float(&vm, 3.5);
    pass_float(&vm, 3.5);

    printf("Hello Float %i\n", call_func(&vm, (void*)add_float));

    clear_vm(&vm);

    pass_float(&vm, 3.5);
    pass_float(&vm, 3.5);
    pass_float(&vm, 3.5);
    pass_float(&vm, 3.5);
    pass_float(&vm, 3.5);
    pass_float(&vm, 3.5);
    pass_float(&vm, 5.0);

    printf("Hello float7 %i\n", call_func(&vm, (void*)add_float7));

    destroy_vm(&vm);
    return 0;
}