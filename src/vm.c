#include "vm.h"
#include <stdlib.h>
#include <stdio.h>

// this header is needed to include "alloca" for clang, for some reason?!?!
#include <malloc.h>

void init_struct_constructor(StructConstructor* sc) {
    sc->result = construct_empty_struct_type();
    sc->in_progress_of_creation = false;  
}

void start_creating_struct(StructConstructor* sc, bool packed) {
    sc->in_progress_of_creation = true;
    sc->is_packed = packed;
}

#define BASIC_FIELD_INITALIZE()                                                                 \
{                                                                                               \
    if (!sc->in_progress_of_creation) {                                                         \
        printf("Cannot add field to struct without first stating you want to create one\n");    \
        return;                                                                                 \
    }                                                                                           \
                                                                                                \
    if (sc->in_progress_of_creation && sc->result.elements == NULL) {                           \
        sc->result.size = 8;                                                                    \
        sc->result.elements = malloc(sizeof(StructElement) * 8);                                \
    }                                                                                           \
                                                                                                \
    if (sc->result.len >= sc->result.size) {                                                    \
        StructElement* new_mem = malloc(sizeof(StructElement) * sc->result.size * 2);           \
                                                                                                \
        for (int i = 0; i < sc->result.len; i++) {                                              \
            new_mem[i] = sc->result.elements[i];                                                \
        }                                                                                       \
                                                                                                \
        free(sc->result.elements);                                                              \
        sc->result.elements = new_mem;                                                          \
    }                                                                                           \
}

#define FIELD_ADD_BODY(TYPE, PADDING)                                                       \
{                                                                                           \
    sc->result.elements[sc->result.len].type = TYPE;                                        \
                                                                                            \
    if (!sc->is_packed)                                                                     \
        sc->result.struct_size += (PADDING - sc->result.struct_size % PADDING) % PADDING;   \
                                                                                            \
    if (sc->result.highest_alignment < PADDING)                                             \
        sc->result.highest_alignment = PADDING;                                             \
                                                                                            \
    sc->result.elements[sc->result.len].pos = sc->result.struct_size;                       \
                                                                                            \
    sc->result.len += 1;                                                                    \
    sc->result.struct_size += PADDING;                                                      \
}

#define FIELD_ADD_BODY_PLUS_INITALIZE(TYPE, PADDING)    \
{                                                       \
    BASIC_FIELD_INITALIZE();                            \
    FIELD_ADD_BODY(TYPE, PADDING);                      \
}   

