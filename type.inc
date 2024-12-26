;.386
;.model flat, stdcall
option casemap:none


;===============================================================
;typedef
FILE TYPEDEF ptr qword  
BOOL TYPEDEF qword      
HANDLE TYPEDEF ptr qword
LPSTR TYPEDEF ptr qword
LPBYTE TYPEDEF ptr qword
PVOID TYPEDEF ptr qword
LPVOID TYPEDEF ptr qword
ULONG_PTR TYPEDEF ptr qword
ULONGLONG TYPEDEF qword
LONGLONG TYPEDEF qword
DWORD64 TYPEDEF qword
;===============================================================
;struct

IMAGE_DOS_HEADER struct               ;// DOS .EXE header
  e_magic    WORD  0                  ;// Magic number
  e_cblp     WORD  0                  ;// Bytes on last page of file
  e_cp       WORD  0                  ;// Pages in file
  e_crlc     WORD  0                  ;// Relocations
  e_cparhdr  WORD  0                  ;// Size of header in paragraphs
  e_minalloc WORD  0                  ;// Minimum extra paragraphs needed
  e_maxalloc WORD  0                  ;// Maximum extra paragraphs needed
  e_ss       WORD  0                  ;// Initial (relative) SS value
  e_sp       WORD  0                  ;// Initial SP value
  e_csum     WORD  0                  ;// Checksum
  e_ip       WORD  0                  ;// Initial IP value
  e_cs       WORD  0                  ;// Initial (relative) CS value
  e_lfarlc   WORD  0                  ;// File address of relocation table
  e_ovno     WORD  0                  ;// Overlay number
  e_res      WORD  4 dup(0)           ;// Reserved words
  e_oemid    WORD  0                  ;// OEM identifier (for e_oeminfo)
  e_oeminfo  WORD  0                  ;// OEM information; e_oemid specific
  e_res2     WORD  10 dup(0)          ;// Reserved words
  e_lfanew   DWORD 0                  ;// File address of new exe header
IMAGE_DOS_HEADER ends

IMAGE_FILE_HEADER struct
  Machine               WORD   0 
  NumberOfSections      WORD   0
  TimeDateStamp         DWORD  0
  PointerToSymbolTable  DWORD  0
  NumberOfSymbols       DWORD  0
  SizeOfOptionalHeader  WORD   0
  Characteristics       WORD   0
IMAGE_FILE_HEADER ends

IMAGE_DATA_DIRECTORY struct
   VirtualAddress   DWORD 0
   dataSize         DWORD 0  
IMAGE_DATA_DIRECTORY ends

IMAGE_OPTIONAL_HEADER64 struct
  Magic                                 WORD    0    
  MajorLinkerVersion                    BYTE    0    
  MinorLinkerVersion                    BYTE    0    
  SizeOfCode                            DWORD   0    
  SizeOfInitializedData                 DWORD   0    
  SizeOfUninitializedData               DWORD   0    
  AddressOfEntryPoint                   DWORD   0    
  BaseOfCode                            DWORD   0    
  ImageBase                             QWORD   0 
  SectionAlignment                      DWORD   0    
  FileAlignment                         DWORD   0    
  MajorOperatingSystemVersion           WORD    0    
  MinorOperatingSystemVersion           WORD    0    
  MajorImageVersion                     WORD    0    
  MinorImageVersion                     WORD    0    
  MajorSubsystemVersion                 WORD    0    
  MinorSubsystemVersion                 WORD    0    
  Win32VersionValue                     DWORD   0    
  SizeOfImage                           DWORD   0    
  SizeOfHeaders                         DWORD   0    
  CheckSum                              DWORD   0    
  Subsystem                             WORD    0    
  DllCharacteristics                    WORD    0    
  SizeOfStackReserve                    QWORD   0
  SizeOfStackCommit                     QWORD   0
  SizeOfHeapReserve                     QWORD   0
  SizeOfHeapCommit                      QWORD   0
  LoaderFlags                           DWORD   0    
  NumberOfRvaAndSizes                   DWORD   0    
  DataDirectory IMAGE_DATA_DIRECTORY    16 dup(<0>)
IMAGE_OPTIONAL_HEADER64 ends

