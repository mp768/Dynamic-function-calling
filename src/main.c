#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>
#include <time.h>

#include "vm.h"

//void* alloca(size_t mem);

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

// DO NOT OPTIMIZE HIGHER THAN 0
// I need the behavior to stay the EXACT same, since the optimization wouldn't account for many other things!

// reference assembly
//  __asm__("mov     %%rax, %0" :: "g"(value));      \
//  __asm__("lea     %rdx, [%rax+8]");                \
    __asm__("mov     %eax, 16");                     \
    __asm__("sub     %rax, 1");                      \
    __asm__("add     %rax, %rdx");                    \
    __asm__("mov     %ecx, 16");                     \
    __asm__("mov     %edx, 0");                      \
    __asm__("div     %rcx");                         \
    __asm__("imul    %rax, %rax, 16");                \
    __asm__("sub     %rsp, %rax");                    \
    __asm__("mov     %rax, %rsp");                    \
    __asm__("add     %rax, 15");                     \
    __asm__("shr     %rax, 4");                      \
    __asm__("sal     %rax, 4");                      \
    __asm__("mov     %0, %%rax" :: "g"(output));

// produces exit code "-1073741819"
#define gcc_alloca_impl(output, value)                      \
    __asm__("mov     %0, %%rax" :: "g"(value));      \
    __asm__("lea     8(%rax), %rdx");                \
    __asm__("mov     $16, %eax");                     \
    __asm__("sub     $1, %rax");                      \
    __asm__("add     %rdx, %rax");                    \
    __asm__("mov     $16, %ecx");                     \
    __asm__("mov     $0, %edx");                      \
    __asm__("div     %rcx");                         \
    __asm__("imul    $16, %rax, %rax");                \
    __asm__("sub     %rax, %rsp");                    \
    __asm__("mov     %rsp, %rax");                    \
    __asm__("add     $15, %rax");                     \
    __asm__("shr     $4, %rax");                      \
    __asm__("sal     $4, %rax");                      \
    __asm__("mov     %%rax, %0" :: "g"(output));

int add8s(int8_t a, int8_t b, int8_t c, int8_t d, int8_t e, int8_t f) {
    return (int)a + b + c + d + e + f;
}

int add_random_stuff(int32_t a, int8_t b, float c, double d, int32_t e, int8_t f) {
    printf("a is %i\n", a);
    printf("b is %i\n", b);
    printf("c is %g\n", c);
    printf("d is %g\n", d);
    printf("e is %i\n", e);
    printf("f is %i\n", f);

    return a + b + c + d + e + f;
}

int say_hi(const char* str) {
    printf("hello %s\n", str);
    return 2;
}

int modify_value(double* a) {
    *a = 4.5;
    return 0;
}

const char* get_some_string() {
    return "Hello String";
}

uint64_t get_an_int(int32_t j) {
    uint64_t sum = 0;

    for (int32_t i = j; i >= 0; i--) {
        sum += i;
        sum *= 2;
        sum -= 1;
    }

    return sum;
}

struct TESTSTRUCT {
    int8_t a;
    int8_t b;
    int32_t c;
    int8_t d;
};

struct TESTSTRUCT2 {
    struct TESTSTRUCT a;
    int64_t b;
};

typedef struct {
    int64_t a;
    uint64_t b;
} TestStruct3;

uint64_t get_value_from_struct(TestStruct3 val) {
    return (uint64_t)val.a + val.b; 
}

TestStruct3 get_struct_value_from_value(uint64_t a) {
    return (TestStruct3) {
        .a = (int64_t)(a * 2),
        .b = a,
    };
}

