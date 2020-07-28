; proof of concept
format relocatable binary
virtual at 0
fixupz::
end virtual

	use16
mov ax,MyLabel
mov ax,[MyLabel]

	use32
mov eax,MyLabel
mov eax,[MyLabel]

	use64
mov rax,MyLabel
mov rax,[MyLabel]

MyLabel:

include 'display.inc' ; pretty

i = 0
iAddr = 0
while 1
	load vType byte from fixupz:i
	load fFlags byte from fixupz:i+1
	load iAddr dword from fixupz:i+2
	i = i + 6

	if iAddr = 0
	break
	end if

	if iAddr < MyLabel
	display \
	9,"value_type: ",<vType,16>,\
	9,"format_flags: ",<fFlags,16>,\
	9,"address: ",<iAddr,16>,\
	13,10
	else
		break
	end if
end while