IMAGE_OPTIONAL_HEADER32 struct
  Magic                                 WORD    0    
  MajorLinkerVersion                    BYTE    0    
  MinorLinkerVersion                    BYTE    0    
  SizeOfCode                            DWORD   0    
  SizeOfInitializedData                 DWORD   0    
  SizeOfUninitializedData               DWORD   0    
  AddressOfEntryPoint                   DWORD   0    
  BaseOfCode                            DWORD   0  
  BaseOfData                            DWORD   0 
  ImageBase                             DWORD   0 
  SectionAlignment                      DWORD   0    
  FileAlignment                         DWORD   0    
  MajorOperatingSystemVersion           WORD    0    
  MinorOperatingSystemVersion           WORD    0    
  MajorImageVersion                     WORD    0    
  MinorImageVersion                     WORD    0    
  MajorSubsystemVersion                 WORD    0    
  MinorSubsystemVersion                 WORD    0    
  Win32VersionValue                     DWORD   0    
  SizeOfImage                           DWORD   0    
  SizeOfHeaders                         DWORD   0    
  CheckSum                              DWORD   0    
  Subsystem                             WORD    0    
  DllCharacteristics                    WORD    0    
  SizeOfStackReserve                    DWORD   0
  SizeOfStackCommit                     DWORD   0
  SizeOfHeapReserve                     DWORD   0
  SizeOfHeapCommit                      DWORD   0
  LoaderFlags                           DWORD   0    
  NumberOfRvaAndSizes                   DWORD   0    
  DataDirectory IMAGE_DATA_DIRECTORY    16 dup(<0>)
IMAGE_OPTIONAL_HEADER32 ends

IMAGE_NT_HEADERS64 struct
  Signature DWORD 0
  FileHeader IMAGE_FILE_HEADER <0>
  OptionalHeader IMAGE_OPTIONAL_HEADER64 <0>
IMAGE_NT_HEADERS64 ends

IMAGE_NT_HEADERS32 struct
  Signature DWORD 0
  FileHeader IMAGE_FILE_HEADER <0>
  OptionalHeader IMAGE_OPTIONAL_HEADER32 <0>
IMAGE_NT_HEADERS32 ends

STARTUPINFOA struct
  cb               DWORD  0
                   DWORD  0
  lpReserved       LPSTR  0
  lpDesktop        LPSTR  0
  lpTitle          LPSTR  0
  dwX              DWORD  0
  dwY              DWORD  0
  dwXSize          DWORD  0
  dwYSize          DWORD  0
  dwXCountChars    DWORD  0
  dwYCountChars    DWORD  0
  dwFillAttribute  DWORD  0
  dwFlags          DWORD  0
  wShowWindow      WORD   0
  cbReserved2      WORD   3 dup(0)
  lpReserved2      LPBYTE 0
  hStdInput        HANDLE 0
  hStdOutput       HANDLE 0
  hStdError        HANDLE 0
STARTUPINFOA ends


STARTUPINFO TYPEDEF STARTUPINFOA

STARTUPINFOA32 struct
  cb               DWORD  0
                   DWORD  0
  lpReserved       LPSTR  0
  lpDesktop        LPSTR  0
  lpTitle          LPSTR  0
  dwX              DWORD  0
  dwY              DWORD  0
  dwXSize          DWORD  0
  dwYSize          DWORD  0
  dwXCountChars    DWORD  0
  dwYCountChars    DWORD  0
  dwFillAttribute  DWORD  0
  dwFlags          DWORD  0
  wShowWindow      WORD   0
  cbReserved2      WORD   0
  lpReserved2      LPBYTE 0
  hStdInput        HANDLE 0
  hStdOutput       HANDLE 0
  hStdError        HANDLE 0
STARTUPINFOA32 ends

STARTUPINFO32 TYPEDEF STARTUPINFOA32

PROCESS_INFORMATION struct
  hProcess      HANDLE  0
  hThread       HANDLE  0
  dwProcessId   DWORD   0
  dwThreadId    DWORD   0
PROCESS_INFORMATION ends

