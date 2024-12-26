option casemap:none


include constant_type.inc
include export_type.inc
include type.inc

.data
string_r db "r",0
string_error_open db "[-]Open File Fileds! ErrorCode:%d",0dh,0ah,0
string_error_ont_pe64 db "ERROR : This application is not 64-bit",0dh,0ah,0
string_meg_hint_test db "Open Failed",0dh,0ah,0
string_getctx_err db "[-]GetThreadContext Failed! ErrorCode:%d",0dh,0ah,0
string_read_write_err db "[-]ErrorCode:%d",0dh,0ah,0
string_create_process_dbg_info db "[+]CreateProcess:[BaseOfImage:%p][StartAddress:%p]",0dh,0ah,0
string_cmd_bp_error_hint db "[-]Repeat Breake Point",0dh,0ah,0
string_zydis_null db " ",0
string_zydis_address db "%016llX ",0
string_zydis_opcode db " %02X",0
string_zydis_disassmble db "%s",0dh,0ah,0
string_input_cmd_hint db ">>>",0
string_cmd_error_symbol db "未知命令!",0dh,0ah,0
string_cmd_slice_symbol db " ",0
string_cmd_slice2_symbol db "=",0
string_line_address db "%016llX ",0
string_line_feed db 0dh,0ah,0
string_line_db db " %02X",0
string_line_dd db " %08lX",0
string_line_dq db " %016llX",0
;===============================================================
;显示寄存器字符串

string_show_reg_0 db "eax=%08lX ebx=%08lX ecx=%08lX",0dh,0ah,0
string_show_reg_1 db "edx=%08lX esi=%08lX edi=%08lX",0dh,0ah,0
string_show_reg_2 db "eip=%08lX esp=%08lX ebp=%08lX",0dh,0ah,0
string_show_reg_3 db "DR0=%08lX DR1=%08lX DR2=%08lX",0dh,0ah,0
string_show_reg_4 db "DR3=%08lX DR6=%08lX DR7=%08lX",0dh,0ah,0
string_show_reg_5 db "efl=%08lX",0dh,0ah,0

string_edit_reg_0 db "eax",0
string_edit_reg_1 db "ebx",0
string_edit_reg_2 db "ecx",0
string_edit_reg_3 db "edx",0
string_edit_reg_4 db "esi",0
string_edit_reg_5 db "edi",0
string_edit_reg_6 db "eip",0
string_edit_reg_7 db "esp",0
string_edit_reg_8 db "ebp",0
string_edit_reg_9 db "r8",0
string_edit_reg_10 db "r9",0
string_edit_reg_11 db "r10",0
string_edit_reg_12 db "r11",0
string_edit_reg_13 db "r12",0
string_edit_reg_14 db "r13",0
string_edit_reg_15 db "r14",0
string_edit_reg_16 db "r15",0
;===============================================================
;命令字符串
string_cmd_t_symbol db  "t",0
string_cmd_g_symbol db  "g",0
string_cmd_u_symbol db  "u",0
string_cmd_r_symbol db  "r",0
string_cmd_bp_symbol db  "bp",0
string_cmd_bpm_symbol db  "bpm",0
string_cmd_bph_symbol db  "bph",0

string_cmd_db_symbol db  "db",0
string_cmd_dd_symbol db  "dd",0
string_cmd_dq_symbol db  "dq",0

string_cmd_eb_symbol db  "eb",0
string_cmd_ed_symbol db  "ed",0
string_cmd_eq_symbol db  "eq",0

string_cmd_p_symbol db  "p",0
string_cmd_pt_symbol db  "pt",0

string_cmd_ls_symbol db  "ls",0
string_cmd_ss_symbol db  "ss",0

string_cmd_lm_symbol db  "lm",0

string_cmd_dump_symbol db  "dump",0
;命令结构数组
cmd_map COMMAND 20 dup(<0>)

;===============================================================

g_restore_int3_bp_arrr_ptr qword 0

is_system_break db 0 
g_step_flag db 0

;===========
;恢复内存断点相关
g_restore_mem_bp_flag db 0
g_restore_mem_page_base qword 0
;内存断点相关
g_mem_bp_address_arr MemBreakPointInfo 5 dup (<>)
g_mem_bp_count dword 0

g_mem_bp_length dword 0
g_mem_bp_rwx_flag dword 0
g_mem_bp_old_rwx_flag dword 0
g_mem_bp_address qword 0
;===========
;硬件断点相关
g_hardware_int3_flag db 0
g_hardware_lg_flag byte 0
g_hardware_reg_index dword 0
;===========

;===========
;单步相关
g_jmp_call_flag db 0
g_jmp_call_address qword 0

;===========
;运行到返回相关
g_step_to_ret_int3_address qword 0
g_step_to_ret_int3_flag db 0
g_step_to_ret_step_flag db 0

;===========
;执行脚本相关
g_load_script_flag db 0
g_load_script_handle HANDLE 0

;===========
;记录指令相关
g_save_script_flag db 0
g_save_script_path db "save_script.txt",0
g_save_script_handle HANDLE 0

;===========
;dump PE 相关
g_dump_pe_path db "dumpPE.exe",0

;===========
;获取函数名称 相关
g_get_fun_name_symbol db "call [",0
g_get_fun_name_symbol2 db "call ",0
;===========
;显示模块信息相关
;string_lm_lib db "ntdll.dll",0
;string_lm_funname db "NtWow64QueryInformationProcess64",0

string_lm_str db "[ModuleBase]:%016llX [ModuleName]:%s",0dh,0ah,0

debug_event DEBUG_EVENT <0>

;软件断点相关
g_restore_int3_flag db 0
;break info
break_point_int3 BreakPointInfo 10 dup(<>)
break_point_int3_count dword 0

.code


;===============================================================
;初始化命令结构数组
;===============================================================
InitCMDMap PROC
  sub rsp,8
  ;g命令
  lea rax,string_cmd_g_symbol
  mov cmd_map[0].cmd,rax
  lea rax,CmdRun
  mov cmd_map[0].pfn,rax

  ;t命令
  lea rax,string_cmd_t_symbol
  mov cmd_map[10h].cmd,rax
  lea rax,CmdSingelStep
  mov cmd_map[10h].pfn,rax

  ;u命令
  lea rax,string_cmd_u_symbol
  mov cmd_map[20h].cmd,rax
  lea rax,CmdShowDisassembly
  mov cmd_map[20h].pfn,rax

  ;r命令
  lea rax,string_cmd_r_symbol
  mov cmd_map[30h].cmd,rax
  lea rax,CmdShowRegister
  mov cmd_map[30h].pfn,rax

  ;bp命令
  lea rax,string_cmd_bp_symbol
  mov cmd_map[40h].cmd,rax
  lea rax,CmdInt3BreakePoint
  mov cmd_map[40h].pfn,rax

  ;bpm命令
  lea rax,string_cmd_bpm_symbol
  mov cmd_map[50h].cmd,rax
  lea rax,CmdMemoryBreakePoint
  mov cmd_map[50h].pfn,rax

  ;bph命令
  lea rax,string_cmd_bph_symbol
  mov cmd_map[60h].cmd,rax
  lea rax,CmdHardwareBreakePoint
  mov cmd_map[60h].pfn,rax

  ;db命令
  lea rax,string_cmd_db_symbol
  mov cmd_map[70h].cmd,rax
  lea rax,CmdShowMemoryDB
  mov cmd_map[70h].pfn,rax

  ;dd命令
  lea rax,string_cmd_dd_symbol
  mov cmd_map[80h].cmd,rax
  lea rax,CmdShowMemoryDD
  mov cmd_map[80h].pfn,rax

  ;dq命令
  lea rax,string_cmd_dq_symbol
  mov cmd_map[90h].cmd,rax
  lea rax,CmdShowMemoryDQ
  mov cmd_map[90h].pfn,rax

  ;eb命令
  lea rax,string_cmd_eb_symbol
  mov cmd_map[0A0h].cmd,rax
  lea rax,CmdEditMemoryEB
  mov cmd_map[0A0h].pfn,rax

  ;ed命令
  lea rax,string_cmd_ed_symbol
  mov cmd_map[0B0h].cmd,rax
  lea rax,CmdEditMemoryED
  mov cmd_map[0B0h].pfn,rax

  ;eq命令
  lea rax,string_cmd_eq_symbol
  mov cmd_map[0C0h].cmd,rax
  lea rax,CmdEditMemoryEQ
  mov cmd_map[0C0h].pfn,rax

  ;p命令
  lea rax,string_cmd_p_symbol
  mov cmd_map[0D0h].cmd,rax
  lea rax,CmdSingleStepJmpCall
  mov cmd_map[0D0h].pfn,rax

  ;pt命令
  lea rax,string_cmd_pt_symbol
  mov cmd_map[0E0h].cmd,rax
  lea rax,CmdSteptoNextReturn
  mov cmd_map[0E0h].pfn,rax

  ;ls命令
  lea rax,string_cmd_ls_symbol
  mov cmd_map[0F0h].cmd,rax
  lea rax,CmdLoadScript
  mov cmd_map[0F0h].pfn,rax

  ;ss命令
  lea rax,string_cmd_ss_symbol
  mov cmd_map[100h].cmd,rax
  lea rax,CmdSaveScript
  mov cmd_map[100h].pfn,rax

  ;lm命令
  lea rax,string_cmd_lm_symbol
  mov cmd_map[110h].cmd,rax
  lea rax,CmdShowModuleList
  mov cmd_map[110h].pfn,rax

  ;dump命令
  lea rax,string_cmd_dump_symbol
  mov cmd_map[120h].cmd,rax
  lea rax,CmdDumpPE
  mov cmd_map[120h].pfn,rax

  add rsp,8
  ret
InitCMDMap ENDP

;===============================================================
;G命令封装
;===============================================================

CmdRun PROC
  sub rsp,8
  xor rax,rax
  add rsp,8
  ret
CmdRun ENDP

;===============================================================
;T命令封装
;===============================================================
CmdSingelStep PROC
  sub rsp,8
  mov g_step_flag,TRUE
  call SetSingelStep
  xor rax,rax 
  add rsp,8
  ret
CmdSingelStep ENDP

;===============================================================
;U命令封装
;CmdShowDisassembly(char* address_str)
;===============================================================

CmdShowDisassembly PROC
local address_ptr:ptr qword
local address:qword
local line_ptr:ptr qword
  sub rsp,58h
  ;if address ptr=null
  mov address_ptr,rcx
  ;mov line,rdx
  test rcx,rcx
  jne ADDRESS_IS_NOT_NULL
  call GetCurrentRIP
  test rax,rax
  je EXIT_CMDSHOWDISASSEMBLY
  mov rcx,rax
  jmp LINE_IS_NULL
ADDRESS_IS_NOT_NULL:
  lea r8,line_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,address_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDSHOWDISASSEMBLY
  mov r8,10h
  mov rdx,0
  mov rcx,rax
  call strtoull
  ;cmp rax,0
  ;jl EXIT_CMDSHOWDISASSEMBLY
  mov address,rax
  mov r8,line_ptr
  mov al,byte ptr[r8]
  test al,al
  mov rcx,address
  je LINE_IS_NULL
  mov r8,10
  mov rdx,0
  mov rcx,line_ptr
  call strtoull
  ;cmp rax,0
  ;jl EXIT_CMDSHOWDISASSEMBLY

  mov rdx,rax
  mov rcx,address
  jne FUN_DISASSEMBLYFORADDRESS
LINE_IS_NULL:
  mov rdx,20
FUN_DISASSEMBLYFORADDRESS:
  call DisassemblyForAddress
EXIT_CMDSHOWDISASSEMBLY:
  mov rax,TRUE 
  add rsp,58h
  ret
CmdShowDisassembly ENDP

;===============================================================
;r命令封装
;CmdShowRegister(char* str)
;===============================================================
CmdShowRegister PROC
local str_ptr:ptr qword
local next_str_ptr:ptr qword
  push rdi
  push rsi
  sub rsp,60h
  mov str_ptr,rcx
  
  test rcx,rcx
  jne EDIT_REGISTER
  ;str_ptr = 0 显示寄存器
  mov r8,FALSE
  mov rdx,0
  mov rcx,NULL
  call ShowOREditRegedit
  jmp EXIT_CMDSHOWREGISTER
EDIT_REGISTER:
  ;拆解字符串 rax=0f  str ="rax" value=0100040F
  lea r8,next_str_ptr
  lea rdx,string_cmd_slice2_symbol
  mov rcx,str_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDSHOWREGISTER
  mov rdi,rax

  mov rdx,next_str_ptr
  mov al,byte ptr [rdx]
  ;是\0结尾
  test al,al
  je EXIT_CMDSHOWREGISTER
  ;转换成数值
  mov r8,10h
  mov rcx,rdx
  mov rdx,0
  call strtoull
  ;test rax,rax
  ;jl EXIT_CMDSHOWREGISTER
  mov r8,TRUE
  mov rdx,rax
  mov rcx,rdi
  call ShowOREditRegedit
  ;修改寄存器
EXIT_CMDSHOWREGISTER:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdShowRegister ENDP
;===============================================================
;bp命令封装
;CmdShowDisassembly(char* address_str)
;===============================================================
CmdInt3BreakePoint PROC
local address_ptr:ptr qword
local address:qword
  push rdi
  push rsi
  sub rsp,60h
  mov address_ptr,rcx
  ;mov line,rdx
  test rcx,rcx
  je EXIT_CMDINT3BREAKEPOINT
  mov al,byte ptr[rcx]
  test al,al
  je EXIT_CMDINT3BREAKEPOINT
  mov r8,10h
  mov rdx,0
  mov rcx,address_ptr
  call strtoull
  ;cmp rax,0
  ;jl EXIT_CMDINT3BREAKEPOINT
  mov address,rax

  mov rax,sizeof break_point_int3
  cqo
  mov rdi,sizeof BreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_GET_BP_ARR:
  ;lea rax,[rcx-1]
  mov rax,rcx
  push rcx
  imul rax,rdi
  lea rdx,break_point_int3
  mov cl,byte ptr[rdx+rax+BreakPointInfo.using] 
  test cl,cl
  je GET_NEXT_BP_ARR
  mov rcx,qword ptr[rdx+rax+BreakPointInfo.break_address]
  cmp rcx,address
  jne GET_NEXT_BP_ARR
  add rsp,8
  jmp REPEAT_BREAKE_POINT
GET_NEXT_BP_ARR:
  pop rcx
loop LOOP_GET_BP_ARR
  mov rcx,address
  call SetInt3BreakPoint    
  jmp EXIT_CMDINT3BREAKEPOINT
REPEAT_BREAKE_POINT:
  lea rcx,string_cmd_bp_error_hint
  call printf
EXIT_CMDINT3BREAKEPOINT:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdInt3BreakePoint ENDP

;===============================================================
;bpm 命令封装
;CmdMemoryBreakePoint(char* str)
;===============================================================
CmdMemoryBreakePoint PROC
local str_ptr:ptr qword
local address:qword
local next_str_ptr:ptr qword
local bp_length:dword
local bp_type:dword
  push rdi
  push rsi
  sub rsp,60h
  je EXIT_CMDMEMORYBREAKEPOINT
  mov str_ptr,rcx
  ;拆解字符串 获取地址
  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,str_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDMEMORYBREAKEPOINT
  mov r8,10h
  mov rdx,0
  mov rcx,rax
  call strtoull
  ;cmp rax,0
  ;jl EXIT_CMDMEMORYBREAKEPOINT
  mov address,rax
  ;获取设置长度
  mov rax,next_str_ptr
  mov str_ptr,rax

  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,str_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDMEMORYBREAKEPOINT
  mov r8,10h
  mov rdx,0
  mov rcx,rax
  call strtoull
  ;cmp rax,0
  ;jl EXIT_CMDMEMORYBREAKEPOINT
  mov bp_length,eax
  ;获取类型 RWX
  xor eax,eax
  mov rdx,next_str_ptr
  mov al,byte ptr [rdx]
  test al,al
  je EXIT_CMDMEMORYBREAKEPOINT
  mov bp_type,eax
  ;解析类型
  cmp bp_type,"r"
  je IS_READ_TYPE
  cmp bp_type,"w"
  je IS_WRITE_TYPE
  jmp EXIT_CMDMEMORYBREAKEPOINT

IS_READ_TYPE:
  mov bp_type,0
  jmp SET_MEM_BP
IS_WRITE_TYPE:
  mov bp_type,1

SET_MEM_BP:
  xor r8,r8
  mov r8d,bp_type
  xor rdx,rdx
  mov edx,bp_length
  mov rcx,address
  call SetMemoryBreakePoint 

EXIT_CMDMEMORYBREAKEPOINT:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdMemoryBreakePoint ENDP

;===============================================================
;bph 命令封装
;CmdHardwareBreakePoint(char* str)
;===============================================================

CmdHardwareBreakePoint PROC
local str_ptr:ptr qword
local address:qword
local next_str_ptr:ptr qword
local bp_length:dword
local bp_type:dword
  push rdi
  push rsi
  sub rsp,60h
  test rcx,rcx
  je EXIT_CMDHARDWAREBREAKEPOINT
  mov str_ptr,rcx
  ;拆解字符串 获取地址
  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,str_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDHARDWAREBREAKEPOINT
  mov r8,10h
  mov rdx,0
  mov rcx,rax
  call strtoull
  ;cmp rax,0
  ;jl EXIT_CMDHARDWAREBREAKEPOINT
  mov address,rax
  ;获取设置长度
  mov rax,next_str_ptr
  mov str_ptr,rax

  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,str_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDHARDWAREBREAKEPOINT
  mov r8,10h
  mov rdx,0
  mov rcx,rax
  call strtoull
  ;test rax,rax
  ;je EXIT_CMDHARDWAREBREAKEPOINT
  mov bp_length,eax
  ;获取类型 RWX
  xor eax,eax
  mov rdx,next_str_ptr
  mov al,byte ptr [rdx]
  test al,al
  je EXIT_CMDHARDWAREBREAKEPOINT
  mov bp_type,eax

  xor r8,r8
  mov r8d,bp_type
  xor rdx,rdx
  mov edx,bp_length
  mov rcx,address
  call SetHardwareBreakePoint 


