#pragma once
#ifndef MA_INTERNAL_H
#define MA_INTERNAL_H

template<typename Key=void>			//for MS_associated container
struct _MS_Less{
	bool operator()(const Key& _Left,const Key& _Right)const{
		return _Left<_Right;
	}
};

template<>
struct _MS_Less<void>{				//for MS_sort
	template<typename ValueType>
	bool operator()(ValueType _Left,ValueType _Right){
		return _Left<_Right;
	}
};

template<typename ValueType>		//KeyOfValue for MS_map
struct _MS_Select1st{
	typename ValueType::first_type operator()(const ValueType& _Pair){
		return _Pair.first;
	}
};

template<typename ValueType>		//KeyOfValue for MS_set
struct MS_identity{
	ValueType operator()(const ValueType& _Key){
		return _Key;
	}
};



#endif // !MA_INTERNAL_H