EXCEPTION_RECORD struct
  ExceptionCode         DWORD 0
  ExceptionFlags        DWORD 0
  ExceptionRecord       PVOID 0
  ExceptionAddress      PVOID 0
  NumberParameters      DWORD 0
  DWORD 0
  ExceptionInformation  ULONG_PTR EXCEPTION_MAXIMUM_PARAMETERS dup(0)
EXCEPTION_RECORD ends

EXCEPTION_DEBUG_INFO struct
   ExceptionRecord EXCEPTION_RECORD <0>
   dwFirstChance   DWORD 0
   DWORD 0
EXCEPTION_DEBUG_INFO ends

CREATE_THREAD_DEBUG_INFO struct
  hThread             HANDLE  0
  lpThreadLocalBase   LPVOID  0
  lpStartAddress      LPVOID  0
CREATE_THREAD_DEBUG_INFO ends

CREATE_PROCESS_DEBUG_INFO struct
  hFile                                   HANDLE    0
  hProcess                                HANDLE    0
  hThread                                 HANDLE    0
  lpBaseOfImage                           LPVOID    0
  dwDebugInfoFileOffset                   DWORD     0
  nDebugInfoSize                          DWORD     0
  lpThreadLocalBase                       LPVOID    0
  lpStartAddress                          LPVOID    0
  lpImageName                             LPVOID    0  
  fUnicode                                WORD      0
  WORD  3 dup(0)
CREATE_PROCESS_DEBUG_INFO ends

EXIT_THREAD_DEBUG_INFO struct
  dwExitCode WORD  0
EXIT_THREAD_DEBUG_INFO ends

EXIT_PROCESS_DEBUG_INFO struct
  dwExitCode WORD  0
EXIT_PROCESS_DEBUG_INFO ends

LOAD_DLL_DEBUG_INFO struct
  hFile                 HANDLE 0 
  lpBaseOfDll           LPVOID 0
  dwDebugInfoFileOffset DWORD  0
  nDebugInfoSize        DWORD  0
  lpImageName           LPVOID 0
  fUnicode              WORD   0
  WORD  3 dup(0)
LOAD_DLL_DEBUG_INFO ends

UNLOAD_DLL_DEBUG_INFO struct
  lpBaseOfDll LPVOID 0
UNLOAD_DLL_DEBUG_INFO ends

OUTPUT_DEBUG_STRING_INFO struct
  lpDebugStringData   LPVOID  0
  fUnicode            WORD    0
  nDebugStringLength  WORD    0
OUTPUT_DEBUG_STRING_INFO ends

RIP_INFO struct
  dwError  DWORD 0
  dwType   DWORD 0
RIP_INFO ends

DEBUG_EVENT struct
  dwDebugEventCode  DWORD 0
  dwProcessId       DWORD 0
  dwThreadId        DWORD 0
  DWORD 0
  union u
   Exception            EXCEPTION_DEBUG_INFO      <>
   CreateThread         CREATE_THREAD_DEBUG_INFO  <>
   CreateProcessInfo    CREATE_PROCESS_DEBUG_INFO <>
   ExitThread           EXIT_THREAD_DEBUG_INFO    <>
   ExitProcess          EXIT_PROCESS_DEBUG_INFO   <>
   LoadDll              LOAD_DLL_DEBUG_INFO       <>
   UnloadDll            UNLOAD_DLL_DEBUG_INFO     <>
   DebugString          OUTPUT_DEBUG_STRING_INFO  <>
   RipInfo              RIP_INFO                  <>
  ends
DEBUG_EVENT ends

EXCEPTION_RECORD32 struct
  ExceptionCode         DWORD 0
  ExceptionFlags        DWORD 0
  ExceptionRecord       DWORD 0
  ExceptionAddress      DWORD 0
  NumberParameters      DWORD 0
  ExceptionInformation  DWORD EXCEPTION_MAXIMUM_PARAMETERS dup(0)
EXCEPTION_RECORD32 ends

EXCEPTION_DEBUG_INFO32 struct
   ExceptionRecord EXCEPTION_RECORD32 <0>
   dwFirstChance   DWORD 0
EXCEPTION_DEBUG_INFO32 ends

CREATE_THREAD_DEBUG_INFO32 struct
  hThread             DWORD  0
  lpThreadLocalBase   DWORD  0
  lpStartAddress      DWORD  0
