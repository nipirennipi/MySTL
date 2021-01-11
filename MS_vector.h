#pragma once
#ifndef MS_VECTOR_H
#define MS_VECTOR_H

#include"walker.h"
#include"base.h"
#include"MS_exception.h"
#include"MS_algorithm.h"

//Wrap value_type for use by iterator
template<typename ValueType>
struct _wrap_value_type{
	typedef ValueType			value_type;
	typedef value_type*			pointer;
	typedef const value_type*	const_pointer;
	typedef value_type&			reference;
	typedef const value_type&	const_reference;
};

template<typename Ty>
class MS_vector;

template<typename WrapType>
class _MS_vector_const_walker:public MS_walker_base<WrapType>{
public:
	typedef random_access_iterator_tag			iterator_category;
	typedef typename WrapType::value_type		value_type;
	typedef int									difference_type;
	typedef typename WrapType::const_pointer	pointer;
	typedef typename WrapType::const_reference	reference;

	typedef typename WrapType::pointer			_Ty_ptr;				//native pointer type
	typedef typename MS_vector<value_type>*		_Ty_Vec_ptr;			//native pointer type to MS_vector
public:
	_Ty_Vec_ptr _Vec_ptr;												//a pointer to a MS_vector
	_Ty_ptr _Ptr;														//a pointer to the element in a MS_vector
public:
	_MS_vector_const_walker():_Vec_ptr(nullptr),_Ptr(nullptr){}			//default construction
	_MS_vector_const_walker(_Ty_Vec_ptr _Vec_ptr_,_Ty_ptr _Ptr_):_Vec_ptr(_Vec_ptr_),_Ptr(_Ptr_){}	
	_MS_vector_const_walker(_Ty_Vec_ptr _Vec_ptr_):_Vec_ptr(_Vec_ptr_),_Ptr(nullptr){}	

	//ConcreteIterator(ConcreteAggregate<Item>*a):aggr(a),cur(0){}

	reference operator*()const{//---------------------------判断是否指向空的MS_vector,walker in [begin,end)?
		_MSJUDGE(!_Ptr,"Can not dereference the empty MS_vector walker");
		_MSJUDGE(_Ptr<_Vec_ptr->start||_Vec_ptr->finish<_Ptr,"Can not dereference the out of range MS_vector walker");
		return *_Ptr;
	}

	pointer operator->()const{//---------------------------判断是否指向空的MS_vector,walker in [begin,end)?
		_MSJUDGE(!_Ptr,"The empty MS_vector walker can not call operator->()");
		_MSJUDGE(_Ptr<_Vec_ptr->start||_Vec_ptr->finish<_Ptr,"The out of range MS_vector walker can not call operator->()");
		return _Ptr;
	}

	_MS_vector_const_walker& operator++(){//---------------------------判断是否指向空的MS_vector,++walker in [begin,end)?
		_MSJUDGE(!_Ptr,"The empty MS_vector walker can not move");
		_MSJUDGE(_Ptr>=_Vec_ptr->finish,"MS_vector walker can not move over end");
		++_Ptr;
		return *this;
	}

	_MS_vector_const_walker operator++(int){
		_MS_vector_const_walker temp=*this;
		++*this;											//用operator++()的异常处理
		return temp;
	}

	_MS_vector_const_walker& operator--(){//---------------------------判断是否指向空的MS_vector,--walker in [begin,end)?
		_MSJUDGE(!_Ptr,"The empty MS_vector walker can not move");
		_MSJUDGE(_Ptr<=_Vec_ptr->start,"MS_vector walker can not move over begin");
		--_Ptr;
		return *this;
	}

	_MS_vector_const_walker operator--(int){
		_MS_vector_const_walker temp=*this;
		--*this;											//用operator--()的异常处理
		return temp;
	}

	_MS_vector_const_walker& operator+=(const difference_type _Dif){
		//---------------------------判断是否指向空的MS_vector,walker+=_Dif in [begin,end)?
		_MSJUDGE(_Dif!=0&&!_Ptr,"The empty MS_vector walker cannot move");
		_MSJUDGE(_Dif>0&&_Dif>_Vec_ptr->finish-_Ptr,"MS_vector walker can not move over end");
		_MSJUDGE(_Dif<0&&_Dif<_Vec_ptr->start-_Ptr,"MS_vector walker can not move over begin");
		_Ptr+=_Dif;
		return *this;
	}

	_MS_vector_const_walker operator+(const difference_type _Dif)const{
		_MS_vector_const_walker temp=*this;
		return temp+=_Dif;									//用operator+=()的异常处理
	}

	_MS_vector_const_walker& operator-=(const difference_type _Dif){
		return *this+=-_Dif;								//用operator+=()的异常处理						
	}

