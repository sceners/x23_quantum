;
; demo_build
;
; Datas file
;

section .data

global _exefile
_exefile incbin "../demo_exe/Release/demo_exe.exe"
endexefile:
global _exefile_size
_exefile_size dd endexefile - _exefile
