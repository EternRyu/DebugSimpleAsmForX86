# DebugSimpleAsm
用64位进程的调试器调试x86的程序(Wow64)</br>
## 核心
用64位进程调试兼容x86程序时不能使用常规异常值！！！</br>
EXCEPTION_BREAKPOINT EQU 080000003h</br>
EXCEPTION_SINGLE_STEP EQU 080000004h</br>

需要使用的是：</br>
兼容x86的异常断点 STATUS_WX86_BREAKPOINT EQU 4000001Fh</br>
兼容x86的异常单步 STATUS_WX86_SINGLE_STEP EQU 4000001Eh</br>
MSDN相关连接：</br>
https://learn.microsoft.com/en-us/openspecs/windows_protocols/ms-erref/596a1078-e883-4972-9bbc-49e60bebca55

获取x86线程Context的函数</br>
Wow64GetThreadContext</br>
Wow64SetThreadContext</br>

# 关于x86程序（Wow64）的PEB
兼容型的x86进程拥有两个PEB，一个是64位的用于模拟环境的， 一个是PEB32自身的</br>
通过 NtQueryInformationProcess或ZwQueryInformationProcess</br>
查询ProcessBasicInformation值得到的PEB是64位的</br>
在win10 19045.4780版本下 PEB64在PEB32的-0x1000位置</br>
所以要获取PEB32需要在PEB64的地址上+0x1000得到PEB32的起始位置</br>
获得PEB32后按着PEB32位的格式解析即可 注意指针长度为4byte，使用的结构都是32位的