	_MS_vector_const_walker operator-(const difference_type _Dif)const{
		_MS_vector_const_walker temp=*this;
		return temp+=-_Dif;									//用operator+=()的异常处理
	}
	
	difference_type operator-(const _MS_vector_const_walker& _Right)const{//---------------------------判断是否指向一同个MS_vector
		_MSJUDGE(_Vec_ptr!=_Right._Vec_ptr,"Two walkers from different MS_vector");
		return _Ptr-_Right._Ptr;
	}

	reference operator[](const difference_type _Dif)const{
		return *(*this+_Dif);								//用operator+=()的异常处理
	}

	bool operator==(const _MS_vector_const_walker& _Right)const{//---------------------------判断是否指向一同个MS_vector
		_MSJUDGE(_Vec_ptr!=_Right._Vec_ptr,"Two walkers from different MS_vector");
		return _Ptr==_Right._Ptr;
	}

	bool operator!=(const _MS_vector_const_walker& _Right)const{
		return !(*this==_Right);							//用operator==()的异常处理
	}

	bool operator<(const _MS_vector_const_walker& _Right)const{//---------------------------判断是否指向一同个MS_vector
		_MSJUDGE(_Vec_ptr!=_Right._Vec_ptr,"Two walkers from different MS_vector");
		return _Ptr<_Right._Ptr;
	}

	bool operator>(const _MS_vector_const_walker& _Right)const{
		return _Right<*this;								//用operator<()的异常处理
	}

	bool operator<=(const _MS_vector_const_walker& _Right)const{
		return !(_Right<*this);								//用operator<()的异常处理
	}

	bool operator>=(const _MS_vector_const_walker& _Right)const{
		return !(*this<_Right);								//用operator<()的异常处理
	}
};

template<typename WrapType>
class _MS_vector_walker:public _MS_vector_const_walker<WrapType>{
public:
	typedef random_access_iterator_tag					iterator_category;
	typedef typename WrapType::value_type				value_type;
	typedef int											difference_type;
	typedef typename WrapType::pointer					pointer;
	typedef typename WrapType::reference				reference;

	typedef typename _MS_vector_const_walker<WrapType>	MS_vcwalker;
	typedef typename WrapType::pointer					_Ty_ptr;				//native pointer type
	typedef typename MS_vector<value_type>*				_Ty_Vec_ptr;			//native pointer type to MS_vector

public:
	_MS_vector_walker(){}
	_MS_vector_walker(_Ty_Vec_ptr _Vec_ptr_,_Ty_ptr _Ptr_):MS_vcwalker(_Vec_ptr_,_Ptr_){}

	reference operator*()const{
		return const_cast<reference>(MS_vcwalker::operator*());
	}

	pointer operator->()const{
		return const_cast<pointer>(MS_vcwalker::operator->());
	}

	_MS_vector_walker& operator++(){
		MS_vcwalker::operator++();
		return *this;
	}

	_MS_vector_walker operator++(int){
		_MS_vector_walker temp;
		MS_vcwalker::operator++();
		return temp;
	}

	_MS_vector_walker& operator--(){
		MS_vcwalker::operator--();
		return *this;
	}

	_MS_vector_walker operator--(int){
		_MS_vector_walker temp=*this;
		MS_vcwalker::operator--();
		return temp;
	}

	_MS_vector_walker& operator+=(const difference_type _Dif){
		MS_vcwalker::operator+=(_Dif);
		return *this;
	}

	_MS_vector_walker operator+(const difference_type _Dif)const{
		_MS_vector_walker temp=*this;
		return temp+=_Dif;
	}

	_MS_vector_walker& operator-=(const difference_type _Dif){
		MS_vcwalker::operator+=(-_Dif);
		return *this;
	}

	_MS_vector_walker operator-(const difference_type _Dif)const{
		_MS_vector_walker temp=*this;
		return temp+=-_Dif;
	}

	difference_type operator-(const _MS_vector_walker& _Right)const{
		return MS_vcwalker::operator-(_Right);
	}

	reference operator[](const difference_type _Dif)const{
		return const_cast<reference>(MS_vcwalker::operator[](_Dif));
	}
};

template<typename Ty>
class MS_vector:public MS_container_base<Ty>{	
	template<typename WrapType> friend class _MS_vector_const_walker;
public:
	typedef Ty				value_type;
	typedef Ty*				pointer;
	typedef const Ty*		const_pointer;
	typedef Ty&				reference;
	typedef const Ty&		const_reference;
	typedef size_t			size_type;

	typedef int				int_type;

public:
	using walker			=_MS_vector_walker<_wrap_value_type<Ty> >;
	using const_walker		=_MS_vector_const_walker<_wrap_value_type<Ty> >;

private:
	pointer start;
	pointer finish;
	pointer end_of_storage;

private:
	//address of MS_vector object
	MS_vector<Ty>* address()const{	//const MS_vector<Ty>* const 
		return const_cast<MS_vector<Ty>*>(this);
	}
	
