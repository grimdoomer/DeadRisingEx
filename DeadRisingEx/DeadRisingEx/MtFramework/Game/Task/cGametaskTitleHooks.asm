
PUBLIC Hook_cGametaskTitle_UnkUpdateFunc

EXTERNDEF SnatcherModuleHandle:qword

.code

	Hook_cGametaskTitle_UnkUpdateFunc PROC

		; Instructions we replaced:
		mov     eax, ecx				; this->pShedulerEdit->mAttr
		and     eax, 7					; this->pShedulerEdit->mAttr.Move
		dec     eax
		cmp     eax, 1
		ja      short return_to_code

		; Set the move operation to 3 which flags uSchedulerEdit instance for cleanup. We
		; must also set the pSchedulerEdit pointer to null to prevent the game from using
		; it in the cGametaskTitle destructor after it has already been free'd.
		and     ecx, 0FFFFFFFBh
		or      ecx, 3
		mov     [rdx+8], ecx			; this->pShedulerEdit->mAttr.Move = 3

		xor		rcx, rcx
		mov		[rbx+0E0h], rcx			; this->pShedulerEdit = nullptr

	return_to_code:

		; Jump back to the function after the instructions we replaced.
		mov		rax, 14020CFC0h
        mov		rcx, 140000000h
        sub		rax, rcx
        mov		rcx, SnatcherModuleHandle
        add		rax, rcx
		jmp		rax

	Hook_cGametaskTitle_UnkUpdateFunc ENDP

END