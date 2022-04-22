;		"Art" - a 64K demo by Farbrausch and Scoopex
;
;		RTLIB2.ASM - small math run time library replacement
;
;		Written by:
;   Ryg/FR, kb/FR - code

bits        32

section		.data

zero_point_five dd 0.5

section		.text

; ---- sF64 sin(sF64 a)

global      _sin@8
_sin@8:
	fld		qword [esp+4]
	fsin
	ret		8

; ---- sF64 cos(sF64 a)

global      _cos@8
_cos@8:
	fld		qword [esp+4]
	fcos
	ret		8

; ---- sF64 tan(sF64 a)

global		_tan@8
_tan@8:
	fld		qword [esp+4]
    fptan
	fstp    st0
    ret		8

; ---- sF64 asin(sF64 a)

global      _asin@8
_asin@8:
  fld   qword [esp+4]
	fld		st0
	fabs
	fcom	dword [zero_point_five]
	fstsw	ax
	sahf
	jbe		.shorter
	fld1
	fsubrp	st1, st0
	fld		st0
	fadd	st0
	fxch	st1
	fmul	st0
	fsubp	st1, st0
	jmp		short .end

.shorter:
	fstp	st0
	fld		st0
	fmul	st0
	fld1
	fsubrp	st1, st0
	
.end:
	fsqrt
	fpatan
	ret   8

; ---- sF64 atan2(sF64 y, sF64 x)

global      _atan2@16
_atan2@16:
	fld		qword [esp+4]
	fld		qword [esp+12]
	fpatan
	ret		16
    
; ---- sF64 sqrt(sF64 a)

global      _sqrt@8
_sqrt@8:
	fld		qword [esp+4]
	fsqrt
	ret		8

; ---- sF64 pow(sF64 a, sF64 b)

global      _pow@16
_pow@16:
	fld	    qword [esp+12]
	fld		qword [esp+4]
	ftst
	fstsw	ax
	sahf
	jz		.zero

	fyl2x

;	fld     st0
;	frndint
;	fsubr   st1, st0
;	fxch   st1
;	fchs
;	f2xm1
;	fld1
;	faddp   st1, st0
;	fscale

	fld1
	fld		st1
	fprem
	f2xm1
	faddp	st1, st0
	fscale

.zero:
	fstp	st1
	ret		16
    
; ---- float->int
global		__ftol
__ftol:
	sub		esp, byte 8
 	fistp	qword [esp]
 	pop		eax
 	pop		edx
 	ret

global __chkesp
__chkesp:
  ret