EXIT_CMDHARDWAREBREAKEPOINT:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdHardwareBreakePoint ENDP

;===============================================================
;DB 命令封装
;CmdShowMemoryDB(char* str)
;===============================================================

CmdShowMemoryDB PROC
local address:qword
  push rdi
  push rsi
  sub rsp,58h
  mov address,0
  ;mov address,rcx
  mov r8,10h
  mov rdx,0
  call strtoull
  mov address,rax
  mov rdx,1
  mov rcx,address
  call ShowDebugProcessMemory
EXIT_CMDSHOWMEMORYDB:
  mov rax,TRUE
  add rsp,58h
  pop rsi
  pop rdi
  ret
CmdShowMemoryDB ENDP

;===============================================================
;DD 命令封装
;CmdShowMemoryDD(char* str)
;===============================================================

CmdShowMemoryDD PROC
local address:qword
  push rdi
  push rsi
  sub rsp,58h
  mov r8,10h
  mov rdx,0
  call strtoull
  mov address,rax
  mov rdx,4
  mov rcx,address
  call ShowDebugProcessMemory
EXIT_CMDSHOWMEMORYDD:
  mov rax,TRUE
  add rsp,58h
  pop rsi
  pop rdi
  ret
CmdShowMemoryDD ENDP

;===============================================================
;DQ 命令封装
;CmdShowMemoryDQ(char* str)
;===============================================================

CmdShowMemoryDQ PROC
local address:qword
  push rdi
  push rsi
  sub rsp,58h
  mov r8,10h
  mov rdx,0
  call strtoull
  mov address,rax
  mov rdx,8
  mov rcx,address
  call ShowDebugProcessMemory
EXIT_CMDSHOWMEMORYDQ:
  mov rax,TRUE
  add rsp,58h
  pop rsi
  pop rdi
  ret
CmdShowMemoryDQ ENDP

;===============================================================
;EB 命令封装
;CmdEditMemoryEB(char* str)
;===============================================================

CmdEditMemoryEB PROC
local address_ptr:ptr qword
local next_str_ptr:ptr qword
local address:qword
local value:qword
  push rdi
  push rsi
  sub rsp,60h
  mov address_ptr,rcx
  test rcx,rcx
  je EXIT_CMDEDITMEMORYEB

  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,address_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDEDITMEMORYEB

  ;判断第3个参数是否存在
  mov r8,next_str_ptr
  mov al,byte ptr[r8]
  test al,al
  je EXIT_CMDEDITMEMORYEB

  mov r8,10h
  mov rdx,0
  mov rcx,address_ptr
  call strtoull
  mov address,rax

  mov r8,10h
  mov rdx,0
  mov rcx,next_str_ptr
  call strtoull
  and rax,0FFh
  mov value,rax

  mov r8,value
  mov rdx,1
  mov rcx,address
  call WriteDataToMemory
EXIT_CMDEDITMEMORYEB:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdEditMemoryEB ENDP

;===============================================================
;ED 命令封装
;CmdEditMemoryED(char* str)
;===============================================================

CmdEditMemoryED PROC
local address_ptr:ptr qword
local next_str_ptr:ptr qword
local address:qword
local value:qword
  push rdi
  push rsi
  sub rsp,60h
  mov address_ptr,rcx
  test rcx,rcx
  je EXIT_CMDEDITMEMORYED

  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,address_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDEDITMEMORYED

  ;判断第3个参数是否存在
  mov r8,next_str_ptr
  mov al,byte ptr[r8]
  test al,al
  je EXIT_CMDEDITMEMORYED

  mov r8,10h
  mov rdx,0
  mov rcx,address_ptr
  call strtoull
  mov address,rax

  mov r8,10h
  mov rdx,0
  mov rcx,next_str_ptr
  call strtoull
  and rax,0FFFFFFFFh
  mov value,rax

  mov r8,value
  mov rdx,4
  mov rcx,address
  call WriteDataToMemory
EXIT_CMDEDITMEMORYED:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdEditMemoryED ENDP

;===============================================================
;ED 命令封装
;CmdEditMemoryEQ(char* str)
;===============================================================

CmdEditMemoryEQ PROC
local address_ptr:ptr qword
local next_str_ptr:ptr qword
local address:qword
local value:qword
  push rdi
  push rsi
  sub rsp,60h
  mov address_ptr,rcx
  test rcx,rcx
  je EXIT_CMDEDITMEMORYEQ

  lea r8,next_str_ptr
  lea rdx,string_cmd_slice_symbol
  mov rcx,address_ptr
  call strtok_s
  test rax,rax
  je EXIT_CMDEDITMEMORYEQ

  ;判断第3个参数是否存在
  mov r8,next_str_ptr
  mov al,byte ptr[r8]
  test al,al
  je EXIT_CMDEDITMEMORYEQ

  mov r8,10h
  mov rdx,0
  mov rcx,address_ptr
  call strtoull
  mov address,rax

  mov r8,10h
  mov rdx,0
  mov rcx,next_str_ptr
  call strtoull
  and rax,0FFFFFFFFFFFFFFFFh
  mov value,rax

  mov r8,value
  mov rdx,8
  mov rcx,address
  call WriteDataToMemory
EXIT_CMDEDITMEMORYEQ:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
CmdEditMemoryEQ ENDP

;===============================================================
;p 命令封装
;CmdEditMemoryEQ()
;===============================================================

CmdSingleStepJmpCall PROC
  sub rsp,8h
  call SingleStepJmpCall
  xor rax,rax
  add rsp,8h
  ret 
CmdSingleStepJmpCall ENDP

;===============================================================
;pt 命令封装
;CmdSteptoNextReturn()
;===============================================================

CmdSteptoNextReturn PROC
  sub rsp,8h
  ;call 
  call SteptoNextReturn
  xor rax,rax
  add rsp,8h
  ret 
CmdSteptoNextReturn ENDP

;===============================================================
;ls 命令封装
;CmdLoadScript(char* script_path)
;===============================================================
CmdLoadScript PROC
local script_path:ptr qword
  push rdi
  push rsi
  sub rsp,58h
  mov script_path,0
  mov script_path,rcx
  test rcx,rax
  je EXIT_CMDLOADSCRIPT
  ;获取文件句柄
  mov rdx,OPEN_EXISTING
  mov rcx,script_path
  call CreateFileGetHandle
  test rax,rax
  je EXIT_CMDLOADSCRIPT
  mov g_load_script_handle,rax
  ;设置脚本运行标记
  mov g_load_script_flag,TRUE
EXIT_CMDLOADSCRIPT:
  mov rax,TRUE
  add rsp,58h
  pop rsi
  pop rdi
  ret
CmdLoadScript ENDP

;===============================================================
;ss 命令封装
;CmdSaveScript()
;===============================================================
CmdSaveScript PROC
  sub rsp,8h
  cmp g_save_script_flag,TRUE
  jne ON_FLAG
  cmp g_save_script_handle,0
  je EXITM_CMDSAVESCRIPT
  mov rcx,g_save_script_handle
  call CloseHandle
  mov g_save_script_flag,FALSE
  mov g_save_script_handle,0
  jmp EXITM_CMDSAVESCRIPT
ON_FLAG:
  mov rdx,CREATE_ALWAYS
  lea rcx,g_save_script_path
  call CreateFileGetHandle
  cmp rax,INVALID_HANDLE_VALUE
  je EXITM_CMDSAVESCRIPT
  mov g_save_script_handle,rax
  mov g_save_script_flag,TRUE
EXITM_CMDSAVESCRIPT:
  mov rax,TRUE
  add rsp,8h
  ret 
CmdSaveScript ENDP

;===============================================================
;lm 命令封装
;CmdShowModuleList()
;===============================================================

CmdShowModuleList PROC 
  sub rsp,8h
  ;call 
  call ShowDebugProcessModuleList
  mov rax,TRUE
  add rsp,8h
  ret 
CmdShowModuleList ENDP

;===============================================================
;dump 命令封装
;CmdDumpPE()
;===============================================================

CmdDumpPE PROC 
  sub rsp,8h
  ;call 
  call DumpPE
  mov rax,TRUE
  add rsp,8h
  ret 
CmdDumpPE ENDP

;===============================================================
;判断PE标记位
;===============================================================

;判断是不是64位程序
;ret 不是64位rax = 0 否则 rax = 1
IsPE64 PROC
local file_handle:FILE 
local image_dos:IMAGE_DOS_HEADER
local image_nt:IMAGE_NT_HEADERS64
local bool_ret:BOOL
  sub rsp,58h
  mov file_handle,0
  mov bool_ret,0
  lea r8,string_r
  mov rdx,rcx ;path
  lea rcx,file_handle
  call fopen_s
  test rax,rax
  jne PRINT_ERROR ;if ret != 0 ret
  mov r8,SEEK_SET
  mov rdx,0
  mov rcx,file_handle
  call fseek
  mov r9,file_handle
  mov r8,sizeof IMAGE_DOS_HEADER
  mov rdx,1
  lea rcx,image_dos
  call fread
  mov r8,SEEK_SET
  xor rdx,rdx
  mov edx,image_dos.e_lfanew
  mov rcx,file_handle
  call fseek
  mov r9,file_handle
  mov r8,sizeof IMAGE_NT_HEADERS64
  mov rdx,1
  lea rcx,image_nt
  call fread
  xor rax,rax
  movzx eax,image_nt.FileHeader.Machine
  xor eax,IMAGE_FILE_MACHINE_I386
  je NotPE64
  mov rax,1;
  jmp EXIT_IsPE64

NotPE64:
  mov r9,MB_OK
  lea r8,string_meg_hint_test
  lea rdx,string_error_ont_pe64
  mov rcx,NULL
  call MessageBoxA
  mov rax,0;
  jmp EXIT_IsPE64

PRINT_ERROR:
  mov rdx,rax
  lea rcx,string_error_open
  call printf
  mov rax,0
  add rsp,8
  ret

EXIT_IsPE64:
  mov rcx,file_handle
  call fclose
  add rsp,58h
  ret
IsPE64 ENDP

;===============================================================
;调试事件分发
;===============================================================

;调试事件分发
DistributeDebugEvent PROC
local loop_flag:dword
local continue_status:dword
;local continue_status1:qword
  sub rsp,58h
  mov continue_status,DBG_CONTINUE
  mov loop_flag,1
LOOP_GET_EVENT:
  ;mov r8,sizeof DEBUG_EVENT
  
  mov rdx,INFINITE
  lea rcx,debug_event
  call WaitForDebugEvent
  cmp debug_event.dwDebugEventCode,EXCEPTION_DEBUG_EVENT
  jne CREATE_THREAD_EVENT
  ;EXCEPTION_DEBUG_EVENT 调试事件
  call DebugEventException
  mov continue_status,eax
  jmp NEXT_DEBUG_EVENT  ;break
CREATE_THREAD_EVENT:
  cmp debug_event.dwDebugEventCode,CREATE_THREAD_DEBUG_EVENT
  jne CREATE_PROCESS_EVENT
  ;CREATE_THREAD_DEBUG_EVENT 创建线程事件
  jmp NEXT_DEBUG_EVENT  ;break
CREATE_PROCESS_EVENT:
  cmp debug_event.dwDebugEventCode,CREATE_PROCESS_DEBUG_EVENT
  jne EXIT_THREAD_EVENT
  ;CREATE_PROCESS_DEBUG_EVENT 创建进程事件
  call CreateProcessEventManage
  mov continue_status,eax
  jmp NEXT_DEBUG_EVENT  ;break
EXIT_THREAD_EVENT:
  cmp debug_event.dwDebugEventCode,EXIT_THREAD_DEBUG_EVENT
  jne EXIT_PROCESS_EVENT
  ;EXIT_THREAD_DEBUG_EVENT 退出线程事件
  jmp NEXT_DEBUG_EVENT  ;break
EXIT_PROCESS_EVENT:
  cmp debug_event.dwDebugEventCode,EXIT_PROCESS_DEBUG_EVENT
  jne LOAD_DLL_EVENT
  ;EXIT_PROCESS_DEBUG_EVENT 退出进程事件
  jmp NEXT_DEBUG_EVENT  ;break
LOAD_DLL_EVENT:
  cmp debug_event.dwDebugEventCode,LOAD_DLL_DEBUG_EVENT
  jne UNLOAD_DLL_EVENT
  ;LOAD_DLL_DEBUG_EVENT 加载模块事件
  jmp NEXT_DEBUG_EVENT  ;break
UNLOAD_DLL_EVENT:
  cmp debug_event.dwDebugEventCode,UNLOAD_DLL_DEBUG_EVENT
  jne OUTPUT_STRING_EVENT
  ;UNLOAD_DLL_DEBUG_EVENT 卸载模块事件
  jmp NEXT_DEBUG_EVENT  ;break
OUTPUT_STRING_EVENT:
  cmp debug_event.dwDebugEventCode,OUTPUT_DEBUG_STRING_EVENT
  jne RIP_DEBUG_EVENT
  ;OUTPUT_DEBUG_STRING_EVENT 输出调试字符串事件
  jmp NEXT_DEBUG_EVENT  ;break
RIP_DEBUG_EVENT:
  cmp debug_event.dwDebugEventCode,RIP_EVENT
  jne NEXT_DEBUG_EVENT
  ;RIP_EVENT 控制事件
NEXT_DEBUG_EVENT:
  xor r8,r8 
  mov r8d,continue_status
  xor rdx,rdx
  mov edx,debug_event.dwThreadId
  xor rcx,rcx
  mov ecx,debug_event.dwProcessId
  call ContinueDebugEvent
  xor eax,0
  jne NEXT_LOOP
  mov loop_flag,0

NEXT_LOOP:
  cmp loop_flag,1
  je LOOP_GET_EVENT

EXITM_DISTRIBUTEDEBUGEVENT:
  add rsp,58h
  ret
DistributeDebugEvent ENDP

;===============================================================
;异常调试事件分发
;===============================================================

;异常调试事件分发
DebugEventException PROC
 local  exception_debug_info:ptr EXCEPTION_DEBUG_INFO
 local  exception_record:ptr EXCEPTION_RECORD
 local  status:dword 
  sub rsp,58h;栈空间
  mov status,DBG_CONTINUE
  lea rax,debug_event.u.Exception
  mov exception_debug_info,rax
  lea rax,[rax + EXCEPTION_DEBUG_INFO.ExceptionRecord]
  mov exception_record,rax

  ;lpExceptionRecord->ExceptionCode
  mov rax,exception_record
  mov eax,[rax + EXCEPTION_RECORD.ExceptionCode]
  ;switch
  cmp eax,EXCEPTION_ACCESS_VIOLATION
  jne EXCEPTION_BREAKPOINT_@
  ;EXCEPTION_ACCESS_VIOLATION 内存访问异常
  call ExceptionMemoryEvent
  mov status,eax
  jmp EXIT_DEBUGEVENTEXCEPTION  ;break
EXCEPTION_BREAKPOINT_@:
  cmp eax,STATUS_WX86_BREAKPOINT
  jne EXCEPTION_SINGLE_STEP_@
  ;EXCEPTION_BREAKPOINT 断点异常
  call ExceptionBreakPoint
  mov status,eax
  jmp EXIT_DEBUGEVENTEXCEPTION  ;break
EXCEPTION_SINGLE_STEP_@:
  cmp eax,STATUS_WX86_SINGLE_STEP
  jne DBG_CONTROL_C_@
  ;EXCEPTION_SINGLE_STEP 单步异常
  call ExceptionSingelStep
  mov status,eax
  jmp EXIT_DEBUGEVENTEXCEPTION  ;break
DBG_CONTROL_C_@:
  cmp eax,DBG_CONTROL_C
  jne EXIT_DEBUGEVENTEXCEPTION
  ;DBG_CONTROL_C 控制台退出
EXIT_DEBUGEVENTEXCEPTION:
  mov eax,status
  add rsp,58h
  ret
DebugEventException ENDP

;===============================================================
;创建进程事件处理函数
;===============================================================

;创建进程事件处理
CreateProcessEventManage PROC
local create_process_info:ptr CREATE_PROCESS_DEBUG_INFO

  sub rsp,58h
  ;LPCREATE_PROCESS_DEBUG_INFO lpCreateProcessInfo = &DebugEvent.u.CreateProcessInfo;
  lea rax,debug_event.u.CreateProcessInfo
  mov create_process_info,rax
  ;printf_s("[+]CreateProcess:[BaseOfImage:%p][StartAddress:%p]\r\n", lpCreateProcessInfo->lpBaseOfImage, lpCreateProcessInfo->lpStartAddress);
  mov r8,qword ptr [rax + CREATE_PROCESS_DEBUG_INFO.lpStartAddress]
  mov rdx,qword ptr [rax + CREATE_PROCESS_DEBUG_INFO.lpBaseOfImage]
  lea rcx,string_create_process_dbg_info
  call printf
  ;入口点下断点
  mov rax,create_process_info
  mov rcx,qword ptr [rax + CREATE_PROCESS_DEBUG_INFO.lpStartAddress]
  call SetInt3BreakPoint


EXIT_CREATEPROCESSEVENTMANAGE:
  xor rax,DBG_CONTINUE 
  add rsp,58h
  ret
CreateProcessEventManage ENDP

;===============================================================
;断点设置
;===============================================================

