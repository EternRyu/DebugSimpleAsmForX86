
option casemap:none
include debug_core.inc



.data
g_filepath db "TestDebug.exe",0
;g_filepath db "dumptestPE.exe",0
;g_filepath db "HELLOWRDMSG.exe",0


;g_test db "%s\r\n",0
string_create_err db "[-]Create Debug Process Failed! ErrorCode:%d",0dh,0ah,0
string_create_success db"[+]Create Debug Process Success!",0dh,0ah,0
string_pause db "pause",0

.code
main PROC
local startup_info:STARTUPINFO
local process_info:PROCESS_INFORMATION
  push rdi
  push rsi
  ;sub rsp,8;栈平衡
  call InitCMDMap
  ;IsPE64(g_filepath)
  ;lea rcx,g_filepath
  ;call IsPE64
  ;;if not pe64 (ret != 1 ; goto ret )
  ;test rax,1
  ;jne EXIT_MAIN ;
  
  lea rax,process_info
  mov rdi,rax 
  xor eax,eax 
  ;mov ecx,sizeof IMAGE_OPTIONAL_HEADER32
  rep stos byte ptr [rdi]
  
  lea rax,startup_info
  mov rdi,rax  
  xor eax,eax  
  mov ecx,sizeof STARTUPINFO  
  rep stos byte ptr [rdi]
  mov rax,sizeof STARTUPINFO
  mov startup_info.cb, eax
  

  ;系统接收异常 -> 调试器接收异常 -> 筛选器接收异常
  ;以调试模式启动进程
  ;附加进程
  
  

  lea rax,process_info
  push rax  ;lpProcessInformation
  ;mov qword ptr [rsp+48h],rax

  lea rax,startup_info
  push rax  ;lpStartupInfo
  ;mov qword ptr [rsp+40h],rax
  push NULL ;lpCurrentDirectory
  ;mov qword ptr [rsp+38h],NULL
  push NULL ;lpEnvironment
  ;mov qword ptr [rsp+30h],NULL
  push DEBUG_PROCESS  ;dwCreationFlags
  ;mov qword ptr [rsp+28h],DEBUG_PROCESS
  push FALSE ;bInheritHandles
  ;mov qword ptr [rsp+20h],0
  sub rsp,20h;栈平衡
  
  mov r9,NULL ;lpThreadAttributes
  mov r8,NULL ;lpProcessAttributes
  mov rdx,NULL  ;lpCommandLine
  lea rcx,g_filepath  ;lpApplicationName
  call CreateProcessA
  add rsp,50h
  cmp eax,0
  je LAST_ERROR
  lea rcx,string_create_success
  call printf

  ;DBG_EXCEPTION_NOT_HANDLED 调试器不处理异常
  ;DBG_CONTINUE 调试器处理异常
  call DistributeDebugEvent

  mov rcx,process_info.hProcess
  call CloseHandle
  mov rcx,process_info.hThread
  call CloseHandle

  jmp EXIT_MAIN

LAST_ERROR:
  call GetLastError
  xor rdx,rdx
  mov edx,eax
  lea rcx,string_create_err
  call printf
  lea rcx,string_pause
  call system
EXIT_MAIN:
  ;add rsp,8;栈平衡
  pop rsi
  pop rdi
  ret
main ENDP

END