CREATE_THREAD_DEBUG_INFO32 ends

CREATE_PROCESS_DEBUG_INFO32 struct
  hFile                                   DWORD     0
  hProcess                                DWORD     0
  hThread                                 DWORD     0
  lpBaseOfImage                           DWORD     0
  dwDebugInfoFileOffset                   DWORD     0
  nDebugInfoSize                          DWORD     0
  lpThreadLocalBase                       DWORD     0
  lpStartAddress                          DWORD     0
  lpImageName                             DWORD     0  
  fUnicode                                WORD      0
  WORD  1 dup(0)
CREATE_PROCESS_DEBUG_INFO32 ends

EXIT_THREAD_DEBUG_INFO32 struct
  dwExitCode DWORD  0
EXIT_THREAD_DEBUG_INFO32 ends

EXIT_PROCESS_DEBUG_INFO32 struct
  dwExitCode WORD  0
EXIT_PROCESS_DEBUG_INFO32 ends

LOAD_DLL_DEBUG_INFO32 struct
  hFile                 DWORD  0
  lpBaseOfDll           DWORD  0
  dwDebugInfoFileOffset DWORD  0
  nDebugInfoSize        DWORD  0
  lpImageName           DWORD  0
  fUnicode              WORD   0
  WORD  1 dup(0)
LOAD_DLL_DEBUG_INFO32 ends

UNLOAD_DLL_DEBUG_INFO32 struct
  lpBaseOfDll DWORD  0
UNLOAD_DLL_DEBUG_INFO32 ends

OUTPUT_DEBUG_STRING_INFO32 struct
  lpDebugStringData   DWORD  0
  fUnicode            WORD    0
  nDebugStringLength  WORD    0
OUTPUT_DEBUG_STRING_INFO32 ends

DEBUG_EVENT32 struct
  dwDebugEventCode  DWORD 0
  dwProcessId       DWORD 0
  dwThreadId        DWORD 0
  union u
   Exception            EXCEPTION_DEBUG_INFO32      <>
   CreateThread         CREATE_THREAD_DEBUG_INFO32  <>
   CreateProcessInfo    CREATE_PROCESS_DEBUG_INFO32 <>
   ExitThread           EXIT_THREAD_DEBUG_INFO32    <>
   ExitProcess          EXIT_PROCESS_DEBUG_INFO32   <>
   LoadDll              LOAD_DLL_DEBUG_INFO32       <>
   UnloadDll            UNLOAD_DLL_DEBUG_INFO32     <>
   DebugString          OUTPUT_DEBUG_STRING_INFO32  <>
   RipInfo              RIP_INFO                    <>
  ends
DEBUG_EVENT32 ends



M128A struct
  Low@   ULONGLONG 0
  High@  LONGLONG  0
M128A ends

XSAVE_FORMAT struct
  ControlWord          WORD   0
  StatusWord           WORD   0
  TagWord              BYTE   0
  Reserved1            BYTE   0
  ErrorOpcode          WORD   0
  ErrorOffset          DWORD  0
  ErrorSelector        WORD   0
  Reserved2            WORD   0
  DataOffset           DWORD  0
  DataSelector         WORD   0
  Reserved3            WORD   0
  MxCsr                DWORD  0
  MxCsr_Mask           DWORD  0
  FloatRegisters       M128A  8 dup(<0>)
  XmmRegisters         M128A  16 dup(<0>)
  Reserved4            BYTE   96 dup(<0>)
XSAVE_FORMAT ends

XMM_SAVE_AREA32 TYPEDEF XSAVE_FORMAT