;设置Int3断点函数
;SetInt3BreakPoint(VOID* pAddress)
;ret bool
SetInt3BreakPoint PROC
;local break_point:BreakPointInfo
local breake_address:qword
local process_handle:HANDLE
local write_length:QWORD
local bool_ret:dword
;local breake_point_ptr:ptr qword
local int3_byte:byte
local read_byte:byte
  push  rdi
  push  rsi
  sub rsp,60h
  mov bool_ret,0
  mov breake_address,rcx
  ;BreakPointInfo BP = { 0 };
  ;lea rax,break_point
  ;mov rdi,rax  
  ;xor eax,eax  
  ;mov ecx,sizeof BreakPointInfo 
  ;rep stos byte ptr [rdi]
  ;UCHAR szInt3 = 0xCC;
  mov int3_byte,0cch
  mov process_handle,NULL


  ;if break_point_count >= 10
  cmp break_point_int3_count,10
  jge EXIT_SETINT3BREAKPOINT

  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  ;hProcess == NULL
  test rax,rax
  je EXIT_SETINT3BREAKPOINT
  mov process_handle,rax



  ;lea rax,[rdx+r8*r9]
  ;lea rax,break_point[1]
  ;jmp CLOSE_HANDLE_@1
  ;mov rcx,r9
  ;ReadProcessMemory(hProcess, pAddress, &BP.szOldCode, sizeof(BP.szOldCode),NULL);
  sub rsp,8
  push NULL
  sub rsp,20h
  mov r9,1
  lea r8,read_byte;byte ptr [rcx + BreakPointInfo.old_code]
  mov rdx,breake_address
  mov rcx,process_handle
  ;mov byte ptr[rdx],1
  call ReadProcessMemory
  add rsp,30h
  ;if ret = 0 函数失败，则返回值为 0
  xor eax,0
  je CLOSE_HANDLE_@1

  ;WriteProcessMemory(hProcess, pAddress, &szInt3, sizeof(szInt3), &stWriteLength);
  
  mov write_length,0
  lea rax,write_length
  sub rsp,8
  push rax
  sub rsp,20h
  mov r9,1
  lea r8, int3_byte
  mov rdx,breake_address
  mov rcx,process_handle
  call WriteProcessMemory
  add rsp,30h
  ;if ret = 0 函数失败，则返回值为 0
  xor eax,0
  je CLOSE_HANDLE_@1

  ;sizeof(g_Commands) / sizeof(g_Commands[0]
  mov rax,sizeof break_point_int3
  cqo
  mov rdi,sizeof BreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_GET_BP_ARR:
  ;lea rax,[rcx-1]
  mov rax,rcx
  push rcx
  imul rax,rdi
  lea rdx,break_point_int3
  mov cl,byte ptr[rdx+rax+BreakPointInfo.using] 
  test cl,cl
  jne GET_NEXT_BP_ARR
  mov byte ptr[rdx+rax+BreakPointInfo.using],1
  mov byte ptr[rdx+rax+BreakPointInfo.efficacious],1
  mov cl,read_byte
  mov byte ptr[rdx+rax+BreakPointInfo.old_code],cl
  mov rcx,breake_address
  mov qword ptr[rdx+rax+BreakPointInfo.break_address],rcx
  add break_point_int3_count,1
  add rsp,8
  jmp CLOSE_HANDLE_@1
GET_NEXT_BP_ARR:
  pop rcx 
loop LOOP_GET_BP_ARR


  ;arr[index].address= breake_address
  ;movsxd r8,dword ptr [break_point_int3_count]
  ;imul rax,r8
  ;lea rdx,break_point_int3
  ;;mov r9,breake_address
  ;mov rcx,breake_address
  ;;mov qword ptr [rdx+rax],rcx
  ;lea r9,qword ptr [rdx+rax]
  ;mov breake_point_ptr,r9
  ;mov qword ptr [r9+BreakPointInfo.break_address],rcx

  ;++break_point_int3
 

CLOSE_HANDLE_@1:
  mov rcx,process_handle
  call CloseHandle


EXIT_SETINT3BREAKPOINT:
  xor rax,rax
  mov eax,bool_ret
  add rsp,60h
  pop rsi
  pop rdi
  ret
SetInt3BreakPoint ENDP

;===============================================================
;断点删除
;===============================================================
;删除Int3断点函数
;DelInt3BreakPoint(VOID* pAddress)
;ret bool
DelInt3BreakPoint PROC
local break_address:qword
local old_code:byte
  push  rdi
  push  rsi
  sub rsp,60h
  mov break_address,rcx
  ;遍历
  ;sizeof(g_Commands) / sizeof(g_Commands[0]
  mov rax,sizeof break_point_int3
  cqo
  mov rdi,sizeof BreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_GET_BP_ARR:
  ;lea rax,[rcx-1]
  mov rax,rcx
  imul rax,rdi
  lea rdx,break_point_int3
  mov r9,qword ptr[rdx+rax+BreakPointInfo.break_address] 
  cmp r9,break_address
  jne GET_NEXT_BP_ARR
  ;是否在使用中
  mov r9b,byte ptr[rdx+rax+BreakPointInfo.efficacious] 
  test r9b,r9b
  je NOT_USING; efficacious = 0
  ;还原软件断点位置
  mov r8,1
  mov dl,byte ptr[rdx+rax+BreakPointInfo.old_code] 
  mov old_code,dl
  lea rdx,old_code
  mov rcx,qword ptr[rdx+rax+BreakPointInfo.break_address] 
  call WriteDebugProcessMemory
  test rax,rax
  je EXIT_DELINT3BREAKPOINT

  ;不在使用中
NOT_USING:
  mov byte ptr[rdx+rax+BreakPointInfo.using],0
  mov byte ptr[rdx+rax+BreakPointInfo.efficacious],0
  mov byte ptr[rdx+rax+BreakPointInfo.old_code],0
  mov qword ptr[rdx+rax+BreakPointInfo.break_address],0
  dec break_point_int3_count
  jmp EXIT_DELINT3BREAKPOINT
GET_NEXT_BP_ARR:
loop LOOP_GET_BP_ARR

EXIT_DELINT3BREAKPOINT:
  add rsp,60h
  pop rsi
  pop rdi
  ret
DelInt3BreakPoint ENDP
;===============================================================
;还原软件
;RestoreInt3BreakPoint(BreakPointInfo* bpi)
;===============================================================
RestoreInt3BreakPoint PROC
local process_handle:HANDLE
local break_point_ptr:ptr qword
local write_length:qword
local int3_byte:byte

  push  rdi
  push  rsi
  sub rsp,60h
  mov process_handle,0
  mov break_point_ptr,0
  mov int3_byte,0cch
  test rcx,rcx
  je EXIT_RESTOREINT3BREAKPOINT
  mov al,byte ptr [rcx + BreakPointInfo.efficacious]
  test al,al
  jne EXIT_RESTOREINT3BREAKPOINT
  mov break_point_ptr,rcx
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  ;hProcess == NULL
  test rax,rax
  je EXIT_RESTOREINT3BREAKPOINT
  mov process_handle,rax
  ;ReadProcessMemory
  sub rsp,8
  push NULL
  sub rsp,20h
  mov r9,1
  mov rax,break_point_ptr
  lea r8,[rax+BreakPointInfo.old_code]
  mov rdx,qword ptr [rax + BreakPointInfo.break_address]
  mov rcx,process_handle
  ;mov byte ptr[rdx],1
  call ReadProcessMemory
  add rsp,30h
  ;if ret = 0 函数失败，则返回值为 0
  xor eax,0
  je CLOSE_HANDLE@31
  ;WriteProcessMemory(hProcess, pAddress, &szInt3, sizeof(szInt3), &stWriteLength);
  mov write_length,0
  lea rax,write_length
  sub rsp,8
  push rax
  sub rsp,20h
  mov r9,1
  lea r8,int3_byte
  mov rax,break_point_ptr
  mov rdx,qword ptr [rax + BreakPointInfo.break_address]
  mov rcx,process_handle
  call WriteProcessMemory
  add rsp,30h
  mov rax,break_point_ptr
  mov byte ptr [rax + BreakPointInfo.efficacious],TRUE

CLOSE_HANDLE@31:
  mov rcx,process_handle
  call CloseHandle
EXIT_RESTOREINT3BREAKPOINT:
  add rsp,60h
  pop rsi
  pop rdi
  ret
RestoreInt3BreakPoint ENDP

;===============================================================
;设置内存断点
;SetMemoryBreakePoint(lpvoid address,dword64 length,dword type)
;不允许跨页
;===============================================================
SetMemoryBreakePoint PROC
local address:qword
local length_bp:qword
local process_handle:HANDLE
local mbi:MEMORY_BASIC_INFORMATION
local page_type:dword
local old_page_type:dword
  push rdi
  push rsi
  sub rsp,60h
  mov old_page_type,0
  test rcx,rcx
  je EXIT_SETMEMORYBREAKEPOINT
  mov address,rcx
  test rdx,rdx
  je EXIT_SETMEMORYBREAKEPOINT
  mov length_bp,rdx
  test r8,r8
  je EXIT_SETMEMORYBREAKEPOINT
  mov page_type,r8d
  ;判断内存断点是否已满
  xor rax,rax
  mov eax,g_mem_bp_count
  cmp rax,5
  jnl EXIT_SETMEMORYBREAKEPOINT
;  jmp ADDRESS_DISPOSE
;PAGE_TYPE_IS_NOT_NULL:
;  mov page_type,PAGE_NOACCESS
  ;跨页判断
  mov rcx,address
  and rcx,0FFFh
  add rcx,length_bp
  cmp rcx,1000h
  jge EXIT_SETMEMORYBREAKEPOINT
ADDRESS_DISPOSE:
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  ;hProcess == NULL
  test rax,rax
  je EXIT_SETMEMORYBREAKEPOINT
  mov process_handle,rax
  ;判断内存区域是否存在
  mov r9,sizeof MEMORY_BASIC_INFORMATION
  lea r8,mbi
  mov rdx,address
  mov rcx,process_handle
  call VirtualQueryEx
  test rax,rax
  je CLOSE_HANDLE
  cmp mbi.AllocationProtect,0
  je CLOSE_HANDLE
  ;判断是否有同样页基址的断点
  mov rsi,address
  and rsi,0FFFFFFFFFFFFF000h;page base
  mov rax,sizeof g_mem_bp_address_arr
  cqo
  mov rdi,sizeof MemBreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_CMP_BREAK:
  ;查找数组
  lea rax,g_mem_bp_address_arr
  mov r9,rcx
  imul r9,rdi
  mov rdx,qword ptr [rax+r9+MemBreakPointInfo.page_base]
  cmp rdx,rsi ;比较页基址是否一致
  jne CMP_NEXT_BREAK
  ;保存页面属性
  mov edx,dword ptr [rax+r9+MemBreakPointInfo.old_type]
  mov old_page_type,edx
  ;页基址一致 查找空位填入数据
  jmp SEARCH_EMPTY_NODE
CMP_NEXT_BREAK:
  dec rcx
  jne LOOP_CMP_BREAK
  ;没用找到相同的页基址

  

  ;设置断点
SET_MEM_BREAKE:
  lea rax,old_page_type
  push rax
  sub rsp,20h
  mov r9,PAGE_NOACCESS
  mov r8,length_bp
  mov rdx,address
  mov rcx,process_handle
  call VirtualProtectEx 
  add rsp,28h
  ;非零值
  test rax,rax
  je CLOSE_HANDLE
  ;记录数据
  jmp CLOSE_HANDLE_AND_SAVE_DATA
  ;mov rax,address
  ;mov g_mem_bp_address,rax
  ;mov rax,length_bp
  ;mov g_mem_bp_length,eax
  ;mov eax,page_type
  ;mov g_mem_bp_rwx_flag,eax

CLOSE_HANDLE:
  mov rcx,process_handle
  call CloseHandle
  jmp EXIT_SETMEMORYBREAKEPOINT

CLOSE_HANDLE_AND_SAVE_DATA:
  mov rcx,process_handle
  call CloseHandle
;查找空位
SEARCH_EMPTY_NODE:
  mov rax,sizeof g_mem_bp_address_arr
  cqo
  mov rdi,sizeof MemBreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_SEARCH_EMPTY_NODE:
  ;查找数组
  lea rax,g_mem_bp_address_arr
  mov r9,rcx
  imul r9,rdi
  mov rdx,qword ptr [rax+r9+MemBreakPointInfo.break_address]
  cmp rdx,0 ;是否是空的
  jne SEARCH_NEXT_NODE
  ;填入数据
  mov r8,address
  mov qword ptr [rax+r9+MemBreakPointInfo.break_address],r8
  and r8,0FFFFFFFFFFFFF000h;page base
  mov qword ptr [rax+r9+MemBreakPointInfo.page_base],r8
  mov r8d,old_page_type
  mov dword ptr [rax+r9+MemBreakPointInfo.old_type],r8d
  mov r8,length_bp
  mov dword ptr [rax+r9+MemBreakPointInfo.bp_length],r8d
  mov r8d,page_type
  mov dword ptr [rax+r9+MemBreakPointInfo.rxw_type],r8d


  jmp EXIT_SETMEMORYBREAKEPOINT

SEARCH_NEXT_NODE:
  dec rcx
  jne LOOP_SEARCH_EMPTY_NODE



EXIT_SETMEMORYBREAKEPOINT:
  mov rax,TRUE
  add rsp,60h
  pop rsi
  pop rdi
  ret
SetMemoryBreakePoint ENDP

;===============================================================
;设置硬件断点
;SetHardwareBreakePoint(lpvoid address,dword64 length,dword type,bool flag)
;===============================================================
SetHardwareBreakePoint PROC
local ctx:CONTEXT32
local address:qword
local length_bp:qword
local hbp_type:dword
local flag:dword
  push rdi
  push rsi
  sub rsp,58h
  test rcx,rcx
  je EXIT_SETHARDWAREBREAKEPOINT
  mov address,rcx
  test rdx,rdx
  je EXIT_SETHARDWAREBREAKEPOINT
  mov length_bp,rdx
  test r8,r8
  je EXIT_SETHARDWAREBREAKEPOINT
  mov hbp_type,r8d
  ;判断是要下全局还是局部断点
  cmp r9,1
  je GLOBAL_BREAKPOINT
  mov flag,0
  jmp GET_CTX
 GLOBAL_BREAKPOINT:
  mov flag,1

 GET_CTX:
  ;获取线程上下文
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  lea rcx,ctx
  call GetDebugThreadContext  
 
  ;转换参数
  cmp length_bp,1
  jne LENGTH_NOT_ONE
  mov length_bp,0b ;00 — 1-byte length.
  jmp CHECK_TYPE
LENGTH_NOT_ONE:
  cmp length_bp,2
  jne LENGTH_NOT_TWO
  mov length_bp,1b ;01 — 2-byte length.
  jmp CHECK_TYPE
LENGTH_NOT_TWO:
  cmp length_bp,4
  jne LENGTH_NOT_THREE
  mov length_bp,11b ;11 — 4-byte length.
  jmp CHECK_TYPE
LENGTH_NOT_THREE:
  cmp length_bp,8
  jne LENGTH_NOT_EIGHT
  mov length_bp,10b ;10 — 8-byte length.
  jmp CHECK_TYPE
LENGTH_NOT_EIGHT:
  jmp EXIT_SETHARDWAREBREAKEPOINT

CHECK_TYPE:
  cmp hbp_type,"x"
  jne LENGTH_NOT_X
  mov hbp_type,0 ;00 — 执行
  jmp CRAFT_ARGUMENT
LENGTH_NOT_X:
  cmp hbp_type,"w"
  jne LENGTH_NOT_W
  mov hbp_type,1b ;01 — 写
  jmp CRAFT_ARGUMENT
LENGTH_NOT_W:
  cmp hbp_type,"o"
  jne LENGTH_NOT_O
  mov hbp_type,10b ;10 — IO断点
  jmp CRAFT_ARGUMENT
LENGTH_NOT_O:
  cmp hbp_type,"r"
  jne LENGTH_NOT_R
  mov hbp_type,11b ;11 — 读
  jmp CRAFT_ARGUMENT
LENGTH_NOT_R:
  jmp EXIT_SETHARDWAREBREAKEPOINT
CRAFT_ARGUMENT:
  ;检测是否有空的可用寄存器
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,1
  test rax,rax
  jne DR7_L0_NOT_NULL
  mov rax,address
  mov ctx.Dr0@,eax
  ;dr7.fields.l0 = in
  cmp flag,1
  je SET_G0
 ;SET_L0:
 ;dr7.fields.l0 = 1
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,1  
  mov ctx.Dr7@,eax
  ;dr7.fields.le = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,8  
  mov ctx.Dr7@,eax
  jmp SET_G0_END
SET_G0:
  ;dr7.fields.g0 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,2  
  mov ctx.Dr7@,eax
  ;dr7.fields.ge = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,9  
  mov ctx.Dr7@,eax
SET_G0_END:
  ;dr7.fields.len0 = in;
  mov rax,length_bp
  and rax,3  
  shl rax,12h  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0FFF3FFFFh  
  or rcx,rax  
  mov ctx.Dr7@,ecx
  ;dr7.fields.rw0 = in;
  mov eax,hbp_type 
  and rax,3  
  shl rax,10h  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0FFFCFFFFh  
  or rcx,rax   
  mov ctx.Dr7@,ecx
  jmp SET_CTX
DR7_L0_NOT_NULL:
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,4
  test rax,rax
  jne DR7_L1_NOT_NULL

  mov rax,address
  mov ctx.Dr1@,eax

  cmp flag,1
  je SET_G1
  ;dr7.fields.l1 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,4  
  mov ctx.Dr7@,eax
  ;dr7.fields.le = 1;    
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,8  
  mov ctx.Dr7@,eax
  jmp SET_G1_END
SET_G1:
  ;dr7.fields.g1 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,8  
  mov ctx.Dr7@,eax
  ;dr7.fields.ge = 1;   
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,9  
  mov ctx.Dr7@,eax
SET_G1_END:
  ;dr7.fields.len1 = in;
  mov rax,length_bp
  and rax,3  
  shl rax,16h  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0FF3FFFFFh  
  or rcx,rax  
  mov ctx.Dr7@,ecx
  ;dr7.fields.rw1 = in;
  mov eax,hbp_type  
  and rax,3  
  shl rax,14h  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0FFCFFFFFh  
  or rcx,rax  
  mov rax,rcx  
  mov ctx.Dr7@,eax
  jmp SET_CTX
DR7_L1_NOT_NULL:
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,10h
  test rax,rax
  jne DR7_L2_NOT_NULL
  mov rax,address
  mov ctx.Dr2@,eax

  cmp flag,1
  je SET_G2
  ;dr7.fields.l2 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,10h  
  mov ctx.Dr7@,eax
  ;dr7.fields.le = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,8  
  mov ctx.Dr7@,eax
  jmp SET_G2_END
  SET_G2:
  ;dr7.fields.g2 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,20h  
  mov ctx.Dr7@,eax
  ;dr7.fields.ge = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,9  
  mov ctx.Dr7@,eax
  SET_G2_END:
  ;dr7.fields.len2 = in;
  mov rax,length_bp 
  and rax,3  
  shl rax,1Ah  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0F3FFFFFFh  
  or rcx,rax  
  mov ctx.Dr7@,ecx
  ;dr7.fields.rw2 = in;
  mov eax,hbp_type
  and rax,3  
  shl rax,18h  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0FCFFFFFFh  
  or rcx,rax  
  mov ctx.Dr7@,eax
  jmp SET_CTX
DR7_L2_NOT_NULL:
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,40h
  test rax,rax
  jne DR7_L3_NOT_NULL
  mov rax,address
  mov ctx.Dr3@,eax

  cmp flag,1
  je SET_G3
  ;dr7.fields.l3 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  or rax,40h  
  mov ctx.Dr7@,eax
  ;dr7.fields.le = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,8  
  mov ctx.Dr7@,eax
  jmp SET_G3_END
  SET_G3:
  ;dr7.fields.g3 = 1;
  xor rax,rax
  mov eax,ctx.Dr7@
  bts rax,7  
  mov ctx.Dr7@,eax
  ;dr7.fields.ge = 1;
  mov eax,ctx.Dr7@
  bts rax,9  
  mov ctx.Dr7@,eax
  SET_G3_END:
  ;dr7.fields.len3 = in;
  mov rax,length_bp
  and rax,3  
  shl rax,1Eh  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,3FFFFFFFh  
  or rcx,rax  
  mov ctx.Dr7@,ecx 
  ;dr7.fields.rw3 = in;
  mov eax,hbp_type
  and rax,3  
  shl rax,1Ch  
  xor rcx,rcx
  mov ecx,ctx.Dr7@
  and rcx,0CFFFFFFFh  
  or rcx,rax  
  mov ctx.Dr7@,ecx
  jmp SET_CTX
DR7_L3_NOT_NULL:
 jmp EXIT_SETHARDWAREBREAKEPOINT

SET_CTX:
  lea rcx,ctx
  call SetDebugThreadContext  

  
  ;mov r8,sizeof CONTEXT32
  ;mov rdx,0
  ;lea rcx,ctx
  ;call memset
  ;mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  ;lea rcx,ctx
  ;call GetDebugThreadContext 

EXIT_SETHARDWAREBREAKEPOINT:
  mov rax,TRUE
  add rsp,58h
  pop rsi
  pop rdi
  ret
SetHardwareBreakePoint ENDP


;===============================================================
;设置单步断点
;===============================================================

SetSingelStep PROC
local ctx:CONTEXT32
;local thread_handle:ptr HANDLE
  push  rdi
  push  rsi
  sub rsp,60h
  mov r8,sizeof CONTEXT32
  mov rdx,0
  lea rcx,ctx
  call memset
  ;ctx.ContextFlags = CONTEXT_ALL;
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  lea rcx,ctx
  call GetDebugThreadContext
  test rax,rax
  je EXIT_SETSINGELSTEP
  or ctx.EFlags,100h
  lea rcx,ctx
  call SetDebugThreadContext  
EXIT_SETSINGELSTEP:
  add rsp,60h
  pop rsi
  pop rdi
  ret
SetSingelStep ENDP


;===============================================================
;断点异常回调函数
;===============================================================

;断点异常回调函数
;ret dword
ExceptionBreakPoint PROC
local ctx:CONTEXT32
local exception_record:ptr EXCEPTION_RECORD
local exception_address:qword
local break_point_ptr:ptr qword
local ret_opcode:qword
local status:dword



;local status2[3]:dword
;local ctx:CONTEXT
  ;mov rax,sizeof CONTEXT
  push  rdi
  push  rsi
  ;push rbp
  ;mov rbp,rsp
  sub rsp,58h
  mov status,DBG_EXCEPTION_NOT_HANDLED
  mov exception_record,0
  mov break_point_ptr,0
  ;LPEXCEPTION_RECORD lpExceptionRecord = &DebugEvent.u.Exception.ExceptionRecord;
  lea rax,debug_event.u.Exception.ExceptionRecord
  mov exception_record,rax
  ;exception_address = lpExceptionRecord->ExceptionAddress
  mov rcx,qword ptr [rax + EXCEPTION_RECORD.ExceptionAddress]
  mov exception_address,rcx
  
  cmp is_system_break,0
  jne USER_DEFINED_BREAKE_POINT
  ;系统断点处理
  mov is_system_break,TRUE
  call GetCurrentRIP
  test rax,rax
  je EXIT_EXCEPTIONBREAKPOINT
  ;ctx.Rip -= 1;
  mov rdx,20
  mov rcx,rax
  dec rcx
  call DisassemblyForAddress
  call GetCommandLine
  mov status,DBG_CONTINUE
  jmp EXIT_EXCEPTIONBREAKPOINT

;
;  ;for (size_t Index = 0; Index < BreakPointCount; Index++)
;  xor rcx,rcx
;  mov ecx,break_point_int3_count
;  ;add rcx,1
;LOOP_GETNEXTBREAK@:
;  ;BPGroup[Index].BreakAddress
;  lea rdx,[rcx-1]
;  mov rax,sizeof BreakPointInfo
;  imul rax,rdx
;  lea rdx,break_point_int3
;  ;mov qword ptr [rdx+rax],rcx
;  lea r9,qword ptr [rdx+rax]
;  mov r8,qword ptr [r9]
;  ;if (BPGroup[Index].BreakAddress != lpExceptionRecord->ExceptionAddress)
;  cmp r8,exception_address
;  jne GET_NEXT_INDEX@
USER_DEFINED_BREAKE_POINT:
  mov rax,sizeof break_point_int3
  cqo
  mov rdi,sizeof BreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_GET_BP_ARR@21:
  ;lea rax,[rcx-1]
  mov rax,rcx
  push rcx
  imul rax,rdi
  lea rdx,break_point_int3
  mov cl,byte ptr[rdx+rax+BreakPointInfo.using] 
  test cl,cl
  je GET_NEXT_BP_ARR@21
  mov rcx,qword ptr[rdx+rax+BreakPointInfo.break_address]
  cmp rcx,exception_address
  jne GET_NEXT_BP_ARR@21
  lea r9,[rdx+rax]
  mov break_point_ptr,r9
  add rsp,8
  jmp RESTORE_BREAKE_POINT
GET_NEXT_BP_ARR@21:
  pop rcx
loop LOOP_GET_BP_ARR@21

RESTORE_BREAKE_POINT:
  ;还原软件断点位置
  mov r9,break_point_ptr
  mov rcx,exception_address
  mov r8,1
  lea rdx,[r9 + BreakPointInfo.old_code]
  call WriteDebugProcessMemory
  test rax,rax
  je EXIT_EXCEPTIONBREAKPOINT
  mov r9,break_point_ptr
  mov byte ptr [r9+BreakPointInfo.efficacious],0

  mov r8,sizeof CONTEXT32
  mov rdx,0
  lea rcx,ctx
  call memset
  ;ctx.ContextFlags = CONTEXT_ALL;
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  lea rcx,ctx
  call GetDebugThreadContext
  test rax,rax
  je EXIT_EXCEPTIONBREAKPOINT
  dec ctx.Eip
  lea rcx,ctx
  call SetDebugThreadContext
  ;如果是步过设置的断点
  mov r9,break_point_ptr
  mov rax,qword ptr [r9+BreakPointInfo.break_address]
  cmp rax,g_jmp_call_address
  jne CMP_EXE_TO_RET
  cmp g_jmp_call_flag,TRUE
  jne CMP_EXE_TO_RET
  ;是步过设置的断点
  mov g_jmp_call_flag,FALSE
  ;删除该断点
  mov rcx,g_jmp_call_address
  call DelInt3BreakPoint
  mov g_jmp_call_address,0;
  mov rdx,1
  jmp SHOW_CMD

CMP_EXE_TO_RET:
  ;判断是不是运行到ret设置的int3

  mov r9,break_point_ptr
  mov rax,qword ptr [r9+BreakPointInfo.break_address]
  cmp rax,g_step_to_ret_int3_address
  jne SET_RECOVER_SINGELSTEP
  cmp g_step_to_ret_int3_flag,TRUE
  jne SET_RECOVER_SINGELSTEP
  ;是是运行到ret设置的断点
  mov g_step_to_ret_int3_flag,FALSE
  ;删除该断点
  mov rcx,g_step_to_ret_int3_address
  call DelInt3BreakPoint

  ;读取当前RIP位置是否是RET操作码
  call GetCurrentRIP
  mov rcx,rax
  mov r9,8
  mov r8,8
  mov ret_opcode,0
  lea rdx,ret_opcode
  call ReadDebugProcessMemory 
  test eax,eax
  je EXIT_EXCEPTIONBREAKPOINT
  mov rax,ret_opcode
  ;and rax,0FFh
  cmp al,0C3h;RET（或 RETN）
  je IS_RET_OPCODE
  cmp al,0CBh;retf
  je IS_RET_OPCODE
  cmp al,0C2h;RET imm16（或 RETN imm16）
  je IS_RET_OPCODE
  cmp al,0CAh;RETF imm16
  je IS_RET_OPCODE
NOT_RET_OPCODE:
  ;不是继续设置步过
  call SteptoNextReturn
  mov status,DBG_CONTINUE
  jmp EXIT_EXCEPTIONBREAKPOINT
IS_RET_OPCODE:
  ;是ret
  mov rdx,1
  jmp SHOW_CMD
SET_RECOVER_SINGELSTEP:
  ;单步
  mov r9,break_point_ptr
  mov g_restore_int3_bp_arrr_ptr,r9
  mov g_restore_int3_flag,TRUE
  call SetSingelStep
  mov rdx,20
  ;mov rax,exception_record
  ;exception_address = lpExceptionRecord->ExceptionAddress
  ;mov rcx,qword ptr [rax + EXCEPTION_RECORD.ExceptionAddress]
  ;call GetCurrentRIP
  ;test rax,rax
  ;je EXIT_EXCEPTIONBREAKPOINT
  ;ctx.Rip -= 1;
SHOW_CMD:
  xor rax,rax
  mov ecx,ctx.Eip
  ;dec rcx
  call DisassemblyForAddress
  call GetCommandLine
  mov status,DBG_CONTINUE
  jmp EXIT_EXCEPTIONBREAKPOINT

;GET_NEXT_INDEX@:
;  dec rcx
;  jne LOOP_GETNEXTBREAK@
  ;非指定断点异常处理

EXIT_EXCEPTIONBREAKPOINT:
  mov eax,status
  ;leave 
  add rsp,58h
  pop rsi
  pop rdi
  ret
ExceptionBreakPoint ENDP

;===============================================================
;内存异常处理函数
;===============================================================
ExceptionMemoryEvent PROC
local exc_code:qword
local exc_mem_address:qword
local process_handle:HANDLE
local status:dword
local recover_page_type:dword
  push  rdi
  push  rsi
  sub rsp,60h
  mov status,DBG_EXCEPTION_NOT_HANDLED
  lea rax,debug_event.u.Exception.ExceptionRecord.ExceptionInformation
  mov rdx,qword ptr [rax];ExceptionInformation[0]
  mov exc_code,rdx
  mov rsi,qword ptr [rax+8h];ExceptionInformation[1]
  mov exc_mem_address,rsi
  ;判断是否是因为调试器修改内存属性而导致的C05
  cmp exc_code,-1
  je EXIT_EXCEPTIONMEMORYEVENT

  ;判断是不是设置的地址
  and rsi,0FFFFFFFFFFFFF000h

  mov rax,sizeof g_mem_bp_address_arr
  cqo
  mov rdi,sizeof MemBreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_CMP_BREAK_PAGE_BASE:
  ;查找数组
  lea rax,g_mem_bp_address_arr
  mov r9,rcx
  imul r9,rdi
  mov rdx,qword ptr [rax+r9+MemBreakPointInfo.page_base]
  cmp rdx,rsi ;比较页基址是否一致
  jne CMP_NEXT_BREAK_BASE
  ;获得原来的页属性page_type
  mov edx,dword ptr [rax+r9+MemBreakPointInfo.old_type]
  mov recover_page_type,edx
  ;恢复页面
  jmp RECOVER_MEM_PAGE_TYPE

CMP_NEXT_BREAK_BASE:
  dec rcx
  jne LOOP_CMP_BREAK_PAGE_BASE
  ;不是设置的断点页面
  jne EXIT_EXCEPTIONMEMORYEVENT

  ;mov rax,g_mem_bp_address
  ;and rax,0FFFFFFFFFFFFF000h
  ;cmp rax,rsi
  ;jne EXIT_EXCEPTIONMEMORYEVENT
RECOVER_MEM_PAGE_TYPE:
  ;恢复内存属性
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  ;hProcess == NULL
  test rax,rax
  je EXIT_EXCEPTIONMEMORYEVENT
  mov process_handle,rax
  ;记录页面基址
  mov g_restore_mem_page_base,rsi
  ;恢复
  lea rax,recover_page_type
  push rax
  sub rsp,20h
  xor r9,r9
  mov r9d,recover_page_type
  xor r8,r8
  mov r8d,1000h
  mov rdx,g_restore_mem_page_base
  mov rcx,process_handle
  call VirtualProtectEx 
  add rsp,28h
  ;非零值
  test rax,rax
  je CLOSE_HANDLE

  ;设置单步断点
  mov g_restore_mem_bp_flag,TRUE
  call SetSingelStep  

  mov rsi,g_restore_mem_page_base
  mov rax,sizeof g_mem_bp_address_arr
  cqo
  mov rdi,sizeof MemBreakPointInfo
  idiv rdi
  mov rcx,rax
  dec rcx
LOOP_CMP_BREAK_ADDRESS:
  ;查找数组
  lea rax,g_mem_bp_address_arr
  mov r9,rcx
  imul r9,rdi
  mov rdx,qword ptr [rax+r9+MemBreakPointInfo.page_base]
  cmp rdx,rsi ;比较页基址是否一致
  jne CMP_NEXT_BREAK_ADDRESS
  ;比较是否在断点范围
  
  mov rdx,qword ptr [rax+r9+MemBreakPointInfo.break_address]
  xor r8,r8
  mov r8d,dword ptr [rax+r9+MemBreakPointInfo.bp_length]
  add r8,rdx
  ;判断是不是目标断点 g_mem_bp_address <= mem_addr <=  g_mem_bp_address+length
  cmp exc_mem_address,rdx
  jl CMP_NEXT_BREAK_ADDRESS
  cmp exc_mem_address,r8
  jg CMP_NEXT_BREAK_ADDRESS
  ;判断是不是需要的异常类型
  xor rdx,rdx
  mov edx,dword ptr [rax+r9+MemBreakPointInfo.rxw_type]
  cmp exc_code,rdx
  jne CMP_NEXT_BREAK_ADDRESS

   ;用户断点
  call GetCurrentRIP  
  mov rdx,20
  mov rcx,rax
  call DisassemblyForAddress
  call GetCommandLine
  mov status,DBG_CONTINUE
  jmp CLOSE_HANDLE

CMP_NEXT_BREAK_ADDRESS:
  dec rcx
  jne LOOP_CMP_BREAK_ADDRESS
  mov status,DBG_CONTINUE



 
CLOSE_HANDLE:
  mov rcx,process_handle
  call CloseHandle
EXIT_EXCEPTIONMEMORYEVENT:
  mov eax,status
  add rsp,60h
  pop rsi
  pop rdi
  ret
ExceptionMemoryEvent ENDP
;===============================================================
;单步异常异常处理函数
;===============================================================
ExceptionSingelStep PROC
local ctx:CONTEXT32
local status:dword
local old_page_type:dword
local ret_opcode:qword
  push  rdi
  push  rsi
  sub rsp,60h
  mov status,DBG_EXCEPTION_NOT_HANDLED
  ;Get CONTEXT
  mov r8,sizeof CONTEXT32
  mov rdx,0
  lea rcx,ctx
  call memset
  ;ctx.ContextFlags = CONTEXT_ALL;
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  lea rcx,ctx
  call GetDebugThreadContext
  test rax,rax
  je EXIT_EXCEPTIONSINGELSTEP
  ;是内存断点要还原
  cmp g_restore_mem_bp_flag,TRUE
  jne NEXT_FUN@7
  mov g_restore_mem_bp_flag,FALSE
  ;恢复内存属性
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  ;hProcess == NULL
  test rax,rax
  je NEXT_FUN@7
  mov rdi,rax;hProcess
  ;恢复
  lea rax,old_page_type
  push rax
  sub rsp,20h
  xor r9,r9
  mov r9d,PAGE_NOACCESS
  xor r8,r8
  mov r8d,1000h
  mov rdx,g_restore_mem_page_base
  mov rcx,rdi
  call VirtualProtectEx 
  add rsp,28h
  mov g_restore_mem_page_base,0
  mov rcx,rdi
  call CloseHandle
  mov status,DBG_CONTINUE

  NEXT_FUN@7:
  ;是硬件读写断点要还原
  cmp g_hardware_int3_flag,TRUE
  jne NEXT_FUN@4
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  lea rcx,ctx
  call GetDebugThreadContext

  mov g_hardware_int3_flag,FALSE
  xor rax,rax
  movzx eax,g_hardware_lg_flag
  mov ecx,g_hardware_reg_index
  shl rax,cl;100000 << n
  mov rdx,11b
  shl rdx,cl;110000 << n
  not rdx;001111
  xor r8,r8
  mov r8d,ctx.Dr7@
  and r8,rdx;11001111
  or r8,rax;11001111 or 100000 = 11101111
  mov ctx.Dr7@,r8d
  lea rcx,ctx
  call SetDebugThreadContext
  mov status,DBG_CONTINUE
NEXT_FUN@4:

  ;是软件断点要还原断点
  cmp g_restore_int3_flag,TRUE
  jne NEXT_FUN@1
  mov g_restore_int3_flag,FALSE
  mov rcx,g_restore_int3_bp_arrr_ptr
  ;mov rdx,qword ptr [rax + BreakPointInfo.break_address]
  test rcx,rcx
  je NEXT_FUN@1
  call RestoreInt3BreakPoint  
  mov status,DBG_CONTINUE
NEXT_FUN@1:
  cmp g_step_flag,TRUE
  jne NEXT_FUN@2
  ;是单步断点
  mov g_step_flag,FALSE
  mov rdx,1
  mov rcx,rcx
  mov ecx,ctx.Eip
  call DisassemblyForAddress
  call GetCommandLine
  mov status,DBG_CONTINUE

NEXT_FUN@2:
  ;判断是不是硬件断点
  xor rax,rax
  mov eax,ctx.Dr6@
  and rax,0fh
  je NEXT_FUN@3
  cmp rax,1
  je IS_B0
  cmp rax,2
  je IS_B1
  cmp rax,4
  je IS_B2
  cmp rax,8
  je IS_B3
  jmp NEXT_FUN@3

IS_B0:
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,10h
  and rax,3  
  test eax,eax
  jne NOT_EXECUTE_BREAKE_POINT
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,11b
  mov g_hardware_lg_flag,al
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,0FFFFFFFCh
  mov ctx.Dr7@,eax
  mov g_hardware_reg_index,0
  jmp SET_CTX
IS_B1:
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,14h
  and rax,3  
  test eax,eax
  jne NOT_EXECUTE_BREAKE_POINT
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,2
  mov g_hardware_lg_flag,al
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,0FFFFFFF3h
  mov ctx.Dr7@,eax
  mov g_hardware_reg_index,1
  jmp SET_CTX
IS_B2:
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,18h
  and rax,3  
  test eax,eax
  jne NOT_EXECUTE_BREAKE_POINT
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,4
  mov g_hardware_lg_flag,al
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,0FFFFFFCFh
  mov ctx.Dr7@,eax
  mov g_hardware_reg_index,2
  jmp SET_CTX
IS_B3:
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,1ch
  and rax,3  
  test eax,eax
  jne NOT_EXECUTE_BREAKE_POINT
  xor rax,rax
  mov eax,ctx.Dr7@
  shr rax,6
  mov g_hardware_lg_flag,al
  xor rax,rax
  mov eax,ctx.Dr7@
  and rax,0FFFFFF3Fh
  mov ctx.Dr7@,eax
  mov g_hardware_reg_index,3
  jmp SET_CTX
SET_CTX:
  lea rcx,ctx
  call SetDebugThreadContext  
  mov g_hardware_int3_flag,TRUE
  call SetSingelStep  
NOT_EXECUTE_BREAKE_POINT:
  mov rdx,1
  xor rcx,rcx
  mov ecx,ctx.Eip
  call DisassemblyForAddress
  call GetCommandLine
  mov status,DBG_CONTINUE

NEXT_FUN@3:
;判断是不是运行到ret设置的单步
  cmp g_step_to_ret_step_flag,TRUE
  jne NEXT_FUN@8
  ;是运行到返回设置的标记
  mov g_step_to_ret_step_flag,FALSE
  ;读取当前RIP位置是否是RET操作码
  call GetCurrentRIP
  mov rsi,rax
  mov r9,8
  mov r8,8
  mov ret_opcode,0
  lea rdx,ret_opcode
  mov rcx,rsi
  call ReadDebugProcessMemory 
  test eax,eax
  je NEXT_FUN@8
  mov rax,ret_opcode
  ;and rax,0FFh
  cmp al,0C3h;RET（或 RETN）
  je IS_RET_OPCODE
  cmp al,0CBh;retf
  je IS_RET_OPCODE
  cmp al,0C2h;RET imm16（或 RETN imm16）
  je IS_RET_OPCODE
  cmp al,0CAh;RETF imm16
  je IS_RET_OPCODE
NOT_RET_OPCODE:
  ;不是继续设置步过
  call SteptoNextReturn
  mov status,DBG_CONTINUE
  jmp NEXT_FUN@8
IS_RET_OPCODE:
  ;是ret
  mov rdx,1
  mov rcx,rsi
  call DisassemblyForAddress
  call GetCommandLine
  mov status,DBG_CONTINUE
NEXT_FUN@8:

EXIT_EXCEPTIONSINGELSTEP:
  xor rax,rax
  mov eax,status
  add rsp,60h
  pop rsi
  pop rdi
  ret
ExceptionSingelStep ENDP

;===============================================================
;反汇编调试进程内指定地址
;DisassemblyForAddress(dword64 address,dword line)
;===============================================================

;反汇编指定地址
;rcx = DWORD64 address
;rdx = line number
DisassemblyForAddress PROC
local opcode_text[256]:byte
local code_data[1000h]:byte
local instruction:ZydisDisassembledInstruction
local src_address:qword
local process_handle:HANDLE
local read_length:qword
local runtime_address:qword
local offset@1:qword
local code_data_ptr:ptr byte
local index:dword
local line:dword
  push  rdi
  push  rsi
  sub rsp,58h
  mov read_length,0
  mov src_address,rcx
  mov line,edx
  ;add src_address,1
  ;code_data memset 0
  mov r8,sizeof code_data
  mov rdx,0
  lea rcx,code_data
  call memset
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  ;hProcess == NULL
  test rax,rax
  je EXIT_DISASSEMBLYFORADDRESS
  mov process_handle,rax
  ;ReadProcessMemory(hProcess, (LPVOID)Ctx.Rip, &CodeData, sizeof(CodeData), &stReadLength);
  
  lea rax,read_length
  ;sub rsp,8
  push rax
  sub rsp,20h
  mov r9,sizeof code_data
  lea r8,code_data
  mov rdx,src_address
  mov rcx,process_handle
  call ReadProcessMemory
  add rsp,28h
  ;read failed
  xor eax,0
  je CLOSE_HANDLE@21
  mov index,0
  mov rax,src_address
  mov runtime_address,rax
  mov offset@1,0
LOOP_DISASSEMBLE:
  ;while (ZYAN_SUCCESS(ZydisDisassembleIntel
  lea rax,instruction
  push rax  ;instruction
  sub rsp,20h
  mov rax,sizeof code_data
  sub rax,offset@1
  mov r9,rax ;length

  mov rax,offset@1
  lea r8,code_data[rax];buffer

  mov rdx,runtime_address
  mov rcx,ZYDIS_MACHINE_MODE_LONG_64
  call ZydisDisassembleIntel
  add rsp,28h
  and eax,80000000h
  test eax,eax
  jne CLOSE_HANDLE@21
  ;lea r8,instruction.text
  mov rdx,runtime_address
  lea rcx,string_zydis_address
  call printf

  lea rax,code_data
  add rax,offset@1
  mov code_data_ptr,rax
  mov rdi,0
LOOP_PRINTF_OPCODE:
  mov rax,code_data_ptr
  xor rdx,rdx
  mov dl,byte ptr[rax]
  lea rcx,string_zydis_opcode
  call printf
  inc rdi
  inc code_data_ptr
  ;if r9 = instruction.info.length@
  cmp dil,instruction.info.length@
  jb LOOP_PRINTF_OPCODE
  imul rdi,3
LOOP_PRINTF_NULL:
  inc rdi
  cmp rdi,30
  jnb PRINTF_DISASSMBLE
  lea rcx,string_zydis_null
  call printf
  jmp LOOP_PRINTF_NULL

PRINTF_DISASSMBLE:
  ;判断是不是call 是尝试获取名称
  lea rdx,opcode_text
  lea rcx,instruction.text
  call GetFunName
  test rax,rax
  je NOT_FUN_NAME
  ;获取到函数名
  lea rdx,opcode_text
  lea rcx,string_zydis_disassmble
  call printf
  jmp NEXT_OPCODE
NOT_FUN_NAME:
  lea rdx,instruction.text
  lea rcx,string_zydis_disassmble
  call printf

NEXT_OPCODE:

  ;offset += instruction.info.length;
  ;mov rax,offset@1
  xor r8,r8
  mov r8b,instruction.info.length@
  add offset@1,r8
  ;runtime_address += instruction.info.length;
  add runtime_address,r8
  ;Index++
  inc index
  ;if (Index == dwLine) { break; }
  mov eax,line
  cmp index,eax
  jge CLOSE_HANDLE@21

  ;while
  jmp LOOP_DISASSEMBLE


CLOSE_HANDLE@21:
  mov rcx,process_handle
  call CloseHandle

EXIT_DISASSEMBLYFORADDRESS:
  mov rax,0
  add rsp,58h
  pop rsi
  pop rdi
  ret
DisassemblyForAddress ENDP

;===============================================================
;显示或修改寄存器
;ShowOREditRegedit(char* reg_str, DWORD64 value, bool flag)
;===============================================================
ShowOREditRegedit PROC
local ctx:CONTEXT32
local reg_str:ptr qword
local reg_value:qword
local edit_flag:dword 
  push  rdi
  push  rsi
  sub rsp,58h
  mov reg_str,0
  mov reg_str,rcx
  mov reg_value,0
  mov reg_value,rdx
  mov edit_flag,FALSE
  mov edit_flag,r8d
  ;获取cxt
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  lea rcx,ctx
  call GetDebugThreadContext
  test rax,rax
  je EXIT_SHOWOREDITREGEDIT

  ;reg_str == NULL 显示寄存器
  mov r8d,edit_flag
  test r8d,r8d
  jne EDIT_REGEDIT
  ;显示寄存器
  xor r9,r9
  xor r8,r8
  xor rcx,rcx
  mov r9d,ctx.Ecx@
  mov r8d,ctx.Ebx@
  mov edx,ctx.Eax@
  lea rcx,string_show_reg_0
  call printf

  xor r9,r9
  xor r8,r8
  xor rcx,rcx
  mov r9d,ctx.Edi@
  mov r8d,ctx.Esi@
  mov edx,ctx.Edx@
  lea rcx,string_show_reg_1
  call printf

  xor r9,r9
  xor r8,r8
  xor rcx,rcx
  mov r9d,ctx.Ebp@
  mov r8d,ctx.Esp@
  mov edx,ctx.Eip
  lea rcx,string_show_reg_2
  call printf

  xor r9,r9
  xor r8,r8
  xor rcx,rcx
  mov r9d,ctx.Dr2@
  mov r8d,ctx.Dr1@
  mov edx,ctx.Dr0@
  lea rcx,string_show_reg_3
  call printf

  xor r9,r9
  xor r8,r8
  xor rcx,rcx
  mov r9d,ctx.Dr7@
  mov r8d,ctx.Dr6@
  mov edx,ctx.Dr3@
  lea rcx,string_show_reg_4
  call printf

  ;xor r9,r9
  ;xor r8,r8
  ;xor rcx,rcx
  xor r9,r9
  mov r9d,ctx.EFlags
  ;mov r8d,ctx.Dr7@
  ;mov edx,ctx.Dr6@
  lea rcx,string_show_reg_5
  call printf

  jmp EXIT_SHOWOREDITREGEDIT

EDIT_REGEDIT:
;修改寄存器
  
  lea rdx,string_edit_reg_0
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG0
  mov rax,reg_value
  mov ctx.Eax@,eax
  jmp SET_CTX
NEXT_REG0:
  lea rdx,string_edit_reg_1
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG1
  mov rax,reg_value
  mov ctx.Ebx@,eax
  jmp SET_CTX
NEXT_REG1:
  lea rdx,string_edit_reg_2
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG2
  mov rax,reg_value
  mov ctx.Ecx@,eax
  jmp SET_CTX
NEXT_REG2:
  lea rdx,string_edit_reg_3
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG3
  mov rax,reg_value
  mov ctx.Edx@,Eax
  jmp SET_CTX
NEXT_REG3:
  lea rdx,string_edit_reg_4
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG4
  mov rax,reg_value
  mov ctx.Esi@,eax
  jmp SET_CTX
NEXT_REG4:
  lea rdx,string_edit_reg_5
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG5
  mov rax,reg_value
  mov ctx.Edi@,eax
  jmp SET_CTX
NEXT_REG5:
  lea rdx,string_edit_reg_6
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG6
  mov rax,reg_value
  mov ctx.Eip,eax
  jmp SET_CTX
NEXT_REG6:
  lea rdx,string_edit_reg_7
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG7
  mov rax,reg_value
  mov ctx.Esp@,eax
  jmp SET_CTX
NEXT_REG7:
  lea rdx,string_edit_reg_8
  mov rcx,reg_str
  call strcmp
  test eax,eax
  jne NEXT_REG8
  mov rax,reg_value
  mov ctx.Ebp@,eax
  jmp SET_CTX
NEXT_REG8:
;  lea rdx,string_edit_reg_9
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG9
;  mov rax,reg_value
;  mov ctx.R8@,rax
;  jmp SET_CTX
;NEXT_REG9:
;  lea rdx,string_edit_reg_10
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG10
;  mov rax,reg_value
;  mov ctx.R9@,rax
;  jmp SET_CTX
;NEXT_REG10:
;  lea rdx,string_edit_reg_11
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG11
;  mov rax,reg_value
;  mov ctx.R10@,rax
;  jmp SET_CTX
;NEXT_REG11:
;  lea rdx,string_edit_reg_12
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG12
;  mov rax,reg_value
;  mov ctx.R11@,rax
;  jmp SET_CTX
;NEXT_REG12:
;  lea rdx,string_edit_reg_13
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG13
;  mov rax,reg_value
;  mov ctx.R12@,rax
;  jmp SET_CTX
;NEXT_REG13:
;  lea rdx,string_edit_reg_14
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG14
;  mov rax,reg_value
;  mov ctx.R13@,rax
;  jmp SET_CTX
;NEXT_REG14:
;  lea rdx,string_edit_reg_15
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG15
;  mov rax,reg_value
;  mov ctx.R14@,rax
;  jmp SET_CTX
;NEXT_REG15:
;  lea rdx,string_edit_reg_16
;  mov rcx,reg_str
;  call strcmp
;  test eax,eax
;  jne NEXT_REG16
;  mov rax,reg_value
;  mov ctx.R15@,rax
;  jmp SET_CTX
;NEXT_REG16:
jmp EXIT_SHOWOREDITREGEDIT

SET_CTX:
 lea rcx,ctx
 call SetDebugThreadContext 

EXIT_SHOWOREDITREGEDIT:
  add rsp,58h
  pop rsi
  pop rdi
  ret
ShowOREditRegedit ENDP

;===============================================================
;显示指定地址内存 固定读取0x80字节
;ShowDebugProcessMemory(DWORD64 address, DWORD len)
;===============================================================
ShowDebugProcessMemory PROC
local buffer[80h]:byte
local address:qword
local show_len:dword
local read_len:dword
local buffer_index:dword
local header_index:dword
  push  rdi
  push  rsi
  sub rsp,58h
  mov address,0
  mov address,rcx
  mov show_len,0
  mov show_len,edx
  mov read_len,80h

  cmp show_len,0
  jle EXIT_SHOWDEBUGPROCESSMEMORY

  ;mov buffer,NULL
  ;xor rcx,rcx
  ;mov ecx,read_len
  ;call malloc
  ;test rax,rax
  ;je EXIT_SHOWDEBUGPROCESSMEMORY
  ;mov buffer,rax
  xor r8,r8
  mov r8d,read_len
  mov rdx,0
  lea rcx,buffer
  call memset
  ;读取内存
  xor r9,r9
  mov r9d,read_len
  xor r8,r8
  mov r8d,read_len
  lea rdx,buffer
  mov rcx,address
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMEMORY
  ;按规则显示
  xor rax,rax
  mov eax,read_len
  cqo
  xor rdi,rdi
  mov edi,show_len
  idiv rdi
  mov rdi,rax

  cmp show_len,1
  jne SHOW_DD
  mov buffer_index,0
  mov header_index,0
  lea rsi,buffer
LOOP_SHOW_DB:
  cmp header_index,0
  jne PRINT_DB
  ;打印地址
  mov rdx,address
  lea rcx,string_line_address
  call printf
PRINT_DB:
  xor rdx,rdx
  mov dl,byte ptr [rsi]
  lea rcx,string_line_db
  call printf
 


NEXT_PRINT_DB:
  inc rsi
  inc buffer_index
  inc header_index
  cmp header_index,10h
  jl NEXT_PRINT_DB2
  mov header_index,0
  add address,10h

  lea rcx,string_line_feed
  call printf
NEXT_PRINT_DB2:
  cmp buffer_index,80h
  jl LOOP_SHOW_DB
  jmp EXIT_SHOWDEBUGPROCESSMEMORY
SHOW_DD:

  cmp show_len,4
  jne SHOW_DQ
  mov buffer_index,0
  mov header_index,0
  lea rsi,buffer
LOOP_SHOW_DD:
  cmp header_index,0
  jne PRINT_DD
  ;打印地址
  mov rdx,address
  lea rcx,string_line_address
  call printf
PRINT_DD:
  xor rdx,rdx
  mov edx,dword ptr [rsi]
  lea rcx,string_line_dd
  call printf
 


NEXT_PRINT_DD:
  add rsi,4
  add buffer_index,4
  add header_index,4
  cmp header_index,10h
  jl NEXT_PRINT_DD2
  mov header_index,0
  add address,10h

  lea rcx,string_line_feed
  call printf
NEXT_PRINT_DD2:
  cmp buffer_index,80h
  jl LOOP_SHOW_DD
  jmp EXIT_SHOWDEBUGPROCESSMEMORY

SHOW_DQ:

cmp show_len,8
  jne EXIT_SHOWDEBUGPROCESSMEMORY
  mov buffer_index,0
  mov header_index,0
  lea rsi,buffer
LOOP_SHOW_DQ:
  cmp header_index,0
  jne PRINT_DQ
  ;打印地址
  mov rdx,address
  lea rcx,string_line_address
  call printf
PRINT_DQ:
  mov rdx,qword ptr [rsi]
  lea rcx,string_line_dq
  call printf
 


NEXT_PRINT_DQ:
  add rsi,8
  add buffer_index,8
  add header_index,8
  cmp header_index,10h
  jl NEXT_PRINT_DQ2
  mov header_index,0
  add address,10h

  lea rcx,string_line_feed
  call printf
NEXT_PRINT_DQ2:
  cmp buffer_index,80h
  jl LOOP_SHOW_DQ
  jmp EXIT_SHOWDEBUGPROCESSMEMORY

;FREE_BUFFER:
;  mov rcx,buffer
;  call free


EXIT_SHOWDEBUGPROCESSMEMORY:
  add rsp,58h
  pop rsi
  pop rdi
  ret
ShowDebugProcessMemory ENDP

;===============================================================
;显示进程PEB下已经加载的模块列表
;ShowDebugProcessModuleList()
;===============================================================
ShowDebugProcessModuleList PROC
local pbi:PROCESS_BASIC_INFORMATION
local address_ptr:ptr qword
local query_szie:qword
local buffer[256]:byte
local wbuffer[256]:byte
local ustr:UNICODE_STRING32
  push  rdi
  push  rsi
  sub rsp,60h
  mov address_ptr,0
  ;lea rcx,string_lm_lib
  ;call LoadLibraryA
  ;lea rdx,string_lm_funname
  ;mov rcx,rax
  ;call GetProcAddress
  ;mov rdi,rax
  mov r8,sizeof PROCESS_BASIC_INFORMATION
  mov rdx,0
  lea rcx,pbi
  call memset

  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  test rax,rax
  je EXIT_SHOWDEBUGPROCESSMODULELIST
  mov rdi,rax

  lea rax,query_szie
  push rax
  sub rsp,20h
  mov r9,sizeof PROCESS_BASIC_INFORMATION
  lea r8,pbi
  mov rdx,ProcessBasicInformation
  xor rcx,rcx
  mov rcx,rdi
  call NtQueryInformationProcess  
  add rsp,28h
  ;NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
  mov rcx,rdi
  call CloseHandle
  cmp edi,0
  jl EXIT_SHOWDEBUGPROCESSMODULELIST

  ;mov rax,gs:[60h]	;TEB+0x60->PEB 
  ;mov rax,[rax+18h]	;PEB+0x18->Ldr
  ;在64系统中,32位进程会有两套PEB.
  ;32位和64位的PEB偏移正好相差 0x1000.
  ;如果cpudbg是64位的, 则使用 NtQueryInformationProcess 或者 GS:[0x60] 获取的PEB是64位的.
  ;相反如果cpudbg是32位的, 则使用 NtQueryInformationProcess 或者 FS:[0x30] 获取的PEB是32位的.
  ;在使用64位调试器里, 要想使用32位的 PEB 只需将得到的 PEB 加上 0x1000 即可.如下:
  ;
  ;pbi.PebBaseAddress = (PPEB)((DWORD64)pbi.PebBaseAddress+0x1000);
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  ;xor rcx,rcx
  mov rcx,pbi.PebBaseAddress
  add rcx,1000h
  add rcx,0ch
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMODULELIST
  ;mov rsi,[rax+20h]	;Ldr+0x20->InMemoryOrderModuleList

  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,address_ptr
  add rcx,14h
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMODULELIST
  mov rdi,address_ptr
  jmp START_READ_MODULE
LOOP_READ_MODULE:
  ;InMemoryOrderModuleList->BLink
  ;mov rax,qword ptr [rsi]
  ;mov rdi,rax
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,address_ptr
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMODULELIST
  mov rdi,address_ptr

START_READ_MODULE:
  mov r8,256
  mov rdx,0
  lea rcx,buffer
  call memset

  mov r8,256
  mov rdx,0
  lea rcx,wbuffer
  call memset

  ;mov rdx,qword ptr [rdi+48h];_LDR_DATA_TABLE_ENTRY->BaseDllName
  ;test rdx,rdx
  ;je EXIT_SHOWDEBUGPROCESSMODULELIST

  mov r9,sizeof UNICODE_STRING32
  mov r8,sizeof UNICODE_STRING32
  lea rdx,ustr
  mov rcx,rdi
  add rcx,24h
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMODULELIST
  cmp ustr.Buffer,0
  je EXIT_SHOWDEBUGPROCESSMODULELIST

  mov r9,256
  xor r8,r8
  mov r8w,ustr.Length@
  lea rdx,wbuffer
  xor rcx,rcx
  mov ecx,ustr.Buffer
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMODULELIST


  ;UNICODE_STRING

  push NULL
  push NULL 
  push 256
  lea rax,buffer
  push rax
  sub rsp,20h
  xor r9,r9
  mov r9w,ustr.Length@
  lea r8,wbuffer
  mov rdx,0
  mov rcx,CP_ACP
  call WideCharToMultiByte
  add rsp,40h

  mov r8,256
  lea rdx,wbuffer
  lea rcx,buffer
  call wcstombs
  ;lea rcx,[rsi+20h]	;获取kernel32.dll的基址 (_LDR_DATA_TABLE_ENTRY->InMemoryOrderLinks) + 0x20 -> DllBase
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,rdi
  add rcx,10h
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_SHOWDEBUGPROCESSMODULELIST

  lea r8,buffer
  mov rdx,address_ptr
  lea rcx,string_lm_str
  call printf
  mov address_ptr,rdi
  jmp LOOP_READ_MODULE

  
EXIT_SHOWDEBUGPROCESSMODULELIST:
  add rsp,60h
  pop rsi
  pop rdi
  ret
ShowDebugProcessModuleList ENDP

;===============================================================
;dump PE
;DumpPE()
;===============================================================
DumpPE PROC
local pbi:PROCESS_BASIC_INFORMATION
local image_dos:IMAGE_DOS_HEADER
local image_nt:IMAGE_NT_HEADERS32
local image_section:IMAGE_SECTION_HEADER
local address_ptr:qword
local query_szie:qword
local image_base:qword
local file_handle:HANDLE
local buffer_ptr:ptr qword
  push  rdi
  push  rsi
  sub rsp,60h
  ;打开文件
  mov file_handle,INVALID_HANDLE_VALUE
  mov rdx,CREATE_ALWAYS
  lea rcx,g_dump_pe_path
  call CreateFileGetHandle
  cmp rax,INVALID_HANDLE_VALUE
  je EXIT_DUMPPE
  mov file_handle,rax

  ;获取PEB
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  test rax,rax
  je CLOSE_HANDLE
  mov rdi,rax

  lea rax,query_szie
  push rax
  sub rsp,20h
  mov r9,sizeof PROCESS_BASIC_INFORMATION
  lea r8,pbi
  mov rdx,ProcessBasicInformation
  xor rcx,rcx
  mov rcx,rdi
  call NtQueryInformationProcess  
  add rsp,28h
  ;NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
  mov rcx,rdi
  call CloseHandle
  cmp edi,0
  jl CLOSE_HANDLE

  ;获取imagebase peb+0x10->ImageBaseAddress
  mov image_base,0
  mov r9,4
  mov r8,4
  lea rdx,image_base
  mov rcx,pbi.PebBaseAddress
  add rcx,1000h
  add rcx,8h
  call ReadDebugProcessMemory
  test eax,eax
  je CLOSE_HANDLE
  ;获取DOS
  mov r9,sizeof IMAGE_DOS_HEADER
  mov r8,sizeof IMAGE_DOS_HEADER
  lea rdx,image_dos
  mov rcx,image_base
  call ReadDebugProcessMemory
  test eax,eax
  je CLOSE_HANDLE
  ;获取NT
  mov r9,sizeof IMAGE_NT_HEADERS32
  mov r8,sizeof IMAGE_NT_HEADERS32
  lea rdx,image_nt
  xor rcx,rcx
  mov ecx,image_dos.e_lfanew
  add rcx,image_base
  call ReadDebugProcessMemory
  test eax,eax
  je CLOSE_HANDLE

  ;读取imageheader
  xor rcx,rcx
  mov ecx,image_nt.OptionalHeader.SizeOfHeaders
  call malloc
  test rax,rax
  je CLOSE_HANDLE
  mov buffer_ptr,rax

  xor r8,r8
  mov r8d,image_nt.OptionalHeader.SizeOfHeaders
  mov rdx,0
  mov rcx,buffer_ptr
  call memset

  xor r9,r9
  xor r8,r8
  mov r9d,image_nt.OptionalHeader.SizeOfHeaders
  mov r8d,image_nt.OptionalHeader.SizeOfHeaders
  mov rdx,buffer_ptr
  mov rcx,image_base
  call ReadDebugProcessMemory
  test eax,eax
  je FREE_BUFFER

  ;写入imageheader
  mov r8d,image_nt.OptionalHeader.SizeOfHeaders
  mov rdx,buffer_ptr
  mov rcx,file_handle
  call WriteDataToFile
  test eax,eax
  je FREE_BUFFER

  mov rcx,buffer_ptr
  call free
  mov buffer_ptr,0

  ;获取节区表数量
  xor rdi,rdi
  mov di,image_nt.FileHeader.NumberOfSections
  ;计算节区表起始地址
  xor rdx,rdx
  mov edx,image_dos.e_lfanew
  add rdx,image_base
  add rdx,sizeof IMAGE_NT_HEADERS32
  mov address_ptr,rdx
LOOP_READ_WRITE_SECTION:
  ;获取节区表

  mov r8,sizeof IMAGE_SECTION_HEADER
  mov rdx,0
  lea rcx,image_section
  call memset

  mov r9,sizeof IMAGE_SECTION_HEADER
  mov r8,sizeof IMAGE_SECTION_HEADER
  lea rdx,image_section
  mov rcx,address_ptr
  call ReadDebugProcessMemory
  test eax,eax
  je FREE_BUFFER



  ;读取内存中的数据
  xor rcx,rcx
  mov ecx,image_section.SizeOfRawData
  call malloc
  test rax,rax
  je CLOSE_HANDLE
  mov buffer_ptr,rax

  xor r8,r8
  mov r8d,image_section.SizeOfRawData
  mov rdx,0
  mov rcx,buffer_ptr
  call memset

  xor r9,r9
  xor r8,r8
  mov r9d,image_section.SizeOfRawData
  mov r8d,image_section.SizeOfRawData
  mov rdx,buffer_ptr
  xor rcx,rcx
  mov ecx,image_section.VirtualAddress
  add rcx,image_base
  call ReadDebugProcessMemory
  test eax,eax
  je FREE_BUFFER

  ;移动文件指针到区段起始地址
  mov r9,FILE_BEGIN
  mov r8,NULL
  xor rdx,rdx
  mov edx,image_section.PointerToRawData
  mov rcx,file_handle
  call SetFilePointer
  cmp eax,INVALID_SET_FILE_POINTER
  je FREE_BUFFER


  ;写入文件
  xor r8,r8
  mov r8d,image_section.SizeOfRawData
  mov rdx,buffer_ptr
  mov rcx,file_handle
  call WriteDataToFile
  test eax,eax
  je FREE_BUFFER

  mov rcx,buffer_ptr
  call free
  mov buffer_ptr,0
  ;计算下一个节区表的偏移
  mov rax,sizeof IMAGE_SECTION_HEADER
  add address_ptr,rax
  dec rdi
  cmp rdi,0
  jg LOOP_READ_WRITE_SECTION


FREE_BUFFER:
  cmp buffer_ptr,0
  je CLOSE_HANDLE
  mov rcx,buffer_ptr
  call free
  mov buffer_ptr,0
CLOSE_HANDLE:
  cmp file_handle,0
  je EXIT_DUMPPE
  cmp file_handle,INVALID_HANDLE_VALUE
  je EXIT_DUMPPE
  mov rcx,file_handle
  call CloseHandle

EXIT_DUMPPE:
  add rsp,60h
  pop rsi
  pop rdi
  ret
DumpPE ENDP
;===============================================================
;写入数据到内存当中
;WriteDataToMemory(VOID* address, DWORD len, DWORD64 value)
;===============================================================

WriteDataToMemory PROC
local address:qword
local value:qword
local write_length:dword
  push  rdi
  push  rsi
  sub rsp,58h
  mov address,0
  mov address,rcx
  mov write_length,0
  mov write_length,edx
  mov value,0
  mov value,r8

  cmp write_length,1
  je WRITE_TO_MEMORY
  cmp write_length,4
  je WRITE_TO_MEMORY
  cmp write_length,8
  je WRITE_TO_MEMORY
  jmp EXIT_WRITEDATATOMEMORY
WRITE_TO_MEMORY:
  xor r8,r8
  mov r8d,write_length
  lea rdx,value
  mov rcx,address
  call WriteDebugProcessMemory  

EXIT_WRITEDATATOMEMORY:
  add rsp,58h
  pop rsi
  pop rdi
  ret
WriteDataToMemory ENDP

;===============================================================
;判断是不是call指令 如果是返回指令长度
;DWORD IsCall(DWORD64 opcode)
;===============================================================
IsCall PROC
local opcode:qword
local opcode_len:dword  
  push  rdi
  push  rsi
  sub rsp,60h
  mov opcode,0
  mov opcode,rcx
  mov opcode_len,0

  ;换成大端
  xor rax,rax
  mov rdx,opcode
  mov rcx,7
LOOP_OVERTURN:
  mov al,dl
  shl rax,8
  shr rdx,8
loop LOOP_OVERTURN
  mov opcode,rax

  ;判断是不是call
  mov rdx,opcode
  shr rdx,24
  mov rax,6741FF1424h
  cmp rdx,rax ;call qword ptr ds:[r12d]  
  je OPCODE_LEN5
  mov rax,6741FF5500h
  cmp rdx,rax ;call qword ptr ds:[r13d]   
  je OPCODE_LEN5
  shr rdx,8
  cmp rdx,41FF1424h ;call qword ptr ds:[r12]
  je OPCODE_LEN4
  cmp rdx,41FF5500h ;call qword ptr ds:[r13] 
  je OPCODE_LEN4
  cmp rdx,67FF5500h ;call qword ptr ss:[ebp]
  je OPCODE_LEN4
  cmp rdx,67FF1424h ;call qword ptr ss:[esp]
  je OPCODE_LEN4
  cmp rdx,6741FF10h ;call qword ptr ds:[r8d]
  je OPCODE_LEN4
  cmp rdx,6741FF11h ;call qword ptr ds:[r9d]
  je OPCODE_LEN4
  cmp rdx,6741FF12h ;call qword ptr ds:[r10d]
  je OPCODE_LEN4
  cmp rdx,6741FF13h ;call qword ptr ds:[r11d]
  je OPCODE_LEN4
  cmp rdx,6741FF16h ;call qword ptr ds:[r14d]
  je OPCODE_LEN4
  cmp rdx,6741FF17h ;call qword ptr ds:[r15d]
  je OPCODE_LEN4
  shr rdx,8
  cmp rdx,0FF1425h ;FF1425 01000000 call qword ptr ds:[1]
  je OPCODE_LEN7
  cmp rdx,41FFD0h   ;call r8
  je OPCODE_LEN3
  cmp rdx,41FFD1h ;call r9 
  je OPCODE_LEN3
  cmp rdx,41FFD2h ;call r10
  je OPCODE_LEN3
  cmp rdx,41FFD3h ;call r11
  je OPCODE_LEN3
  cmp rdx,41FFD4h ;call r12
  je OPCODE_LEN3
  cmp rdx,41FFD5h ;call r13
  je OPCODE_LEN3
  cmp rdx,41FFD6h ;call r14
  je OPCODE_LEN3
  cmp rdx,41FFD7h ;call r15
  je OPCODE_LEN3
  cmp rdx,0FF5500h ;call qword ptr ss:[rbp]
  je OPCODE_LEN3
  cmp rdx,0FF1424h ;call qword ptr ss:[rsp]
  je OPCODE_LEN3
  cmp rdx,41FF10h ;call qword ptr ds:[r8]
  je OPCODE_LEN3
  cmp rdx,41FF11h ;call qword ptr ds:[r9]
  je OPCODE_LEN3
  cmp rdx,41FF12h ;call qword ptr ds:[r10]
  je OPCODE_LEN3
  cmp rdx,41FF13h ;call qword ptr ds:[r11]
  je OPCODE_LEN3
  cmp rdx,41FF16h ;call qword ptr ds:[r14]
  je OPCODE_LEN3
  cmp rdx,41FF17h ;call qword ptr ds:[r15]
  je OPCODE_LEN3
  cmp rdx,67FF10h ;call qword ptr ds:[eax]
  je OPCODE_LEN3
  cmp rdx,67FF13h ;call qword ptr ds:[ebx]
  je OPCODE_LEN3
  cmp rdx,67FF11h ;call qword ptr ds:[ecx]
  je OPCODE_LEN3
  cmp rdx,67FF12h ;call qword ptr ds:[edx]
  je OPCODE_LEN3
  cmp rdx,67FF16h ;call qword ptr ds:[esi]
  je OPCODE_LEN3
  cmp rdx,67FF17h ;call qword ptr ds:[edi]
  je OPCODE_LEN3
  shr rdx,8
  cmp rdx,0FFD0h ;call rax
  je OPCODE_LEN2
  cmp rdx,0FFD3h ;call rbx
  je OPCODE_LEN2
  cmp rdx,0FFD1h ;call rcx
  je OPCODE_LEN2
  cmp rdx,0FFD2h ;call rdx
  je OPCODE_LEN2
  cmp rdx,6741FF13h ;FFD5call rbp
  je OPCODE_LEN2
  cmp rdx,0FFD4h ;call rsp
  je OPCODE_LEN2
  cmp rdx,0FFD6h ;call rsi
  je OPCODE_LEN2
  cmp rdx,0FFD7h ;call rdi
  je OPCODE_LEN2
  cmp rdx,0FF10h ;call qword ptr ds:[rax]
  je OPCODE_LEN2
  cmp rdx,0FF13h ;call qword ptr ds:[rbx]
  je OPCODE_LEN2
  cmp rdx,0FF11h ;call qword ptr ds:[rcx]
  je OPCODE_LEN2
  cmp rdx,0FF12h ;call qword ptr ds:[rdx]
  je OPCODE_LEN2
  cmp rdx,0FF16h ;call qword ptr ds:[rsi]
  je OPCODE_LEN2
  cmp rdx,0FF17h ;call qword ptr ds:[rdi]
  je OPCODE_LEN2
  cmp rdx,0FF15h ;FF15 00004889 call qword ptr ds:[7FF691773C69]
  je OPCODE_LEN6
  shr rdx,8
  cmp rdx,0e8h;E8 11223344 
  je OPCODE_LEN5
  jmp EXIT_ISCALL
  
OPCODE_LEN7:
  ;长度7
  mov opcode_len,7
  jmp EXIT_ISCALL

OPCODE_LEN6:
  ;长度6
  mov opcode_len,6
  jmp EXIT_ISCALL

OPCODE_LEN5:
  ;长度5
  mov opcode_len,5
  jmp EXIT_ISCALL

OPCODE_LEN4:
  ;长度4
  mov opcode_len,4
  jmp EXIT_ISCALL

OPCODE_LEN3:
  ;长度3
  mov opcode_len,3
  jmp EXIT_ISCALL

OPCODE_LEN2:
  ;长度2
  mov opcode_len,2
  jmp EXIT_ISCALL

EXIT_ISCALL:
  xor rax,rax
  mov eax,opcode_len
  add rsp,60h
  pop rsi
  pop rdi
  ret
IsCall ENDP

;===============================================================
;单步步过
;SingleStepJmpCall()
;===============================================================
SingleStepJmpCall PROC
local rip_address:qword
local opcode:qword
local opcode_len:dword  
  push  rdi
  push  rsi
  sub rsp,58h
  ;lea rax,opcode
  mov opcode,0
  ;获取rip
  call GetCurrentRIP
  mov rip_address,rax

  ;读取8字节 call 命令最长7字节
  mov r9,8
  mov r8,8
  lea rdx,opcode
  mov rcx,rax
  call ReadDebugProcessMemory
  test rax,rax
  je EXIT_SINGLESTEPJMPCALL

;  ;换成大端
;  xor rax,rax
;  mov rdx,opcode
;  mov rcx,7
;LOOP_OVERTURN:
;  mov al,dl
;  shl rax,8
;  shr rdx,8
;loop LOOP_OVERTURN
;  mov opcode,rax

  ;判断是不是call
  mov rcx,opcode
  call IsCall 
  test eax,eax
  je SET_SINGLESTEP ;ret =0 不是call
  mov opcode_len,eax
SET_INT3_BREAK:
;设置软件断点
  mov rcx,rip_address
  xor rdx,rdx
  mov edx,opcode_len
  add rcx,rdx
  mov g_jmp_call_flag,TRUE
  mov g_jmp_call_address,rcx
  call SetInt3BreakPoint 
jmp EXIT_SINGLESTEPJMPCALL

SET_SINGLESTEP:
;设置单步
  mov g_step_flag,TRUE
  call SetSingelStep

EXIT_SINGLESTEPJMPCALL:
  add rsp,58h
  pop rsi
  pop rdi
  ret
SingleStepJmpCall ENDP

;===============================================================
;执行到返回
;===============================================================
SteptoNextReturn PROC
local rip_address:qword
local opcode:qword
local opcode_len:dword  
  push  rdi
  push  rsi
  sub rsp,58h
  ;lea rax,opcode
  mov opcode,0
  ;获取rip
  call GetCurrentRIP
  mov rip_address,rax

  ;读取8字节 call 命令最长7字节
  mov r9,8
  mov r8,8
  lea rdx,opcode
  mov rcx,rax
  call ReadDebugProcessMemory
  test rax,rax
  je EXIT_STEPTONEXTRETURN

  ;判断是不是call
  mov rcx,opcode
  call IsCall 
  test eax,eax
  je SET_SINGLESTEP ;ret =0 不是call
  mov opcode_len,eax
SET_INT3_BREAK:
;设置软件断点
  mov rcx,rip_address
  xor rdx,rdx
  mov edx,opcode_len
  add rcx,rdx
  mov g_step_to_ret_int3_flag,TRUE
  mov g_step_to_ret_int3_address,rcx
  call SetInt3BreakPoint 

jmp EXIT_STEPTONEXTRETURN

SET_SINGLESTEP:
;设置单步
  mov g_step_to_ret_step_flag,TRUE
  call SetSingelStep

EXIT_STEPTONEXTRETURN:
  add rsp,58h
  pop rsi
  pop rdi
  ret
SteptoNextReturn ENDP

;===============================================================
;获取命令行
;===============================================================

;获取命令行参数
GetCommandLine PROC
local str_length:qword
local cmd_ptr:ptr qword
local next_cmd_ptr:ptr qword
;local cmd_ptr_addition:ptr qword
local index:qword
local cmd_str[256]:byte

  push  rdi
  push  rsi
  sub rsp,60h

LOOP_GET_CMD@@:
  mov r8,256
  mov rdx,0
  lea rcx,cmd_str
  call memset
SCRIPT_CMD:
  cmp g_load_script_flag,TRUE
  jne USER_IMPORT_CMD
  ;需要执行脚本的命令
  ;获取一行命令
  mov r8,256
  lea rdx,cmd_str
  mov rcx,g_load_script_handle
  call ReadFileOneLine
  test rax,rax
  je USER_IMPORT_CMD
  jmp CMD_DISPOSE

USER_IMPORT_CMD:
  lea rcx,string_input_cmd_hint
  call printf
  ;stdin
  xor rcx,rcx
  call __acrt_iob_func
  mov r8,rax;FILE*
  ;fgets(szCmd, sizeof(szCmd), stdin);
  mov rdx,sizeof cmd_str
  lea rcx,cmd_str
  call fgets
  ; strlen(szCmd);
CMD_DISPOSE:
  lea rcx,cmd_str
  call strlen
  mov str_length,rax

  ;_strlwr_s(szCmd)
  mov rdx,str_length
  inc rdx
  lea rcx,cmd_str
  call _strlwr_s

  ;szCmd[nLen - 1] = '\0'; 去除fgets 的\n
  lea rax,cmd_str
  add rax,str_length
  mov r8b,byte ptr [rax-1]
  cmp r8b,0ah
  jne NOT_ENTER_FLAG
  mov byte ptr [rax-1],0
NOT_ENTER_FLAG:

;判断是否要记录指令
  cmp g_save_script_flag,TRUE
  jne CMD_EXECUTE
  ;需要记录
  lea rcx,cmd_str
  call strlen
  test eax,eax
  je CMD_EXECUTE
  mov r8d,eax
  lea rdx,cmd_str
  mov rcx,g_save_script_handle
  call WriteDataToFile
  test eax,eax
  je CMD_EXECUTE
  ;添加\r\n
  mov r8d,2
  lea rdx,string_line_feed
  mov rcx,g_save_script_handle
  call WriteDataToFile

  

  

CMD_EXECUTE:
  lea r8,next_cmd_ptr
  lea rdx,string_cmd_slice_symbol
  lea rcx,cmd_str
  call strtok_s
  cmp rax,0
  je EXIT_GETCOMMANDLINE
  mov cmd_ptr,rax
  ;sizeof(g_Commands) / sizeof(g_Commands[0]);
  mov r8,sizeof COMMAND
  mov rax,sizeof cmd_map
  cqo
  idiv r8
  mov index,rax
LOOP_TRAVERSAL_CMD:
  mov r8,index
  lea rax,[r8-1]
  mov rdx,sizeof COMMAND
  imul rax,rdx
  lea rdx,cmd_map
  lea rdi,[rdx+rax]
  mov rcx,[rdi+COMMAND.cmd]
  ;ptr == null
  cmp rcx,0
  je GET_NEXT_CMD@2
  mov rdx,cmd_ptr
  call strcmp
  xor eax,0
  je EXECUTE_CMD
GET_NEXT_CMD@2:
  dec index
  jne LOOP_TRAVERSAL_CMD
  lea rcx,string_cmd_error_symbol
  call printf
  ;重新获取指令 不在范围内的指令
  jmp LOOP_GET_CMD@@

 EXECUTE_CMD:
 ;pCmd + strlen(pCmd) + 1
  mov rax,next_cmd_ptr
  mov al,byte ptr[rax]
  cmp al,0
  jne ADDITION_STR_NOT_NULL
  mov rcx,0
  jmp CALL_COMMAND_PFN
ADDITION_STR_NOT_NULL:
  mov rcx,cmd_ptr
  call strlen
  inc rax
  mov rdx,cmd_ptr
  lea rcx,[rdx+rax]
CALL_COMMAND_PFN:
  mov rax,[rdi+COMMAND.pfn]
  call rax
  test eax,eax
  jne LOOP_GET_CMD@@

EXIT_GETCOMMANDLINE:
  mov rax,0
  add rsp,60h
  pop rsi
  pop rdi
  ret
GetCommandLine ENDP

;===============================================================
;获取当前RIP
;===============================================================

;获取当前RIP
;ret RIP rip_reg
GetCurrentRIP PROC
local thread_handle:HANDLE
local rip_reg:qword
local ctx:CONTEXT32
;local ctx2:CONTEXT
  push  rdi
  push  rsi
  ;ENTER 128,0
  ;sub rsp,96+6*8
  sub rsp,60h
  mov thread_handle,0
  mov rip_reg,0
  ;HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, DebugEvent.dwThreadId);
  mov r8d,debug_event.dwThreadId
  mov rdx,FALSE
  mov rcx,THREAD_ALL_ACCESS
  call OpenThread
  ;函数失败，则返回值为 NULL
  cmp rax,0
  je EXIT_GETCURRENTRIP
  mov thread_handle,rax
  ;mov rcx,sizeof CONTEXT
  ;call malloc
  ;mov ctx,rax
  mov r8,sizeof CONTEXT32
  mov rdx,0
  lea rcx,ctx
  call memset


  ;ctx.ContextFlags = CONTEXT_ALL;
  mov ctx.ContextFlags,WOW64_CONTEXT_ALL
  ;GetThreadContext(hThread, &ctx);
  lea rdx,ctx
  ;mov dword ptr[rax+CONTEXT.ContextFlags],CONTEXT_ALL
  ;mov rdx,ctx
  mov rcx,thread_handle
  call Wow64GetThreadContext
  ;函数失败，则返回值为零
  test rax,rax
  je GET_LAST_ERROR@32
  ;ctx.Rip
  xor rax,rax
  mov eax,ctx.Eip
  mov rip_reg,rax
  jmp CLOSE_THREAD@23
GET_LAST_ERROR@32:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_getctx_err
  call printf

CLOSE_THREAD@23:
  mov rcx,thread_handle
  call CloseHandle

EXIT_GETCURRENTRIP:
  mov rax,rip_reg
  ;leave 
  add rsp,60h
  pop rsi
  pop rdi
  ret
GetCurrentRIP ENDP

;===============================================================
;读取内存
;ReadMemory(void* lpAddress, void* lpBuf, SIZE_T read_size, SIZE_T buffer_size)
;===============================================================
ReadDebugProcessMemory PROC
local address:ptr qword
local buffer:ptr qword
local read_size:qword
local buffer_size:qword
local process_handle:HANDLE
local ret_read_size:qword 
local old_page:qword
  push rdi
  push rsi
  sub rsp,58h;
  mov address,rcx
  mov buffer,rdx
  mov read_size,r8
  mov buffer_size,r9
  mov ret_read_size,0
  ;if buffer_size < read_size
  cmp buffer_size,r8
  jb EXIT_READDEBUGPROCESSMEMORY
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  test rax,rax
  je EXIT_READDEBUGPROCESSMEMORY
  mov process_handle,rax

  ;lea rax,old_page
  ;push rax
  ;sub rsp,20h
  ;mov r9,PAGE_EXECUTE_READWRITE
  ;mov r8,read_size
  ;mov rdx,address
  ;mov rcx,process_handle
  ;call VirtualProtectEx
  ;add rsp,28h
  ;test eax,eax
  ;je EXIT_READDEBUGPROCESSMEMORY


  ;ReadProcessMemory
  lea rax,ret_read_size
  ;sub rsp,8
  push rax
  sub rsp,20h
  mov r9,read_size
  mov r8,buffer
  mov rdx,address
  mov rcx,process_handle
  call ReadProcessMemory
  add rsp,28h

  ;lea rax,old_page
  ;push rax
  ;sub rsp,20h
  ;mov r9,old_page
  ;mov r8,read_size
  ;mov rdx,address
  ;mov rcx,process_handle
  ;call VirtualProtectEx
  ;add rsp,28h
  ;test eax,eax
  ;je EXIT_READDEBUGPROCESSMEMORY

  mov rcx,process_handle
  call CloseHandle
  xor rax,rax

EXIT_READDEBUGPROCESSMEMORY:
  mov rax,ret_read_size
  add rsp,58h
  pop rsi
  pop rdi
  ret
ReadDebugProcessMemory ENDP

;===============================================================
;写入内存
;WriteMemory(void* lpAddress, void* lpBuf, SIZE_T nSize)
;===============================================================
;打开线程句柄并写入内存
WriteDebugProcessMemory PROC
local address:qword
local buffer:ptr qword
local write_size:qword
local process_handle:HANDLE
local ret_write_size:qword  
  sub rsp,58h;
  mov address,rcx
  mov buffer,rdx
  mov write_size,r8
  mov ret_write_size,0
  mov process_handle,0
  ;HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, DebugEvent.dwProcessId);
  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  test rax,rax
  je EXIT_WRITEDEBUGPROCESSMEMORY
  mov process_handle,rax
  ;WriteProcessMemory
  lea rax,ret_write_size
  ;sub rsp,8
  push rax
  sub rsp,20h
  mov r9,write_size
  mov r8,buffer
  mov rdx,address
  mov rcx,process_handle
  call WriteProcessMemory
  add rsp,28h
  mov rcx,process_handle
  call CloseHandle
  xor rax,rax
  
EXIT_WRITEDEBUGPROCESSMEMORY:
  mov rax,ret_write_size
  add rsp,58h
  ret
WriteDebugProcessMemory ENDP

;===============================================================
;获得线程上下文
;GetDebugThreadContext(LPCONTEXT lpCtx)
;ret bool
;===============================================================
;获得调试程序的线程的上下文
GetDebugThreadContext PROC
local thread_handle:HANDLE
local bool_ret:qword
local ctx:ptr CONTEXT32
;local ctx2:CONTEXT
  push  rdi
  push  rsi
  ;ENTER 128,0
  ;sub rsp,96+6*8
  sub rsp,58h
  mov ctx,rcx
  mov thread_handle,0
  mov bool_ret,0
  ;HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, DebugEvent.dwThreadId);
  mov r8d,debug_event.dwThreadId
  mov rdx,FALSE
  mov rcx,THREAD_ALL_ACCESS
  call OpenThread
  ;函数失败，则返回值为 NULL
  cmp rax,0
  je EXIT_GETDEBUGTHREADCONTEXT
  mov thread_handle,rax

  ;ctx.ContextFlags = CONTEXT_ALL;
  ;mov ctx.ContextFlags,CONTEXT_ALL
  ;GetThreadContext(hThread, &ctx);
  mov rdx,ctx
  mov rcx,thread_handle
  call Wow64GetThreadContext
  ;函数失败，则返回值为零
  test rax,rax
  je GET_LAST_ERROR@35
  mov bool_ret,TRUE
  jmp CLOSE_THREAD@6
GET_LAST_ERROR@35:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_getctx_err
  call printf

CLOSE_THREAD@6:
  mov rcx,thread_handle
  call CloseHandle

EXIT_GETDEBUGTHREADCONTEXT:
  mov rax,bool_ret
  add rsp,58h
  pop rsi
  pop rdi
  ret
GetDebugThreadContext ENDP


;===============================================================
;设置线程上下文
;SetDebugThreadContext(LPCONTEXT lpCtx)
;ret bool
;===============================================================
SetDebugThreadContext PROC
local thread_handle:HANDLE
local bool_ret:qword
local ctx:ptr CONTEXT32
;local ctx2:CONTEXT
  push  rdi
  push  rsi
  ;ENTER 128,0
  ;sub rsp,96+6*8
  sub rsp,58h
  mov ctx,rcx
  mov thread_handle,0
  mov bool_ret,0
  ;HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, DebugEvent.dwThreadId);
  mov r8d,debug_event.dwThreadId
  mov rdx,FALSE
  mov rcx,THREAD_ALL_ACCESS
  call OpenThread
  ;函数失败，则返回值为 NULL
  cmp rax,0
  je EXIT_SETDEBUGTHREADCONTEXT
  mov thread_handle,rax

  ;ctx.ContextFlags = CONTEXT_ALL;
  ;mov ctx.ContextFlags,CONTEXT_ALL
  ;GetThreadContext(hThread, &ctx);
  mov rdx,ctx
  mov rcx,thread_handle
  call Wow64SetThreadContext
  ;函数失败，则返回值为零
  test rax,rax
  je GET_LAST_ERROR@8
  mov bool_ret,TRUE
  jmp CLOSE_THREAD@9