int main() {
    printf("Hello, World!\n");

    //printf("Hello %i\n", caller(3, 9, (void*)add));

    // pass_i321(2);
    // pass_i322(5);

    // printf("HELLO AGAIN %i\n", call_func((void*)add));

    VM vm;
    init_vm(&vm);

    /*
    clock_t start = clock();

    double temp_val = 0;
    double temp_sum = 0;
    for (size_t i = 0; i < INT_MAX / 64; i++) {
        pass_ptr(&vm, (void*)&temp_val);
        call_func(&vm, (void*)modify_value);

        temp_sum += temp_val;

        clear_vm(&vm);
    }

    clock_t end = clock();

    double time_spent = (double)(end-start);

    printf("Temp sum: %g\nTime spent %g\nstart %ld\nend %ld\n", temp_sum, time_spent, start, end);
    */

    pass_i32(&vm, 2);
    pass_i32(&vm, 3);

    printf("HELLOOOOOOO %i\n", call_func_i32(&vm, (void*)add));

    clear_vm(&vm);

    pass_i32(&vm, 3);
    pass_i32(&vm, 2);
    pass_i32(&vm, 5);
    pass_i32(&vm, 6);

    printf("HELLOO AGAIN %i\n", call_func_i32(&vm, (void*)add4));

    clear_vm(&vm);

    pass_i32(&vm, 4);
    pass_i32(&vm, 5);
    pass_i32(&vm, 4);

    printf("HELLO3 %i\n", call_func_i32(&vm, (void*)add3));

    clear_vm(&vm);

    pass_i32(&vm, 1);
    pass_i32(&vm, 2);
    pass_i32(&vm, 3);
    pass_i32(&vm, 4);
    pass_i32(&vm, 19);
    pass_i32(&vm, 9);
    pass_i32(&vm, 9);

    printf("Hello5 %i\n", call_func_i32(&vm, (void*)add7));

    clear_vm(&vm);

    pass_f32(&vm, 3.5);
    pass_f32(&vm, 3.5);

    printf("Hello Float %i\n", call_func_i32(&vm, (void*)add_float));

    clear_vm(&vm);

    pass_f32(&vm, 3.5);
    pass_f32(&vm, 3.5);
    pass_f32(&vm, 3.5);
    pass_f32(&vm, 3.5);
    pass_f32(&vm, 3.5);
    pass_f32(&vm, 3.5);
    pass_f32(&vm, 5.67);

    printf("Hello float7 %i\n", call_func_i32(&vm, (void*)add_float7));

    clear_vm(&vm);

    pass_i8(&vm, 1);
    pass_i8(&vm, 1);
    pass_i8(&vm, 1);
    pass_i8(&vm, 1);
    pass_i8(&vm, 1);
    pass_i8(&vm, -1);

    printf("Hello i8s %i\n", call_func_i32(&vm, (void*)add8s));

    clear_vm(&vm);

    pass_i32(&vm, 3);
    pass_i8(&vm, 14);
    pass_f32(&vm, 23.45f);
    pass_f64(&vm, 45.65);
    pass_i32(&vm, 4);
    pass_i8(&vm, 12);

    printf("HELLO RESULT %i\n", call_func_i32(&vm, (void*)add_random_stuff));

    clear_vm(&vm);
    
    pass_str(&vm, "Michael");

    call_func(&vm, (void*)say_hi);

    clear_vm(&vm);
    
    double dval = 0.0;

    pass_ptr(&vm, (void*)&dval);

    printf("Before value is modified it is %g\n", dval);

    call_func(&vm, (void*)modify_value);

    printf("The modifed values is now %g\n", dval);

    clear_vm(&vm);

    const char* temp_str = call_func_str(&vm, (void*)get_some_string);
    
    printf("Output of function with return of string is \"%s\"\n", temp_str);
   
    clear_vm(&vm);

    pass_i32(&vm, 50);

    printf("Output of function is %llu\n", call_func_u64(&vm, (void*)get_an_int));

    StructConstructor sc;

    init_struct_constructor(&sc);

    start_creating_struct_type(&sc, false);

    add_field_i8(&sc);
    add_field_i8(&sc);
    add_field_i32(&sc);
    add_field_i8(&sc);

    StructType* type = construct_struct_type(&sc);

    printf("Len %i, Size %i, Struct_Size %i, Actual Struct Size %i\n", type->len, type->size, type->struct_size, sizeof(struct TESTSTRUCT));

    for (int i = 0; i < type->len; i++) {
        printf("Pos %i, Type %i\n", type->elements[i].pos, type->elements[i].type);
    }

    start_creating_struct_type(&sc, false);

    add_field_struct(&sc, type);
    add_field_i64(&sc);

    StructType* type2 = construct_struct_type(&sc);
    
    // the sizes match up, interesting!
    printf("Lne %i, Size %i, Struct Size %i, Actual Struct Size %i\n", type2->len, type2->size, type2->struct_size, sizeof(struct TESTSTRUCT2));

    for (int i = 0; i < type2->len; i++) {
        printf("Pos %i, Type %i\n", type2->elements[i].pos, type2->elements[i].type);
    }

    start_creating_struct_type(&sc, false);
    add_field_i64(&sc);
    add_field_u64(&sc);

    StructType* type3 = construct_struct_type(&sc);

    StructValue* value = make_struct_value(type3);

    set_struct_field_i64(value, 0, 3);
    set_struct_field_u64(value, 1, 4);

    clear_vm(&vm);

    pass_struct(&vm, value);

    printf("Value of operation with struct type is %u\n", call_func_u64(&vm, (void*)get_value_from_struct));

    printf("First Struct field 0 is %lli\n", get_struct_field_i64(value, 0));
    printf("First Struct field 1 is %llu\n", get_struct_field_u64(value, 1));

    clear_vm(&vm);

    pass_u64(&vm, 2);

    TestStruct3 a = get_struct_value_from_value(2);

    printf("RESULT IS %lli and %llu\n", a.a, a.b);

    // I couldn't get struct returns to work, so do not use it :)
    // StructValue* value2 = call_func_struct(&vm, type3, (void*)get_struct_value_from_value);

    // printf("Result Struct field 0 is %lli\n", get_struct_field_i64(value2, 0));
    // printf("Result Struct field 1 is %llu\n", get_struct_field_u64(value2, 1));


    destroy_struct_constructor(&sc);
    destroy_vm(&vm);
    return 0;
}