	void _reset_capacity(int_type _Capacity){
		_MSJUDGE(_Capacity<0,"The allocation of space size in MS_vector cannot be specified as a negative number");
		if(start){
			delete []start;
		}
		start=new value_type[_Capacity];
		_MSJUDGE(!start,"Dynamic memory allocation failed");
		finish=start;
		end_of_storage=start+_Capacity;
	}

	void _reset_capacity_copy(int_type _Capacity){
		_MSJUDGE(_Capacity<0,"The allocation of space size in MS_vector cannot be specified as a negative number");
		size_type Dis=size();
		if(_Capacity<Dis){
			_MSJUDGE(_Capacity<Dis,"The new capacity is not smaller than the original size");
			return;
		}
		pointer New_start=new value_type[_Capacity];
		
		_MSJUDGE(!New_start,"Dynamic memory allocation failed");
		if(start!=finish){
			for(size_type Pos=0;Pos<Dis;++Pos){
				New_start[Pos]=start[Pos];
			}
		}
		if(start){
			delete []start;
		}
		start=New_start;
		finish=start+Dis;
		end_of_storage=start+_Capacity;
	}

public:
	//constructor
	MS_vector():start(nullptr),finish(nullptr),end_of_storage(nullptr){};

	explicit MS_vector(int_type _Capacity){
		_MSJUDGE(_Capacity<0,"The number of elements in MS_vector cannot be specified as a negative number");
		start=new value_type[_Capacity];
		_MSJUDGE(!start,"Dynamic memory allocation failed");
		finish=start;
		end_of_storage=start+_Capacity;
	}

	MS_vector(int_type _Capacity,const value_type& _Val){
		_MSJUDGE(_Capacity<0,"The number of elements in MS_vector cannot be specified as a negative number");
		start=new value_type[_Capacity];
		_MSJUDGE(!start,"Dynamic memory allocation failed");
		finish=start+_Capacity;
		end_of_storage=start+_Capacity;
		for(size_type Pos=0;Pos<_Capacity;++Pos){
			start[Pos]=_Val;
		}
	}

	template <class InputWalker>
	MS_vector(InputWalker _First,InputWalker _Last){
		int_type Dis=static_cast<int_type>(_Last-_First);
		//两个异常处理问题，
		_MSJUDGE(Dis<=0,"Does not satisfy walker range requirements");		//1.[_First,_Last)是一个范围吗?
		typedef	 typename walker_traits<InputWalker>::value_type type2;
		_MSTYPE(value_type,type2);		//2.walker_traits<InputWalker>::value_type与value_type是否为一样的类型
		start=new value_type[Dis];
		_MSJUDGE(!start,"Dynamic memory allocation failed");
		size_type Pos=0;
		while(_First!=_Last){
			start[Pos]=*_First;
			++_First;
			++Pos;
		}
		finish=start+Dis;
		end_of_storage=finish;
	}

	MS_vector(initializer_list<value_type> _List){
		typename ::std::initializer_list<value_type>::iterator It=_List.begin();
		while(It!=_List.end()){
			push_back(*It);
			++It;
		}
	}

	//copy constructor
	MS_vector(const MS_vector<value_type>& _Cpy){
		size_type Capacity=_Cpy.capacity();
		if(Capacity==0){
			MS_vector();
		}
		else{
			start=new value_type[Capacity];
			_MSJUDGE(!start,"Dynamic memory allocation failed");
			size_type Dis=_Cpy.size();
			finish=start+Dis;
			end_of_storage=start+Capacity;
			for(size_type Pos=0;Pos<Dis;++Pos){
				start[Pos]=_Cpy[Pos];
			}
		}
	}

	//move constructor
	MS_vector(MS_vector&& _Cpy)noexcept{
		if(_Cpy.start==nullptr){
			return;
		}
		start=_Cpy.start;
		finish=_Cpy.finish;
		end_of_storage=_Cpy.end_of_storage;
		_Cpy.start=nullptr;
		_Cpy.finish=nullptr;
		_Cpy.end_of_storage=nullptr;
	}

	//copy assignment					
	MS_vector& operator=(const MS_vector& _Cpy){
		if(_Cpy.address()==this){
			return *this;
		}
		else{
			size_type Capacity=_Cpy.capacity();
			size_type Dis=_Cpy.size();
			if(capacity()>=Capacity){
				finish=start+Dis;	
			}
			else{
				if(start){
					delete []start;
				}
				start=new value_type[Capacity];
				_MSJUDGE(!start,"Dynamic memory allocation failed");
				finish=start+Dis;
				end_of_storage=start+Capacity;
			}
			for(size_type Pos=0;Pos<Dis;++Pos){
				start[Pos]=_Cpy[Pos];
			}
		}
		return *this;
	}