GET_LAST_ERROR@8:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_getctx_err
  call printf

CLOSE_THREAD@9:
  mov rcx,thread_handle
  call CloseHandle

EXIT_SETDEBUGTHREADCONTEXT:
  mov rax,bool_ret
  add rsp,58h
  pop rsi
  pop rdi
  ret
SetDebugThreadContext ENDP


;===============================================================
;打开文件取得句柄
;HANDLE CreateFileGetHandle(PCHAR file_path, DWORD mode)
;ret HANDLE
;===============================================================
CreateFileGetHandle PROC
local file_path:qword
local file_handle:HANDLE
  push  rdi
  push  rsi
  sub rsp,60h
  mov file_path,NULL
  mov file_handle,NULL
  mov file_path,rcx
  mov rsi,rdx
  test rcx,rcx
  je EXIT_CREATEFILEGETHANDLE
  push NULL
  push FILE_ATTRIBUTE_NORMAL
  push rsi;OPEN_EXISTING
  sub rsp,20h
  mov r9,NULL
  mov r8,0
  mov rdx,GENERIC_READ
  or rdx,GENERIC_WRITE
  mov rcx,file_path
  call CreateFileA
  add rsp,38h
  cmp rax,INVALID_HANDLE_VALUE
  je ERROR
  mov file_handle,rax
  jmp EXIT_CREATEFILEGETHANDLE
