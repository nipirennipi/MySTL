#pragma once
#ifndef STDAFX_H
#define STDAFX_H

#include<iostream>

#include<cstdlib>		
#include<initializer_list>


//in STL--------------------------------------------
#include<algorithm>		//STL算法库
#include<iterator>
#include<functional>	//函数对象

//顺序容器
#include<array>			//随机迭代器
#include<string>
#include<vector>
#include<deque>
#include<list>			//双向迭代器
#include<forward_list>	//前向迭代器

//容器适配器（无迭代器）
#include<queue>
#include<stack>			//已含（priority_queue）

//关联容器
#include<set>			//双向迭代器
#include<map>			//双向迭代器
#include<unordered_set>
#include<unordered_map>

using namespace std;



#endif // !STDAFX_H