#pragma once
#ifndef MS_EXCEPTION_H
#define MS_EXCEPTION_H

#include"stdafx.h"

//void _MSJUDGE(bool _Cond,string _Message){
//	if(_Cond){
//		_RPTF0(_CRT_ASSERT,_Message);				//Terminate the program,display _Message
//	}
//}

#define _MSJUDGE(_Cond,_Message)\
	if(_Cond){\
		_RPTF0(_CRT_ASSERT,_Message);\
	}


#define _MSTYPE(_Type1,_Type2)\
	try{\
		throw  _Type1();\
	}\
	catch(_Type2){\
	\
	}\
	catch(...){\
		_RPTF0(_CRT_ASSERT,"The element types in the two MS_containers do not match");\
	}



#endif // !MS_EXCEPTION_H