ERROR:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_getctx_err
  call printf
EXIT_CREATEFILEGETHANDLE:
  mov rax,file_handle
  add rsp,60h
  pop rsi
  pop rdi
  ret
CreateFileGetHandle ENDP

;===============================================================
;从当前文件指针位置读取一行 \r\n
;BOOL ReadFileOneLine(HANDLE Handle, LPVOID buffer, DWORD bufferszie)
;ret 读取失败或者到行尾巴 0 否则为读取的大小
;===============================================================
ReadFileOneLine PROC
local handle:HANDLE
local buffer:ptr qword
local bufferszie:dword
local read_size:dword
local read_size_counter:dword
local lpDistanceToMoveHigh:dword
local one_byte:byte
local jmp_flag:byte
  push  rdi
  push  rsi
  sub rsp,58h
  mov handle,0
  mov buffer,0
  mov bufferszie,0
  mov read_size,0
  mov read_size_counter,0
  mov jmp_flag,FALSE
  mov handle,rcx
  mov buffer,rdx
  mov bufferszie,r8d
  test rcx,rcx
  je EXIT_READFILEONELINE
  test rdx,rdx
  je EXIT_READFILEONELINE
  test r8,r8
  je EXIT_READFILEONELINE
  mov rsi,buffer
LOOP_READ_FILE_BYTE:
  push NULL
  sub rsp,20h
  lea r9,read_size
  mov r8,1
  lea rdx,one_byte
  mov rcx,handle
  call ReadFile
  add rsp,28h
  test eax,eax
  je ERROR
  ;文件尾判断
  cmp eax,1
  jne NOT_FILE_EOF
  cmp read_size,0
  jne NOT_FILE_EOF
  ;到达文件尾 如果 lpOverlapped NULL，
  ;则在同步读取操作到达文件末尾时，ReadFile 返回 true true 并将 *lpNumberOfBytesRead 设置为零。
  mov rcx,g_load_script_handle
  call CloseHandle
  mov handle,0
  mov g_load_script_flag,FALSE
  jmp EXIT_READFILEONELINE
