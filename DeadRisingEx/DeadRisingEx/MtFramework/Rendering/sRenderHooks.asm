
PUBLIC sRender_DrawFrame_Hook_Start
PUBLIC sRender_DrawFrame_Hook_End

sRender_CaptureFrame_Start PROTO
sRender_CaptureFrame_End PROTO

.code

	sRender_DrawFrame_Hook_Start PROC

		; Prolog:
		push	rcx
		push	rdx
		push	r8
		push	r9
		push	r10
		push	r11

		; Just call our high level hook.
		call	sRender_CaptureFrame_Start

		; Epilog:
		pop		r11
		pop		r10
		pop		r9
		pop		r8
		pop		rdx
		pop		rcx

		; The return value is the trampoline address.
		jmp		rax

	sRender_DrawFrame_Hook_Start ENDP

	sRender_DrawFrame_Hook_End PROC

		; Prolog:
		push	rcx
		push	rdx
		push	r8
		push	r9
		push	r10
		push	r11

		; Just call our high level hook.
		call	sRender_CaptureFrame_End

		; Epilog:
		pop		r11
		pop		r10
		pop		r9
		pop		r8
		pop		rdx
		pop		rcx

		; The return value is the trampoline address.
		jmp		rax

	sRender_DrawFrame_Hook_End ENDP

END