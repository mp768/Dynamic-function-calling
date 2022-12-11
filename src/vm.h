#include <stdint.h>
#include <stdbool.h>

#ifndef VM_H
#define VM_H

typedef enum Type {
    I8,
    U8,
    I16,
    U16,
    I32,
    U32,
    I64,
    U64,
    F32,
    F64,
    PTR,
    STRUCT,
} Type;

typedef struct StructValue {
    uint8_t* bytes;
    struct StructType* type;
    // size_t size;
} StructValue;

typedef struct StructElement {
    Type type;
    uint32_t pos;
    struct StructType* struct_type;
} StructElement;

typedef struct StructType {
    StructElement* elements;
    int32_t len;
    int32_t size;
    StructValue** values;
    uint32_t value_len;
    uint32_t value_size;
    uint32_t struct_size;
    uint32_t highest_alignment;
} StructType;

// TODO: I might just make the constructor contain all the types it creates, so that I can remove the burden of manually freeing every single
// type created. I also might extend this to the types containg all the values they created and so on, I think this would be really useful 
// since I do not RAII or something else to automatically handle memory deallocation.
typedef struct StructConstructor {
    // this is the part of the constructor that constructs the type which it will return once the user is done.
    StructType result;
    bool in_progress_of_creation;
    bool is_packed;
    uint32_t types_generated_size;
    uint32_t types_generated_len;
    StructType** types_generated;
} StructConstructor;

typedef struct Value {
    union {
        //int8_t i8;
        //int32_t i32;
        float f32;
        double f64;
        uintptr_t ptr_address;
        StructValue* struct_data;
        //StructValue struct_value;
    } as;

    Type is;
} Value;

typedef struct VM {
    Value* values;
    int32_t len;
    int32_t size;
} VM;

void init_struct_constructor(StructConstructor* sc);

void destroy_struct_constructor(StructConstructor* sc);

void start_creating_struct_type(StructConstructor* sc, bool packed);

void add_field_i8(StructConstructor* sc);

void add_field_i16(StructConstructor* sc);

void add_field_i32(StructConstructor* sc);

void add_field_i64(StructConstructor* sc);

void add_field_u8(StructConstructor* sc);

void add_field_u16(StructConstructor* sc);

void add_field_u32(StructConstructor* sc);

void add_field_u64(StructConstructor* sc);

void add_field_f32(StructConstructor* sc);

void add_field_f64(StructConstructor* sc);

void add_field_ptr(StructConstructor* sc);

void add_field_struct(StructConstructor* sc, StructType* st);

StructType* construct_struct_type(StructConstructor* sc);

StructType construct_empty_struct_type();

StructValue* make_struct_value(StructType* type);

const StructValue make_empty_struct_value();

void set_struct_field(StructValue* value, uint32_t entry_idx, void* data);

void set_struct_field_i8(StructValue* value, uint32_t entry_idx, int8_t data);
void set_struct_field_i16(StructValue* value, uint32_t entry_idx, int16_t data);
void set_struct_field_i32(StructValue* value, uint32_t entry_idx, int32_t data);
void set_struct_field_i64(StructValue* value, uint32_t entry_idx, int64_t data);

void set_struct_field_u8(StructValue* value, uint32_t entry_idx, uint8_t data);
void set_struct_field_u16(StructValue* value, uint32_t entry_idx, uint16_t data);
void set_struct_field_u32(StructValue* value, uint32_t entry_idx, uint32_t data);
void set_struct_field_u64(StructValue* value, uint32_t entry_idx, uint64_t data);

void set_struct_field_f32(StructValue* value, uint32_t entry_idx, float data);
void set_struct_field_f64(StructValue* value, uint32_t entry_idx, double data);

void set_struct_field_struct(StructValue* value, uint32_t entry_idx, StructValue* data);

void* get_struct_field(StructValue* value, uint32_t entry_idx);

int8_t get_struct_field_i8(StructValue* value, uint32_t entry_idx);
int16_t get_struct_field_i16(StructValue* value, uint32_t entry_idx);
int32_t get_struct_field_i32(StructValue* value, uint32_t entry_idx);
int64_t get_struct_field_i64(StructValue* value, uint32_t entry_idx);

uint8_t get_struct_field_u8(StructValue* value, uint32_t entry_idx);
uint16_t get_struct_field_u16(StructValue* value, uint32_t entry_idx);
uint32_t get_struct_field_u32(StructValue* value, uint32_t entry_idx);
uint64_t get_struct_field_u64(StructValue* value, uint32_t entry_idx);

float get_struct_field_f32(StructValue* value, uint32_t entry_idx);
double get_struct_field_f64(StructValue* value, uint32_t entry_idx);

StructValue* get_struct_field_struct(StructValue* value, uint32_t entry_idx);

void init_vm(VM* vm);

void clear_vm(VM* vm);

void destroy_vm(VM* vm);

void pass_i8(VM* vm, int8_t a);

void pass_i16(VM* vm, int16_t a);

void pass_i32(VM* vm, int32_t a);

void pass_i64(VM* vm, int64_t a);

void pass_u8(VM* vm, uint8_t a);

void pass_u16(VM* vm, uint16_t a);

void pass_u32(VM* vm, uint32_t a);

void pass_u64(VM* vm, uint64_t a);

void pass_f32(VM* vm, float a);

void pass_f64(VM* vm, double a);

void pass_ptr(VM* vm, void* a);

void pass_str(VM* vm, const char* a); 

void pass_struct(VM* vm, StructValue* a);

#if defined(__clang__)
    #define DONT_OPTIMIZE __attribute__((optnone))
#elif defined(__GNUC__)
    #define DONT_OPTIMIZE __attribute__((optimize(0)))
#endif

// working on the x86 win32 version of the abi matching right now, then I will change it to work on linux as well.
DONT_OPTIMIZE
void* call_func_x86_64_win32(VM* vm, void* fptr); 

// this function is just a wrapper around the call function so I can implement both sysv and win32 abi's at runtime.
void* call_func(VM* vm, void* fptr); 

#define CAST_VOIDP_TO_TYPE(val, type) *((type*)&val)

void call_func_void(VM* vm, void* fptr);

int8_t call_func_i8(VM* vm, void* fptr);

int16_t call_func_i16(VM* vm, void* fptr);

int32_t call_func_i32(VM* vm, void* fptr);

int64_t call_func_i64(VM* vm, void* fptr);

uint8_t call_func_u8(VM* vm, void* fptr);

uint16_t call_func_u16(VM* vm, void* fptr);

uint32_t call_func_u32(VM* vm, void* fptr);

uint64_t call_func_u64(VM* vm, void* fptr);

float call_func_f32(VM* vm, void* fptr);

double call_func_f64(VM* vm, void* fptr);

void* call_func_ptr(VM* vm, void* fptr);

const char* call_func_str(VM* vm, void* fptr);

// do not use this function, it doesn't work :)
StructValue* call_func_struct(VM* vm, StructType* type, void* fptr);

#endif