NOT_FILE_EOF:
  ;判断是不是\r \n 0dh,0ah
  cmp one_byte,0dh
  je IS_LINE_END_FLAG
  cmp one_byte,0ah
  je IS_LINE_END_FLAG
  cmp jmp_flag,TRUE
  je JMP_FLAG
  ;还是当前行
  mov al,one_byte;
  mov byte ptr[rsi],al
 
  inc read_size_counter
  ;判断缓冲区是否满了
  mov eax,bufferszie
  cmp read_size_counter,eax
  jge EXIT_READFILEONELINE

  inc rsi
  jmp LOOP_READ_FILE_BYTE
JMP_FLAG:
;读取到下一行了往回倒一个byte
  mov r9,FILE_CURRENT
  mov lpDistanceToMoveHigh,0FFFFFFFFh
  lea r8,lpDistanceToMoveHigh
  mov rdx,0FFFFFFFFh
  mov rcx,handle
  call SetFilePointer
  jmp EXIT_READFILEONELINE

IS_LINE_END_FLAG:
  ;碰到行尾
  mov jmp_flag,TRUE
  jmp LOOP_READ_FILE_BYTE

ERROR:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_read_write_err
  call printf

EXIT_READFILEONELINE:
  xor rax,rax
  mov eax,read_size_counter
  add rsp,58h
  pop rsi
  pop rdi
  ret