	//move assignment
	MS_vector& operator=(MS_vector&& _Cpy)noexcept{
		if(start==_Cpy.start){
			return *this;
		}
		start=_Cpy.start;
		finish=_Cpy.finish;
		end_of_storage=_Cpy.end_of_storage;
		_Cpy.start=nullptr;
		_Cpy.finish=nullptr;
		_Cpy.end_of_storage=nullptr;
		return *this;
	}

	//Walker
	walker begin(){
		return walker(address(),start);
	}

	const_walker begin()const{
		return const_walker(address(),start);
	}

	walker end(){
		return walker(address(),finish);
	}

	const_walker end()const{
		return const_walker(address(),finish);
	}

	const_walker cbegin()const{
		return const_walker(address(),start);
	}

	const_walker cend()const{
		return const_walker(address(),finish);
	}

	//Capacity
	size_type size()const{
		return finish-start;
	}

	size_type capacity()const{
		return end_of_storage-start;
	}

	bool empty()const{
		return start==finish;
	}

	//Element access
	reference operator[](const int_type _Pos){
		_MSJUDGE(_Pos<0||_Pos>=(finish-start),"access out of range");
		return *(start+_Pos);
	}

	const_reference operator[](const int_type _Pos)const{
		_MSJUDGE(_Pos<0||_Pos>=(finish-start),"access out of range");
		return *(start+_Pos);
	}

	reference at(const int_type _Pos){
		return operator[](_Pos);
	}

	const_reference at(const int_type _Pos)const{
		return operator[](_Pos);
	}

	reference front(){
		_MSJUDGE(start==finish,"empty MS_vector can not call front()");
		return *start;
	}

	const_reference front()const{
		_MSJUDGE(start==finish,"empty MS_vector can not call front()");
		return *start;
	}

	reference back(){
		_MSJUDGE(start==finish,"empty MS_vector can not call front()");
		return *(finish-1);
	}

	const_reference back()const{
		_MSJUDGE(start==finish,"empty MS_vector can not call front()");
		return *(finish-1);
	}

	//Modifiers
	void push_back(const value_type& _Val){
		size_type Dis=size();
		size_type New_capacity=0;
		if(finish==end_of_storage){
			if(capacity()<10){
				New_capacity=capacity()+1;
				
			}
			else{
				New_capacity=capacity()*1.5;
			}
			_reset_capacity_copy(New_capacity);
		}
		start[Dis]=_Val;
		finish=start+Dis+1;
	}

	void pop_back(){
		if(empty()){
			_MSJUDGE(empty(),"empty MS_vector can not do pop_back");
			return;
		}
		finish=finish-1;
	}

	walker erase(walker _Pos){
		_MSJUDGE(_Pos._Vec_ptr!=this,"walker parameter in erase does not point to the current MS_vector")
		_MSJUDGE(_Pos._Ptr<start||finish<=_Pos._Ptr,"walker parameter in erase is out of range");
		size_type Pos=0;
		while(start[Pos]!=*_Pos){
			++Pos;
		}
		size_type Dis=size();
		for(size_type i=Pos;i<=Dis-2;++i){
			start[i]=start[i+1];
		}
		finish=finish-1;
		return _Pos;
	}

	walker erase(walker _First,walker _Last){
		_MSJUDGE(_First._Vec_ptr!=this||_First._Vec_ptr!=this,"walker parameter in erase does not point to the current MS_vector");
		int_type Dif=static_cast<int_type>(_Last-_First);
		_MSJUDGE(Dif<=0,"Does not satisfy walker range requirements");
		_MSJUDGE(!(start<=_First._Ptr&&_First._Ptr<finish),"walker parameter first in erase is out of range");
		_MSJUDGE(!(start<_Last._Ptr&&_Last._Ptr<=finish),"walker parameter last in erase is out of range");
		size_type Pos=0;
		while(start[Pos]!=*_First){
			++Pos;
		}
		size_type Dis=size();
		for(size_type i=Pos;i<=Dis-1-Dif;++i){
			start[i]=start[i+Dif];
		}
		finish=finish-Dif;
		return _First;
	}

	void clear(){
		_reset_capacity(capacity());
	}

	//sort
	void sort(){
		_MS_quicksort(begin(),end(),_MS_Less<>());
	}

	template<typename Compare>
	void sort(Compare _Pred){
		_MS_quicksort(begin(),end(),_Pred);
	}

	//destructor
	~MS_vector(){
		if(start){
			delete[]start;
		}
	}

};

#endif // !MS_VECTOR_H