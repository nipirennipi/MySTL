#pragma once
#ifndef MS_SET_H
#define MS_SET_H

#include"MS_exception.h"
#include"MS_pair.h"
#include"rb_tree.h"
#include"internal.h"

using namespace core;

template <class Key,class Compare=_MS_Less<Key>>
class MS_set{
public:
	typedef Key										key_type;
	typedef Key										value_type;
	typedef Compare									key_compare;

	typedef int										int_type;
	typedef size_t									size_type;
	typedef rb_tree<key_type,value_type,MS_identity<value_type>,Compare>	base;
public:
	typedef typename base::iterator					walker;
	typedef typename base::const_iterator			const_walker;

	typedef typename base::reverse_iterator			reverse_walker;
	typedef typename base::const_reverse_iterator	const_reverse_walker;

private:
	base _rb_tree;
public:
	//constructor
	MS_set():_rb_tree(){};

	MS_set(initializer_list<value_type> _List):_rb_tree(){
		typename initializer_list<value_type>::iterator First=_List.begin();
		for(; First!=_List.end(); ++First){
			_rb_tree.insert_unique(*First);
		}
	}

	template <class InputWalker>
	MS_set(InputWalker _First,InputWalker _Last):_rb_tree(){
		for(; _First!=_Last; ++_First){
			_rb_tree.insert_unique(*_First);
		}
	}

	MS_set(const MS_set& _Cpy){
		_rb_tree=_Cpy._rb_tree;
	}
	//operator=
	MS_set& operator=(const MS_set& _Cpy){
		_rb_tree.operator=(_Cpy._rb_tree);
		return *this;
	}

	//iterator(walker)
	walker begin(){
		return _rb_tree.begin();
	}

	const_walker begin()const{
		return _rb_tree.begin();
	}

	const_walker cbegin()const{
		return _rb_tree.cbegin();
	}

	walker end(){
		return _rb_tree.end();
	}

	const_walker end()const{
		return _rb_tree.end();
	}

	const_walker cend()const{
		return _rb_tree.cend();
	}

	reverse_walker rbegin(){
		return _rb_tree.rbegin();
	}

	const_reverse_walker rbegin()const{
		return _rb_tree.rbegin();
	}

	const_reverse_walker crbegin()const{
		return _rb_tree.crbegin();
	}

	reverse_walker rend() {
		return _rb_tree.rend();
	}

	const_reverse_walker rend()const{
		return _rb_tree.rend();
	}

	const_reverse_walker crend()const{
		return _rb_tree.crend();
	}
	
	//Capacity
	size_type size()const{
		return _rb_tree.size();
	}

	bool empty()const{
		return _rb_tree.empty();
	}

	//Modifiers:
	MS_pair<walker,bool> insert(const value_type& _Val){
		walker Iter=_rb_tree.find(_Val);
		if(Iter==end()){
			Iter=_rb_tree.insert_unique(_Val);
			return MS_pair<walker,bool>(Iter,true);
		}
		return MS_pair<walker,bool>(Iter,false);
	}

	template <class InputIterator>
	void insert(InputIterator _First,InputIterator _Last){
		for(; _First!=_Last; ++_First){
			_rb_tree.insert_unique(*_First);
		}
	}

	void erase(walker _Pos){
		_rb_tree.erase(_Pos);
	}

	size_type erase(const key_type& _Key){
		return _rb_tree.erase(_Key);
	}

	void erase(walker _First,walker _Last){
		_rb_tree.erase(_First,_Last);
	}

	void swap(MS_set& _Other){
		_rb_tree.swap(_Other._rb_tree);
	}

	void clear(){
		_rb_tree.clear();
	}

	//Operations:
	walker find(const key_type& _Key){
		_rb_tree.find(_Key);
	}

	const_walker find(const key_type& _Key)const{
		_rb_tree.find(_Key);
	}

	size_type count(const key_type& _Key)const{
		if(_rb_tree.find(_Key)==end()){
			return 0;
		}
		return 1;
	}

	walker lower_bound(const key_type& _Key){
		return _rb_tree.lower_bound(_Key);
	}

	const_walker lower_bound(const key_type& _Key)const{
		return _rb_tree.lower_bound(_Key);
	}

	walker upper_bound(const key_type& _Key){
		return _rb_tree.upper_bound(_Key);
	}

	const_walker upper_bound(const key_type& _Key)const{
		return _rb_tree.upper_bound(_Key);
	}

	MS_pair<walker,walker> equal_range(const key_type& _Key){
		return MS_pair<walker,walker>(lower_bound(_Key),upper_bound(_Key));
	}

	MS_pair<const_walker,const_walker> equal_range(const key_type& _Key)const{
		return MS_pair<const_walker,const_walker>(lower_bound(_Key),upper_bound(_Key));
	}

	//destructor
	~MS_set(){}
};

#endif // !MS_SET_H