ReadFileOneLine ENDP

;===============================================================
;从当前文件指针位置写入数据
;BOOL WriteDataToFile(HANDLE Handle, LPVOID buffer, DWORD bufferszie)
;ret 写入失败 0 ,否则为写入的大小
;===============================================================
WriteDataToFile PROC
local buffer:ptr qword
local file_handle:HANDLE
local write_size:qword
local buffersize:dword

  push  rdi
  push  rsi
  sub rsp,60h
  mov buffer,0
  mov file_handle,0
  mov buffersize,0
  mov write_size,0
  mov file_handle,rcx
  mov buffer,rdx
  mov buffersize,r8d
  test rcx,rcx
  je EXIT_WRITEDATATOFILE
  test rdx,rdx
  je EXIT_WRITEDATATOFILE
  test r8d,r8d
  je EXIT_WRITEDATATOFILE
  push NULL
  sub rsp,20h
  lea r9,write_size
  xor r8,r8
  mov r8d,buffersize
  mov rdx,buffer
  mov rcx,file_handle
  call WriteFile
  add rsp,28h
  test eax,eax
  je ERROR
  jmp EXIT_WRITEDATATOFILE
ERROR:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_read_write_err
  call printf
EXIT_WRITEDATATOFILE:
  mov rax,write_size
  add rsp,60h
  pop rsi
  pop rdi
  ret
