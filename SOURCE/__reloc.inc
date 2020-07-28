
; minimal custom relocation output for binary format ; fasm.asm:332

__reloc_item_bytes = 6
__reloc_name.bytes = 6

; how to inject 'virtual' addressing space?

__reloc_setup: ; <- formats.inc:246
	cmp al,10h ; just binary for now
	jnz invalid_argument
	cmp [__reloc_label],0
	jnz .skip
	pushad
	mov esi,__reloc_name
	mov ecx,__reloc_name.bytes
	call find_label			; previously indexed by parser
	mov [__reloc_label],eax
	popad
.skip:	jmp select_format



; we need to bypass all the memory checks
__reloc_load_store_intercept: ; <- assemble.inc:649
	push ecx
	mov ecx,[__reloc_label]
	jecxz .skip
	mov ecx,[ecx]
	jecxz .skip
	cmp ecx,ebp ; is it the addressing space associated with our label?
	jz .reloc
.skip:	pop ecx
	retn

.reloc:	pop ecx
	pop edx				; bypass tests on private area
	cmp [esp],edx			; magic ;)
	jnc invalid_use_of_symbol	; store not allowed on "fixups:"

;	cmp [next_pass_needed],0
;	jne .bypass			; assume values don't matter
	cmp [number_of_relocations],0
	jz .bypass			; assume values don't matter

	mov ebx,[free_additional_memory]
	imul edx,[number_of_relocations],__reloc_item_bytes
	sub ebx,edx
	add ebx,eax
	movzx ecx,[operand_size]
	clc
	retn
.bypass:
	xor ecx,ecx
	clc
	retn


; constant and virtual types are filtered out prior
__reloc_mark_custom_relocation: ; <- formats.inc:419
	push eax ebx
	mov ebx,[free_additional_memory]
	add ebx,__reloc_item_bytes
	cmp ebx,[structures_buffer]
	jae out_of_memory
	mov [free_additional_memory],ebx
	inc [number_of_relocations]

	mov al,[value_type]
	mov ah,byte[format_flags]
	mov [ebx-__reloc_item_bytes],eax
	mov eax,edi
	sub eax,[code_start]
	mov [ebx-__reloc_item_bytes+2],eax
	pop ebx eax
	retn

; tables.inc:413
;__reloc_name: db 'fixups'
; tables.inc:443
; db 'relocatable',1Ch,80h ; so many bits ;)
; variable.inc:77
;__reloc_label dd 0
;__reloc_addressing_space dd 0 ; only set non-zero when custom relocations are enabled