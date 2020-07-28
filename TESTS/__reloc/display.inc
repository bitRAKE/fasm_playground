; https://board.flatassembler.net/topic.php?p=440#440
macro __digit num {
	if num < 10
		display '0'+num
	else
		display 'A'+num-10
	end if }
macro __display arg1*,arg2 {
	if arg2 eq
		display arg1
	else
		local ..tmp
		..tmp = arg1
		virtual at 0
			while ..tmp > 0
				db ..tmp mod arg2
				..tmp = ..tmp / arg2
			end while
			repeat $
				load ..tmp byte from $-%
				__digit ..tmp
			end repeat
			if $ = 0
				display '0'
			end if
		end virtual
	end if }
macro display [arg] { __display arg }