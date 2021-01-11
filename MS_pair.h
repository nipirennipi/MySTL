#pragma once
#ifndef MS_PAIR_H
#define MS_PAIR_H

template<typename Ty1,typename Ty2>
struct MS_pair{
	typedef Ty1 first_type;
	typedef Ty2 second_type;
	first_type first;
	second_type second;

	//constructor
	MS_pair():first(),second(){}				//MS_pair<const Key,Ty>必须出示化常量限定类型对象

	MS_pair(const first_type& _Val1):first(_Val1){}

	MS_pair(const first_type& _Val1,const second_type& _Val2):first(_Val1),second(_Val2){}

	template<class CpyTy1,class CpyTy2> 
	MS_pair(const MS_pair<CpyTy1,CpyTy2>& _Cpy):first(_Cpy.first),second(_Cpy.second){}

	//operator=	
	MS_pair& operator=(const MS_pair& _Cpy){
		first=_Cpy.first;
		second=_Cpy.second;
		return *this;
	}

};

	//make_pair
template <class Ty1,class Ty2>
MS_pair<Ty1,Ty2> make_pair(Ty1 _Val1,Ty2 _Val2){
	return MS_pair<Ty1,Ty2>(_Val1,_Val2);
}

	//relational operators
template<typename Ty1,typename Ty2>
bool operator==(const MS_pair<Ty1,Ty2>& _Left,const MS_pair<Ty1,Ty2>& _Right){
	return _Left.first==_Right.first&&_Left.second==_Right.second;
}

template<typename Ty1,typename Ty2>
bool operator!=(const MS_pair<Ty1,Ty2>& _Left,const MS_pair<Ty1,Ty2>& _Right){
	return !(_Left==_Right);
}

template<typename Ty1,typename Ty2>
bool operator<(const MS_pair<Ty1,Ty2>& _Left,const MS_pair<Ty1,Ty2>& _Right){
	return _Left.first<_Right.first||(!(_Right.first<_Left.first)&&_Left.second<_Right.second);
}

template<typename Ty1,typename Ty2>
bool operator>(const MS_pair<Ty1,Ty2>& _Left,const MS_pair<Ty1,Ty2>& _Right){
	return _Right<_Left;
}

template<typename Ty1,typename Ty2>
bool operator<=(const MS_pair<Ty1,Ty2>& _Left,const MS_pair<Ty1,Ty2>& _Right){
	return !(_Right<_Left);
}

template<typename Ty1,typename Ty2>
bool operator>=(const MS_pair<Ty1,Ty2>& _Left,const MS_pair<Ty1,Ty2>& _Right){
	return !(_Left<_Right);
}

#endif // !MS_PAIR_H