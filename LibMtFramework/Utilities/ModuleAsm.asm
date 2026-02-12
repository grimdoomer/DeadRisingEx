
PUBLIC ThisPtrCall
PUBLIC ThisPtrCallNoFixup
PUBLIC AssignVTableEntry

EXTERNDEF SnatcherModuleHandle:qword

.code

    ; __int64 __stdcall ThisPtrCall(void *functionPtr, void *thisPtr, void *arg1, void *arg2, void *arg3, void *arg4)
    ThisPtrCall PROC

        ; Setup the stack.
        push    r12
        push    r14
        sub		rsp, 28h

        ; Calculate the correct function address.
        mov		r12, rcx
        mov		r14, 140000000h
        sub		r12, r14
        mov		r14, SnatcherModuleHandle
        add		r12, r14

        ; Shift the arguments.
        mov		rcx, rdx
        mov		rdx, r8
        mov		r8, r9
        mov		r9, [rsp+38h]

        ; Call the function.
        call	r12

        ; Restore the stack.
        add		rsp, 28h
        pop		r14
        pop		r12
        ret

    ThisPtrCall ENDP

    ; __int64 __stdcall ThisPtrCallNoFixup(void *functionPtr, void *thisPtr, void *arg1, void *arg2, void *arg3, void *arg4)
    ThisPtrCallNoFixup PROC

        ; Setup the stack.
        push    r12
        push    r14
        sub		rsp, 28h

        ; Shift the arguments.
        mov		r12, rcx
        mov		rcx, rdx
        mov		rdx, r8
        mov		r8, r9
        mov		r9, [rsp+38h]

        ; Call the function.
        call    r12

        ; Restore the stack.
        add		rsp, 28h
        pop		r14
        pop		r12
        ret

    ThisPtrCallNoFixup ENDP

    ; void AssignVTableEntry(void** vtable, int index, void* pFunction)
	AssignVTableEntry PROC

		; Store function pointer into vtable entry.
		mov		qword ptr [rcx + rdx * 8], r8
		ret

	AssignVTableEntry ENDP

END