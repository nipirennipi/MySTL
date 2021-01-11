#pragma once
#ifndef MS_ALGORITHM_H
#define MS_ALGORITHM_H

#include"walker.h"
#include"internal.h"

typedef int			int_type;
typedef size_t		size_type;


//_MS_advance for input_walker & forward_walker
template<typename InputWalker,typename Distance>
inline void _MS_advance(InputWalker& _Walker,Distance _Dis,input_iterator_tag){
	_MSJUDGE(_Dis<0,"negative advance of non-bidirectional walker");
	while(_Dis--){
		++_Walker;
	}
}

//_MS_advance for bidirectional_walker
template<typename BidirectionalWalker,typename Distance>
inline void _MS_advance(BidirectionalWalker& _Walker,Distance _Dis,bidirectional_iterator_tag){
	if(_Dis>=0){
		while(_Dis--){
			++_Walker;
		}
	}
	else{
		while(_Dis++){
			--_Walker;
		}
	}
}

//_MS_advance for random_access_walker
template<typename RandomAccessWalker,typename Distance>
inline void _MS_advance(RandomAccessWalker& _Walker,Distance _Dis,random_access_iterator_tag){
	_Walker+=_Dis;
}

//MS_advance
template<typename InputWalker,typename Distance>
inline void MS_advance(InputWalker& _Walker,Distance _Dis){
	_MS_advance(_Walker,_Dis,walker_traits<InputWalker>::iterator_category());
}



//MS_find
template<class InputWalker,class T>
InputWalker MS_find(InputWalker _First,InputWalker _Last,const T& _Val){
	while(_First!=_Last) {
		if(*_First==_Val){
			return _First;
		}
		++_First;
	}
	return _Last;
}


//MS_find_if
template<class InputWalker,class UnaryPredicate>
InputWalker MS_find_if(InputWalker _First,InputWalker _Last,UnaryPredicate _Pred){
	while(_First!=_Last) {
		if(_Pred(*_First)) return _First;
		++_First;
	}
	return _Last;
}



template<typename BidirectionalWalker,typename Compare>
void _MS_insertsort(BidirectionalWalker _First,BidirectionalWalker _Last,Compare _Pred){
	for(BidirectionalWalker W2=_First+1;W2<=_Last-1;++W2){
		typename walker_traits<BidirectionalWalker>::value_type Temp=*W2;
		BidirectionalWalker W1=W2-1;
		while(W1>=_First&&_Pred(Temp,*W1)){//----------------
		//while(W1>=_First&&!(_Pred(*W1,Temp)||*W1==Temp)){//----------------
			*(W1+1)=*W1;
			if(W1==_First){
				break;
			}
			--W1;
		}
		if(W1==_First&&_Pred(Temp,*W1)){//----------------
		//if(W1==_First&&!(_Pred(*W1,Temp)||*W1==Temp)){//----------------
			*W1=Temp;
		}
		else{
			*(W1+1)=Temp;
		}
	}
}

template<typename BidirectionalWalker,typename Compare>
typename walker_traits<BidirectionalWalker>::reference _MS_mid3(BidirectionalWalker _First,BidirectionalWalker _Last,Compare _Pred){			//基准的取法：三者取中
	BidirectionalWalker Left=_First;
	BidirectionalWalker Mid=_First+(_Last-_First)/2;
	BidirectionalWalker Right=_Last-1;
	typedef typename walker_traits<BidirectionalWalker>::value_type value_type;
	value_type Left_v=*Left;
	value_type Mid_v=*Mid;
	value_type Right_v=*Right;               
	value_type Temp_v=(_Pred(Left_v,Mid_v)?(_Pred(Mid_v,Right_v)?Mid_v:_Pred(Left_v,Right_v)?Right_v:Left_v):(_Pred(Mid_v,Right_v)?(_Pred(Left_v,Right_v)?Left_v:Right_v):Mid_v));//----------------
	if(!(_Pred(Mid_v,Temp_v)||_Pred(Temp_v,Mid_v))){
	//if(Mid_v==Temp_v){
		*Mid=*Left;
		*Left=Temp_v;
		return Temp_v;
	}
	if(!(_Pred(Right_v,Temp_v)||_Pred(Temp_v,Right_v))){
	//if(Right_v==Temp_v){
		*Right=*Left;
		*Left=Temp_v;
		return Temp_v;
	}
	return Temp_v;
}

template<typename BidirectionalWalker,typename Compare>
BidirectionalWalker _MS_partition(BidirectionalWalker _First,BidirectionalWalker _Last,Compare _Pred){		//划分函数
	BidirectionalWalker W1=_First;
	BidirectionalWalker W2=_Last-1;
	if(_First<(_Last-1)){
		typename walker_traits<BidirectionalWalker>::value_type Pivot=_MS_mid3(_First,_Last,_Pred);
		while(W1<W2){
			while(W1<W2&&!(_Pred(*W2,Pivot))){//----------------
				--W2;
			}
			*W1=*W2;
			while(W1<W2&&!(_Pred(Pivot,*W1))){//----------------
				++W1;
			}
			*W2=*W1;
		}
		*W1=Pivot;
	}
	return W1;
}

template<typename BidirectionalWalker,typename Compare>
void _MS_quicksort(BidirectionalWalker _First,BidirectionalWalker _Last,Compare _Pred){
	size_type Dis=0;
	BidirectionalWalker Temp=_First;
	while(Temp!=_Last&&Dis<11){
		++Temp;
		++Dis;
	}
 	if(Dis<=10){
		_MS_insertsort(_First,_Last,_Pred);
	}
	/*if(static_cast<size_type>(_Last-_First)<=10){
		_MS_insertsort(_First,_Last,_Pred);
	}*/
	else{
		BidirectionalWalker Pos=_MS_partition(_First,_Last,_Pred);
		_MS_quicksort(_First,Pos,_Pred);
		_MS_quicksort(++Pos,_Last,_Pred);
	}
}

//MS_sort
template<typename BidirectionalWalker>
void MS_sort(BidirectionalWalker _First,BidirectionalWalker _Last){
	_MS_quicksort(_First,_Last,_MS_Less<>());
}

template<typename BidirectionalWalker,typename Compare>
void MS_sort(BidirectionalWalker _First,BidirectionalWalker _Last,Compare _Pred){
	_MS_quicksort(_First,_Last,_Pred);
}


#endif // !MS_ALGORITHM_H