CONTEXT struct
  P1Home          DWORD64   0
  P2Home          DWORD64   0
  P3Home          DWORD64   0
  P4Home          DWORD64   0
  P5Home          DWORD64   0
  P6Home          DWORD64   0

  ContextFlags    DWORD     0
  MxCsr           DWORD     0

  SegCs           WORD      0
  SegDs           WORD      0
  SegEs           WORD      0
  SegFs           WORD      0
  SegGs           WORD      0
  SegSs           WORD      0
  EFlags          DWORD     0

  Dr0@            DWORD64   0
  Dr1@            DWORD64   0
  Dr2@            DWORD64   0
  Dr3@            DWORD64   0
  Dr6@            DWORD64   0
  Dr7@            DWORD64   0

  Rax@            DWORD64   0
  Rcx@            DWORD64   0
  Rdx@            DWORD64   0
  Rbx@            DWORD64   0
  Rsp@            DWORD64   0
  Rbp@            DWORD64   0
  Rsi@            DWORD64   0
  Rdi@            DWORD64   0
  R8@             DWORD64   0
  R9@             DWORD64   0
  R10@            DWORD64   0
  R11@            DWORD64   0
  R12@            DWORD64   0
  R13@            DWORD64   0
  R14@            DWORD64   0
  R15@            DWORD64   0

  Rip@            DWORD64   0

  union DUMMYUNIONNAME
    FltSave       XMM_SAVE_AREA32 <0>
      struct DUMMYSTRUCTNAME
        Header          M128A 2 dup(<0>)
        Legacy          M128A 8 dup(<0>)
        Xmm0@           M128A <0>
        Xmm1@           M128A <0>
        Xmm2@           M128A <0>
        Xmm3@           M128A <0>
        Xmm4@           M128A <0>
        Xmm5@           M128A <0>
        Xmm6@           M128A <0>
        Xmm7@           M128A <0>
        Xmm8@           M128A <0>
        Xmm9@           M128A <0>
        Xmm10@          M128A <0>
        Xmm11@          M128A <0>
        Xmm12@          M128A <0>
        Xmm13@          M128A <0>
        Xmm14@          M128A <0>
        Xmm15@          M128A <0>
      ends
  ends

  VectorRegister  M128A   26 dup(<0>)      
  VectorControl   DWORD64 0

  DebugControl           DWORD64    0
  LastBranchToRip        DWORD64    0
  LastBranchFromRip      DWORD64    0
  LastExceptionToRip     DWORD64    0
  LastExceptionFromRip   DWORD64    0

CONTEXT ends

CONTEXT32 struct
  ContextFlags DWORD 0
  Dr0@    DWORD   0
  Dr1@    DWORD   0
  Dr2@    DWORD   0
  Dr3@    DWORD   0
  Dr6@    DWORD   0
  Dr7@    DWORD   0
  FloatSave byte 112 dup(0)
  SegGs   DWORD   0
  SegFs   DWORD   0
  SegEs   DWORD   0
  SegDs   DWORD   0
  Edi@    DWORD   0
  Esi@    DWORD   0
  Ebx@    DWORD   0
  Edx@    DWORD   0
  Ecx@    DWORD   0
  Eax@    DWORD   0
  Ebp@    DWORD   0
  Eip     DWORD   0
  SegCs   DWORD   0
  EFlags  DWORD   0
  Esp@    DWORD   0
  SegSs   DWORD   0
  ExtendedRegisters byte 512 dup(0)
CONTEXT32 ends


MEMORY_BASIC_INFORMATION struct
  BaseAddress         PVOID 0
  AllocationBase      PVOID 0
  AllocationProtect   DWORD 0
  PartitionId         WORD  0
  WORD 1 dup(0)
  RegionSize          QWORD 0
  State               DWORD 0
  Protect             DWORD 0
  Type@               DWORD 0
  DWORD 0
MEMORY_BASIC_INFORMATION ends

PROCESS_BASIC_INFORMATION struct
    ExitStatus                   ULONG_PTR 0
    PebBaseAddress               ULONG_PTR 0
    AffinityMask                 ULONG_PTR 0
    BasePriority                 ULONG_PTR 0
    UniqueProcessId              ULONG_PTR 0
    InheritedFromUniqueProcessId ULONG_PTR 0
PROCESS_BASIC_INFORMATION ends

PROCESS_BASIC_INFORMATION32 struct
    ExitStatus                   DWORD 0
    PebBaseAddress               DWORD 0
    AffinityMask                 DWORD 0
    BasePriority                 DWORD 0
    UniqueProcessId              DWORD 0
    InheritedFromUniqueProcessId DWORD 0
PROCESS_BASIC_INFORMATION32 ends

