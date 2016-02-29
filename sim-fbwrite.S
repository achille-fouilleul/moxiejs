	.globl _fbwrite
	.type _fbwrite, @function
	.weak fbwrite
	.text
_fbwrite:
fbwrite:
	swi 0x100
	ret
	.size _fbwrite, . - _fbwrite
