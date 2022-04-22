;
; Demo Lib
;
; Datas file
;

section .data

global _module
_module incbin "datas/DB_S1_R.XM"
end_module:
global _module_size
_module_size dd end_module - _module

global _bitmap_wireframe
_bitmap_wireframe incbin "datas/wireframe.dds", 128

global _bitmap_sprite
_bitmap_sprite incbin "datas/sprite.dds", 128

global _bitmap_quantum
_bitmap_quantum incbin "datas/quantum.dds", 128

global _bitmap_font
_bitmap_font incbin "datas/font.dds", 128