UNICODE_STRING struct
    Length@  word 0
    MaximumLength word 0
    byte 4 dup(0)
    Buffer ULONG_PTR 0
UNICODE_STRING ends

UNICODE_STRING32 struct
    Length@  word 0
    MaximumLength word 0
    Buffer DWORD 0
UNICODE_STRING32 ends

IMAGE_SECTION_HEADER struct
  Name@ BYTE 8 dup(0)
  union Misc
    PhysicalAddress  DWORD  0
    VirtualSize      DWORD  0
  ends
  VirtualAddress        DWORD   0
  SizeOfRawData         DWORD   0
  PointerToRawData      DWORD   0
  PointerToRelocations  DWORD   0
  PointerToLinenumbers  DWORD   0
  NumberOfRelocations   WORD    0
  NumberOfLinenumbers   WORD    0
  Characteristics       DWORD   0
IMAGE_SECTION_HEADER ends

IMAGE_EXPORT_DIRECTORY struct
  Characteristics       DWORD   0
  TimeDateStamp         DWORD   0
  MajorVersion          WORD    0
  MinorVersion          WORD    0
  Name@                 DWORD   0
  Base                  DWORD   0
  NumberOfFunctions     DWORD   0
  NumberOfNames         DWORD   0
  AddressOfFunctions    DWORD   0
  AddressOfNames        DWORD   0
  AddressOfNameOrdinals DWORD   0
IMAGE_EXPORT_DIRECTORY ends
;===============================================================
;寄存器 结构


;===============================================================
;Zydis 结构
ZydisDecodedInstructionAvx struct
  vector_length word  0
  word  0
  struct mask@
    mode  dword  0
    reg   dword  0
  ends
  struct broadcast
    is_static   byte  0
    byte 3 dup (0)
    mode        dword 0
  ends
  struct rounding
    mode        dword 0
  ends
  struct swizzle
    mode        dword 0
  ends
  struct conversion
    mode        dword 0
  ends
  has_sae             byte  0
  has_eviction_hint   byte  0
  word 0
ZydisDecodedInstructionAvx ends

ZydisDecodedInstructionMeta struct
  category          dword 0
  isa_set           dword 0
  isa_ext           dword 0
  branch_type       dword 0
  exception_class   dword 0
ZydisDecodedInstructionMeta ends

ZydisDecodedInstructionRaw struct
  prefix_count  byte 0
  byte 7 dup(0)
  prefixes      qword  15 dup(0)
  encoding2     dword  0
  dword  0
  unionstruct@  qword  0
  modrm         dword  0
  sib           dword  0
  disp          qword  2  dup(0)
  imm           qword  6  dup(0)
ZydisDecodedInstructionRaw ends

ZydisDecodedInstruction struct
  machine_mode  dword 0
  mnemonic      dword 0
  length@       byte  0
  byte 3 dup(0)
  encoding      dword 0
  opcode_map    dword 0
  opcode        byte  0
  stack_width   byte  0
  operand_width byte  0
  address_width byte  0
  operand_count byte  0
  operand_count_visible byte  0
  byte 6 dup(0)
  attributes    qword   0
  cpu_flags     qword   0
  fpu_flags     qword   0
  avx           ZydisDecodedInstructionAvx  <0>
  meta          ZydisDecodedInstructionMeta <0>
  raw           ZydisDecodedInstructionRaw  <0>
ZydisDecodedInstruction ends

ZydisDisassembledInstruction  struct
  runtime_address   qword 0
  info              ZydisDecodedInstruction <0>
  operands          qword 100 dup(0)
  text             byte  96  dup(0)
ZydisDisassembledInstruction  ends

;===============================================================
;user-defined struct

;断点结构
BreakPointInfo struct
  break_address PVOID 0
  old_code      byte  0
  efficacious   byte  0
  using         byte  0
  byte 5 dup(0) ;对齐
BreakPointInfo ends

;命令保存结构
COMMAND struct
  cmd PVOID  0
  pfn PVOID  0
COMMAND ends

;内存断点结构
MemBreakPointInfo struct
  break_address PVOID 0
  page_base     qword 0
  old_type      dword 0
  bp_length     dword 0
  rxw_type      dword 0
  dword 0
MemBreakPointInfo ends