void add_field_i8(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(I8, sizeof(int8_t)); 

void add_field_i16(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(I16, sizeof(int16_t));

void add_field_i32(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(I32, sizeof(int32_t));

void add_field_i64(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(I64, sizeof(int64_t));

void add_field_u8(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(U8, sizeof(uint8_t));

void add_field_u16(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(U16, sizeof(uint16_t));

void add_field_u32(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(U32, sizeof(uint32_t));

void add_field_u64(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(U64, sizeof(uint64_t));

void add_field_f32(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(F32, sizeof(float));

void add_field_f64(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(F64, sizeof(double));

void add_field_ptr(StructConstructor* sc) FIELD_ADD_BODY_PLUS_INITALIZE(PTR, sizeof(uintptr_t));

void add_field_struct(StructConstructor* sc, StructType* st) {
    BASIC_FIELD_INITALIZE();

    // So I have to handle aligning the contents of the structure and applying the current size of the structure that wants to be passed.
    // I could also provide a name to every struct type so that it can check that, but for now that isn't important.
    
    sc->result.elements[sc->result.len].type = STRUCT;                            
    
    if (!sc->is_packed)
        sc->result.struct_size += (st->highest_alignment - sc->result.struct_size % st->highest_alignment) % st->highest_alignment;   
                                                                               
    if (sc->result.highest_alignment < st->highest_alignment)
        sc->result.highest_alignment = st->highest_alignment;
                                                                                
    sc->result.elements[sc->result.len].pos = sc->result.struct_size;
                                                                                
    sc->result.len += 1;                                                        
    sc->result.struct_size += st->struct_size;                                          
}

StructType construct_struct_type(StructConstructor* sc) {
    if (!sc->in_progress_of_creation) {
        printf("Cannot create a structure without adding fields\n");
        exit(-1);
    }

    sc->in_progress_of_creation = false;
    StructType result = sc->result;

    printf("highest alignment is %u\n", result.highest_alignment);

    if (!sc->is_packed)
        result.struct_size += (result.highest_alignment - result.struct_size % result.highest_alignment) % result.highest_alignment;

    sc->result = construct_empty_struct_type();

    return result;
}

StructType construct_empty_struct_type() {
    return (StructType) {
        .elements = NULL,
        .len = 0,
        .size = 0,
        .struct_size = 0,
        .highest_alignment = 0,
    };
}

void push_field_i32(StructConstructor* sc, int32_t a) {

}

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

#define PASS_MACRO(type, val_type, type_cast)       \
{                                                   \
    vm->values[vm->len] = (Value) {                 \
        .as.val_type = (type_cast)a,                \
        .is = type,                                 \
    };                                              \
    vm->len++;                                      \
}

#define PASS_MACRO_I() { pass_ptr(vm, (void*)*((uintptr_t*)&a)); }

void pass_i8(VM* vm, int8_t a) PASS_MACRO_I();

void pass_i16(VM* vm, int16_t a) PASS_MACRO_I();

void pass_i32(VM* vm, int32_t a) PASS_MACRO_I();

void pass_i64(VM* vm, int64_t a) PASS_MACRO_I();

void pass_u8(VM* vm, uint8_t a) PASS_MACRO_I();

void pass_u16(VM* vm, uint16_t a) PASS_MACRO_I();

void pass_u32(VM* vm, uint32_t a) PASS_MACRO_I();

void pass_u64(VM* vm, uint64_t a) PASS_MACRO_I();

void pass_f32(VM* vm, float a) PASS_MACRO(F32, f32, float);

void pass_f64(VM* vm, double a) PASS_MACRO(F64, f64, double);

void pass_ptr(VM* vm, void* a) PASS_MACRO(PTR, ptr_address, uintptr_t);

void pass_str(VM* vm, const char* a) PASS_MACRO(PTR, ptr_address, uintptr_t);

// working on the x86 win32 version of the abi matching right now, then I will change it to work on linux as well.
DONT_OPTIMIZE
void* call_func_x86_64_win32(VM* vm, void* fptr) {
    #define WIN32_ASSIGN_VALUE(value, integer_register, float_register)                     \
        switch (value.is) {                                                                 \
            case PTR: {                                                                     \
                uintptr_t val = value.as.ptr_address;                                       \
                asm("mov %0, %%" integer_register : : "g"(val));                            \
                break;                                                                      \
            }                                                                               \
                                                                                            \
            case F64:                                                                       \
            case F32: {                                                                     \
                double val = value.as.f64;                                                  \
                asm("movq %0, %%" float_register : : "g"(val));                             \
                break;                                                                      \
            }                                                                               \
        }

    switch (vm->len) {
        default:
            if (vm->len == 0) break;
            {
                // total_memory = 32;
                int len = vm->len - 4;
                
                #define value vm->values[4+i]
                #define DEFAULT_X86_64_ALIGNMENT 8 

                //void* sptr = NULL;

                //gcc_alloca_impl(sptr, total_memory);

                volatile void* sptr = alloca(DEFAULT_X86_64_ALIGNMENT * len);                

                for (int i = 0; i < len; i++) { 
                    volatile uintptr_t* ptr = (volatile uintptr_t*)sptr;

                    *ptr = value.as.ptr_address;

                    sptr += DEFAULT_X86_64_ALIGNMENT;

                    // TODO: Might have to add a special switch for passing in struct data, but that's about it.
                }

                #undef DEFAULT_X86_64_ALIGNMENT
                #undef value
            }

        case 4:
            WIN32_ASSIGN_VALUE(vm->values[3], "r9", "xmm3");
        case 3:
            WIN32_ASSIGN_VALUE(vm->values[2], "r8", "xmm2");
        case 2:
            WIN32_ASSIGN_VALUE(vm->values[1], "rdx", "xmm1");
        case 1:
            WIN32_ASSIGN_VALUE(vm->values[0], "rcx", "xmm0");
            break;
    }
    
    void* r = NULL;
    r = ((void*(*)(void))fptr)();

    #undef WIN32_ASSIGN_VALUE

    return r;
}

// this function is just a wrapper around the call function so I can implement both sysv and win32 abi's at runtime.
void* call_func(VM* vm, void* fptr) {
    return call_func_x86_64_win32(vm, fptr);
}

#define RET_BODY_MACRO(type)                \
{                                           \
    void* val = call_func(vm, fptr);        \
                                            \
    return CAST_VOIDP_TO_TYPE(val, type);   \
}

int8_t call_func_i8(VM* vm, void* fptr) RET_BODY_MACRO(int8_t);

int16_t call_func_i16(VM* vm, void* fptr) RET_BODY_MACRO(int16_t); 

int32_t call_func_i32(VM* vm, void* fptr) RET_BODY_MACRO(int32_t);

int64_t call_func_i64(VM* vm, void* fptr) RET_BODY_MACRO(int64_t); 

uint8_t call_func_u8(VM* vm, void* fptr) RET_BODY_MACRO(uint8_t); 

uint16_t call_func_u16(VM* vm, void* fptr) RET_BODY_MACRO(uint16_t); 

uint32_t call_func_u32(VM* vm, void* fptr) RET_BODY_MACRO(uint32_t);

uint64_t call_func_u64(VM* vm, void* fptr) RET_BODY_MACRO(uint64_t); 

float call_func_f32(VM* vm, void* fptr) RET_BODY_MACRO(float); 

double call_func_f64(VM* vm, void* fptr) RET_BODY_MACRO(double); 

void* call_func_ptr(VM* vm, void* fptr) RET_BODY_MACRO(void*);

const char* call_func_str(VM* vm, void* fptr) RET_BODY_MACRO(const char*);
