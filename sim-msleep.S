	.globl _msleep
	.type _msleep, @function
	.weak msleep
	.text
_msleep:
msleep:
	swi 0x101
	ret
	.size _msleep, . - _msleep
