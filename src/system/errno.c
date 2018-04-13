# include "errno.h"
ff_u8_t ffly_errno = FF_ERR_NULL;
# ifdef __fflib
ff_s32_t errno = 0;
char const static *s[] = {
	"okay",							//0
	"operation not permitted",		//1
	"no such file or directory",	//2
	"no such process",				//3
	"interrupted system call",		//4
	"i/o error",					//5
	"no such device or address",	//6
	"argument list too long",		//7
	"exec format error",			//8
	"bad file number",				//9
	"no child processes",			//10
	"try again",					//11
	"out of memory",				//12
	"permission denied",			//13
	"bad address",					//14
	"block device required",		//15
	"device or resource busy",		//16
	"file exists",					//17
	"cross-device link",			//18
	"no such device",				//19
	"not a directory",				//20
	"is a directory",				//21
	"invalid argument",				//22
	"file table overflow",			//23
	"too many open files",			//24
	"not a typewriter",				//25
	"text file busy",				//26
	"file too large",				//27
	"no space left on device",		//28
	"illegal seek",					//29
	"read-only file system",		//30
	"too many links",				//31
	"broken pipe",					//32
	"math argument out of domain of func",	//33
	"math result not representable",		//34
	"resource deadlock would occur",		//35
	"file name too long",					//36
	"no record locks available",			//37
	"invalid system call number",			//38
	"directory not empty",					//39
	"too many symbolic links encountered",	//40
	"operation would block",				//41
	"no message of desired type",			//42
	"identifier removed",					//43
	"channel number out of range",			//44
	"level 2 not synchronized",				//45
	"level 3 halted",						//46
	"level 3 reset",						//47
	"link number out of range",				//48
	"protocol driver not attached",			//49
	"no CSI structure available",			//50
	"level 2 halted",						//51
	"invalid exchange",						//52
	"invalid request descriptor",			//53
	"exchange full",						//54
	"no anode",								//55
	"invalid request code",					//56
	"invalid slot",							//57
	"58",									//58
	"bad font file format",					//59
	"device not a stream",					//60
	"no data available",					//61
	"timer expired",						//62
	"out of streams resources",				//63
	"machine is not on the network",		//64
	"package not installed",				//65
	"object is remote",						//66
	"link has been severed",				//67
	"advertise error",						//68
	"srmount error",						//69
	"communication error on send",			//70
	"protocol error",						//71
	"multihop attempted",					//72
	"rfs specific error",					//73
	"not a data message",					//74
	"value too large for defined data type",//75
	"name not unique on network",			//76
	"file descriptor in bad state",			//77
	"remote address changed",				//78
	"can not access a needed shared library",	//79
	"accessing a corrupted shared library",		//80
	".lib section in a.out corrupted",			//81
	"attempting to link in too many shared libraries",	//82
	"cannot exec a shared library directly",	//83
	"illegal byte sequence",					//84
	"interrupted system call should be restarted",	//85
	"streams pipe error",						//86
	"too many users",							//87
	"socket operation on non-socket",			//88
	"destination address required",				//89
	"message too long",							//90
	"protocol wrong type for socket",			//91
	"protocol not available",					//92
	"protocol not supported",					//93
	"socket type not supported",				//94
	"operation not supported on transport endpoint",	//95
	"protocol family not supported",			//96
	"address family not supported by protocol",	//97
	"address already in use",					//98
	"cannot assign requested address",			//99
	"network is down",							//100
	"network is unreachable",					//101
	"network dropped connection because of reset",	//102
	"software caused connection abort",			//103
	"connection reset by peer",					//104
	"no buffer space available",				//105
	"transport endpoint is already connected",	//106
	"transport endpoint is not connected",		//107
	"cannot send after transport endpoint shutdown",	//108
	"too many references: cannot splice",		//109
	"connection timed out",						//110
	"connection refused",						//111
	"host is down",								//112
	"no route to host"							//113
};

char const* strerror(int __no) {
	int unsigned no;
	if ((no = (__no^~0)+1) >= (sizeof(s)/sizeof(char const*)))
		return "unknown";
	return s[no];
}

/*
	find diffrent way to set errno
	as compiler is setting up the stack
	when its not needed.
*/
void __set_errno(void) {
	__asm__("mov %%eax, %0\n\t" : "=m"(errno));
}
# endif