WriteDataToFile ENDP

;===============================================================
;获取进程PEB
;ULONG_PTR GetProcessPEB()
;ret 获取失败 0 ,否则为PEB point
;===============================================================
GetProcessPEB PROC
local pbi:PROCESS_BASIC_INFORMATION
local query_szie:qword
  push  rdi
  push  rsi
  sub rsp,58h



  xor r8,r8
  mov r8d,debug_event.dwProcessId
  mov rdx,FALSE
  mov rcx,PROCESS_ALL_ACCESS
  call OpenProcess
  test rax,rax
  je EXIT_GETPROCESSPEB
  mov rdi,rax

  lea rax,query_szie
  push rax
  sub rsp,20h
  mov r9,sizeof PROCESS_BASIC_INFORMATION
  lea r8,pbi
  mov rdx,ProcessBasicInformation
  xor rcx,rcx
  mov rcx,rdi
  call NtQueryInformationProcess  
  add rsp,28h
  ;NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)
  mov rcx,rdi
  call CloseHandle
  mov rax,0
  cmp edi,0
  jl EXIT_GETPROCESSPEB
  mov rax,pbi.PebBaseAddress
  add rax,1000h
EXIT_GETPROCESSPEB:
  add rsp,58h
  pop rsi
  pop rdi
  ret
GetProcessPEB ENDP

;===============================================================
;尝试获取函数名
;BOOL GetFunName(char* Handle, LPVOID buffer, DWORD bufferszie)
;ret 获取失败 0 ,否则为1
;===============================================================
GetFunName PROC
local buffer[256]:byte
local source_str:ptr qword
local new_str:ptr qword
local fun_ptr:qword
local ret_value:dword
local jmp_flag:dword
local index:dword
  push  rdi
  push  rsi
  sub rsp,58h
  mov source_str,0
  mov new_str,0
  mov ret_value,0
  mov source_str,rcx
  mov new_str,rdx
  test rcx,rcx
  je EXIT_GETFUNNAME
  test rdx,rdx
  je EXIT_GETFUNNAME
  ;判断是不是call
  mov r8,256
  mov rdx,0
  mov rcx,new_str
  call memset

  mov r8,6
  mov rdx,source_str
  mov rcx,new_str
  call memcpy

  lea rdx,g_get_fun_name_symbol
  mov rcx,new_str
  call strcmp
  test eax,eax
  jne CMP_CALL_SYMBOL2
  ;是"call [" 获取" [0xffffffffff]"
  jmp IS_CALL_1
CMP_CALL_SYMBOL2:

  mov r8,256
  mov rdx,0
  mov rcx,new_str
  call memset

  mov r8,5
  mov rdx,source_str
  mov rcx,new_str
  call memcpy

  lea rdx,g_get_fun_name_symbol2
  mov rcx,new_str
  call strcmp
  test eax,eax
  jne EXIT_GETFUNNAME
  ;是"call " 获取" 0xffffffffff"
  jmp IS_CALL_2

IS_CALL_1:
  mov jmp_flag,FALSE
  xor rsi,rsi
  mov rdi,source_str
  add rdi,8
LOOP_GET_NUMBER_LEN:
  mov al,byte ptr[rdi]
  cmp al,"]"
  je GET_NUMBER_STR
  inc rsi
  inc rdi
  jmp LOOP_GET_NUMBER_LEN

IS_CALL_2:
  mov jmp_flag,TRUE
  xor rsi,rsi
  mov rdi,source_str
  add rdi,7
LOOP_GET_NUMBER_LEN2:
  mov al,byte ptr[rdi]
  cmp al,0
  je GET_NUMBER_STR
  inc rsi
  inc rdi
  jmp LOOP_GET_NUMBER_LEN2



GET_NUMBER_STR:
  mov r8,256
  mov rdx,0
  lea rcx,buffer
  call memset

  mov r8,rsi
  mov rdx,source_str
  add rdx,8
  lea rcx,buffer
  call memcpy

  ;转换为数值
  mov r8,10h
  mov rdx,0
  lea rcx,buffer
  call strtoull
  mov fun_ptr,rax

  cmp jmp_flag,TRUE
  je SEARCH_FUN_NAME

  ;读取该地址
  mov r9,8
  mov r8,8
  lea rdx,fun_ptr
  mov rcx,fun_ptr
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_GETFUNNAME

SEARCH_FUN_NAME:
  lea rdx,buffer
  mov rcx,fun_ptr
  call FunAddressSearchName
  test eax,eax
  je EXIT_GETFUNNAME
  ;获取到字符串

  mov r8,256
  mov rdx,0
  mov rcx,new_str
  call memset


  cmp jmp_flag,TRUE
  je NOT_FALG_CALL
  ;是"call [ xxxx ]" 
  mov r8,6
  lea rdx,g_get_fun_name_symbol
  mov rcx,new_str
  call memcpy

  ;复制名称
  lea rcx,buffer
  call strlen
  mov rsi,rax

  xor r8,r8
  mov r8d,eax
  lea rdx,buffer
  mov rcx,new_str
  add rcx,6
  call memcpy

  mov rcx,new_str
  mov byte ptr [rcx+rsi+6],"]"
  mov ret_value,TRUE
  jmp EXIT_GETFUNNAME

 NOT_FALG_CALL:
  ;是"call  xxxx " 
  mov r8,5
  lea rdx,g_get_fun_name_symbol2
  mov rcx,new_str
  call memcpy

  ;复制名称
  lea rcx,buffer
  call strlen
  mov rsi,rax

  xor r8,r8
  mov r8d,eax
  lea rdx,buffer
  mov rcx,new_str
  add rcx,5
  call memcpy

  mov ret_value,TRUE
EXIT_GETFUNNAME:
  xor rax,rax
  mov eax,ret_value
  add rsp,58h
  pop rsi
  pop rdi
  ret
GetFunName ENDP


;===============================================================
;通过函数地址获取函数名称
;BOOL FunAddressSearchName(DWORD64 fun_address, char* buffer)
;ret 获取失败 0 ,否则为1
;===============================================================
FunAddressSearchName PROC
local buffer[256]:byte
local image_dos:IMAGE_DOS_HEADER
local image_nt:IMAGE_NT_HEADERS32
local image_export:IMAGE_EXPORT_DIRECTORY
local address_ptr:qword
local modulebase:qword
local export_start:qword
local export_end:qword
local fun_ptr:qword
local ret_buffer_ptr:qword
local ret_value:dword
local index:dword
local name_index:dword
  push  rdi
  push  rsi
  sub rsp,58h
  mov ret_value,0
  mov fun_ptr,rcx
  mov ret_buffer_ptr,rdx
  ;获取PEB
  call GetProcessPEB
  test rax,rax
  je EXIT_FUNADDRESSSEARCHNAME

  ;找到模块链表
  ;mov rax,gs:[60h]	;TEB+0x60->PEB 
  ;mov rax,[rax+18h]	;PEB+0x18->Ldr
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,rax
  add rcx,0ch
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;mov rsi,[rax+20h]	;Ldr+0x20->InMemoryOrderModuleList
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,address_ptr
  add rcx,14h
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;遍历模块 跳过自己
LOOP_READ_MODULE:
  ;InMemoryOrderModuleList->BLink
  ;mov rax,qword ptr [rsi]
  ;mov rdi,rax
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,address_ptr
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  mov rdi,address_ptr
 
  ;获取模块基址
  mov r9,4
  mov r8,4
  lea rdx,modulebase
  mov rcx,rdi
  add rcx,10h
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
   ;判断是不是空地址
  cmp modulebase,0
  je EXIT_FUNADDRESSSEARCHNAME

  ;获取DOS
  mov r9,sizeof IMAGE_DOS_HEADER
  mov r8,sizeof IMAGE_DOS_HEADER
  lea rdx,image_dos
  mov rcx,modulebase
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;获取NT
  mov r9,sizeof IMAGE_NT_HEADERS32
  mov r8,sizeof IMAGE_NT_HEADERS32
  lea rdx,image_nt
  xor rcx,rcx
  mov ecx,image_dos.e_lfanew
  add rcx,modulebase
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME

  ;得到导出表
  mov r9,sizeof IMAGE_EXPORT_DIRECTORY
  mov r8,sizeof IMAGE_EXPORT_DIRECTORY
  lea rdx,image_export
  xor rcx,rcx
  mov ecx,image_nt.OptionalHeader.DataDirectory.VirtualAddress
  add rcx,modulebase
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;计算节区范围
  xor rax,rax
  mov eax,image_nt.OptionalHeader.DataDirectory.VirtualAddress
  add rax,modulebase
  mov export_start,rax
  xor rdx,rdx
  mov edx,image_nt.OptionalHeader.DataDirectory.dataSize
  add rax,rdx
  mov export_end,rax
  ;获取导出函数地址表起始指针
  xor rsi,rsi
  mov esi,image_export.AddressOfFunctions
  add rsi,modulebase

  ;mov eax,image_export.NumberOfFunctions
  ;mov index,eax
  mov index,0
LOOP_CMP_FUN_ADDRESS:
  ;一项大小DWORD

  ;获取一项 address rva
  mov address_ptr,0
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,rsi
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;对比是否是转发函数
  mov rax,address_ptr
  add rax,modulebase
  mov address_ptr,rax

  cmp rax,export_start
  jl CMP_FUN_VA
  cmp rax,export_end
  jg CMP_FUN_VA
  ;转发函数
  jmp GET_MNEXT_FUN_RVA
CMP_FUN_VA:
  ;对比是否是该函数
  mov rax,address_ptr
  ;add rax,modulebase
  cmp rax,fun_ptr
  jne GET_MNEXT_FUN_RVA
  ;是该函数 通过序号在导出名称序号表中找到对应名称的序号

  ;获取导出名称序号地址表起始指针
  xor rsi,rsi
  mov esi,image_export.AddressOfNameOrdinals
  add rsi,modulebase
  mov eax,image_export.Base
  ;add index,eax ;导出序号减去base就是函数的索引 
  mov name_index,0
LOOP_READ_NAME_ORDINALS:

 ;获取一项 nane Ordinals 
  mov address_ptr,0
  mov r9,2
  mov r8,2
  lea rdx,address_ptr
  mov rcx,rsi
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  xor rax,rax
  mov eax,index
  cmp address_ptr,rax
  jne GET_NEXT_NAME_ORDINALS
  ;通过名称序号找到名称
  ;获取导出名称地址表对应指针
  xor rax,rax
  mov eax,image_export.AddressOfNames
  add rax,modulebase
  xor rcx,rcx
  mov ecx,name_index
  ;dec ecx
  lea rsi,[rax+rcx*4]
  ;读取地址获取名称rva
  mov address_ptr,0
  mov r9,4
  mov r8,4
  lea rdx,address_ptr
  mov rcx,rsi
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;获取名称
  mov r8,256
  mov rdx,0
  lea rcx,buffer
  call memset

  mov r9,256
  mov r8,255
  lea rdx,buffer
  mov rcx,modulebase
  add rcx,address_ptr
  call ReadDebugProcessMemory
  test eax,eax
  je EXIT_FUNADDRESSSEARCHNAME
  ;复制字符串
  lea rcx,buffer
  call strlen

  xor r8,r8
  mov r8d,eax
  lea rdx,buffer
  mov rcx,ret_buffer_ptr
  call memcpy
  
  mov ret_value,TRUE
jmp EXIT_FUNADDRESSSEARCHNAME

GET_NEXT_NAME_ORDINALS:
  inc name_index
  add rsi,2
  mov eax,image_export.NumberOfNames
  cmp name_index,eax
  jl LOOP_READ_NAME_ORDINALS

  jmp EXIT_FUNADDRESSSEARCHNAME
;获取下一个函数地址
GET_MNEXT_FUN_RVA:
  inc index
  add rsi,4
  mov eax,image_export.NumberOfFunctions
  cmp index,eax
  jl LOOP_CMP_FUN_ADDRESS
  
GET_NEXT_MODULEBASE:
  ;获取下一个模块
  mov address_ptr,rdi
  jmp LOOP_READ_MODULE

EXIT_FUNADDRESSSEARCHNAME:
  xor rax,rax
  mov eax,ret_value
  add rsp,58h
  pop rsi
  pop rdi
  ret
FunAddressSearchName ENDP