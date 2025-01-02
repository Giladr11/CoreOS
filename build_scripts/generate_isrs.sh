#!/usr/bin/bash

ISRS_GEN_C=$1
ISRS_GEN_ASM=$2

ISRS_WITH_ERROR_CODE="8 10 11 12 13 14 17 21"

echo -e "#include \"include/arch/x86/idt/idt.h\"\n" > $ISRS_GEN_C
echo -e "#define GDT_CODE_SEGMENT 0x08\n" >> $ISRS_GEN_C

for i in $(seq 0 255); do
    echo "void __attribute((cdecl)) ISR${i}();" >> $ISRS_GEN_C
done

echo -e "\nvoid ISR_InitializeGates()
{" >> $ISRS_GEN_C

for i in $(seq 0 255); do
    echo "    IDT_SetGate(${i}, ISR${i}, GDT_CODE_SEGMENT, IDT_FLAG_RING0 | IDT_FLAG_GATE_32BIT_INT);" >> $ISRS_GEN_C
done

echo "}" >> $ISRS_GEN_C

#-----------------------------------------------------------------------------------------------------

echo "ISR_NOERROR_CODE 0" > $ISRS_GEN_ASM

for i in $(seq 1 255); do
    if echo "$ISRS_WITH_ERROR_CODE" | grep -q "\b${i}\b"; then
        echo "ISR_ERROR_CODE ${i}" >> $ISRS_GEN_ASM

    else
        echo "ISR_NOERROR_CODE ${i}" >> $ISRS_GEN_ASM
    fi
done