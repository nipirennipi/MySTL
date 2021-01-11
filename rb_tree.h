/*====================================================================
Copyright (C) 2016-2016 Ruler. All rights reserved.
Author:  Ruler
Address: Nan'an District,Chongqing,China
Contact: 26105499@qq.com
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.
3. The name of the author may not be used to endorse or promote
   products derived from this software without specific prior
   written permission.
THIS SOFTWARE IS PROVIDED BY RULER ``AS IS'' AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.
====================================================================*/
#pragma once

#ifndef __CORE_RBTREE_H__
#define __CORE_RBTREE_H__

#include <memory>
#include <iterator>
#include <functional>
#include <utility>
using namespace std;

namespace core
{
	typedef bool rb_tree_color_type;
	constexpr rb_tree_color_type rb_tree_red=false;
	constexpr rb_tree_color_type rb_tree_black=true;

	typedef signed char rb_tree_node_state;
	constexpr rb_tree_node_state rb_tree_state_parent=-0x10;
	constexpr rb_tree_node_state rb_tree_state_sibling=0x01;
	constexpr rb_tree_node_state rb_tree_state_left=0x12;
	constexpr rb_tree_node_state rb_tree_state_right=0x13;
	constexpr rb_tree_node_state rb_tree_state_root=0x04;

	// Template class rb_tree_node
	template <class T>
	struct rb_tree_node
	{
		typedef rb_tree_node<T>  node_type;
		typedef node_type* node_pointer;
		typedef const node_type* const_node_pointer;
		typedef node_type& node_reference;
		typedef const node_type& const_node_reference;

		node_pointer             parent;
		node_pointer             left;
		node_pointer             right;
		rb_tree_color_type       color;
		T                        data;
	};

	// Template class rb_tree_type_traits

	template <class Tree,bool IsConst>
	struct rb_tree_type_traits
	{
		typedef typename Tree::node_type       node_type;
		typedef typename Tree::node_pointer    node_pointer;
		typedef typename Tree::value_type      value_type;
		typedef typename Tree::pointer         pointer;
		typedef typename Tree::reference       reference;
		typedef typename Tree::size_type       size_type;
		typedef typename Tree::difference_type difference_type;
	};

	template <class Tree>
	struct rb_tree_type_traits<Tree,true>
	{
		typedef typename Tree::node_type       node_type;
		typedef typename Tree::node_pointer    node_pointer;
		typedef typename Tree::value_type      value_type;
		typedef typename Tree::const_pointer   pointer;
		typedef typename Tree::const_reference reference;
		typedef typename Tree::size_type       size_type;
		typedef typename Tree::difference_type difference_type;
	};

	// Template class rb_tree_iterator
	template <class Tree,bool IsConst>
	class rb_tree_iterator
	{
	public:
		// types:

		typedef rb_tree_iterator<Tree,IsConst>                              iterator_type;
		typedef std::bidirectional_iterator_tag                            iterator_category;//-------------------

		typedef typename rb_tree_type_traits<Tree,IsConst>::node_type       node_type;
		typedef typename rb_tree_type_traits<Tree,IsConst>::node_pointer    node_pointer;
		typedef typename rb_tree_type_traits<Tree,IsConst>::value_type      value_type;
		typedef typename rb_tree_type_traits<Tree,IsConst>::pointer         pointer;
		typedef typename rb_tree_type_traits<Tree,IsConst>::reference       reference;
		typedef typename rb_tree_type_traits<Tree,IsConst>::size_type       size_type;
		typedef typename rb_tree_type_traits<Tree,IsConst>::difference_type difference_type;

		// construct/copy/destroy:

		rb_tree_iterator(void) noexcept
			: node(nullptr)
		{}
		explicit rb_tree_iterator(const node_pointer ptr) noexcept
			: node(ptr)
		{}
		rb_tree_iterator(const rb_tree_iterator<Tree,IsConst>& x) noexcept
			: node(x.get_pointer())
		{}

		rb_tree_iterator<Tree,IsConst>& operator=(const rb_tree_iterator<Tree,IsConst>& x) noexcept
		{
			if(this!=&x)
				node=x.get_pointer();
			return (*this);
		}

		operator rb_tree_iterator<Tree,true>(void) const noexcept
		{
			return rb_tree_iterator<Tree,true>(node);
		}

		// rb_tree_iterator operations:

		node_pointer get_parent(void) noexcept
		{
			return node->parent;
		}
		const node_pointer get_parent(void) const noexcept
		{
			return node->parent;
		}

		node_pointer get_pointer(void) noexcept
		{
			return node;
		}
		const node_pointer get_pointer(void) const noexcept
		{
			return node;
		}

		rb_tree_color_type get_color(void) const noexcept
		{
			return node->color;
		}

		reference operator*(void) const noexcept
		{
			return node->data;
		}
		pointer operator->(void) const noexcept
		{
			return &(operator*());
		}

		// increment / decrement

		rb_tree_iterator<Tree,IsConst>& operator++(void) noexcept
		{
			if(node->right!=nullptr)
			{
				node=node->right;
				while(node->left!=nullptr)
					node=node->left;
			}
			else
			{
				node_pointer ptr=node->parent;
				while(node==ptr->right)
				{
					node=ptr;
					ptr=ptr->parent;
				}
				if(node->right!=ptr)
					node=ptr;
			}
			return *this;
		}
		rb_tree_iterator<Tree,IsConst>& operator--(void) noexcept
		{
			if(node->color==rb_tree_red&&node->parent->parent==node)
			{
				node=node->right;
			}
			else if(node->left!=nullptr)
			{
				node_pointer ptr=node->left;
				while(ptr->right!=nullptr)
					ptr=ptr->right;
				node=ptr;
			}
			else
			{
				node_pointer ptr=node->parent;
				while(node==ptr->left)
				{
					node=ptr;
					ptr=ptr->parent;
				}
				node=ptr;
			}
			return *this;
		}
		rb_tree_iterator<Tree,IsConst> operator++(int) noexcept
		{
			iterator_type tmp(*this);
			this->operator++();
			return tmp;
		}
		rb_tree_iterator<Tree,IsConst> operator--(int) noexcept
		{
			iterator_type tmp(*this);
			this->operator--();
			return tmp;
		}

		// relational operators:

		template <bool b>
		bool operator==(const rb_tree_iterator<Tree,b>& rhs) const noexcept
		{
			return (node==rhs.get_pointer());
		}
		template <bool b>
		bool operator!=(const rb_tree_iterator<Tree,b>& rhs) const noexcept
		{
			return (node!=rhs.get_pointer());
		}
	private:
		node_pointer node;
	};

	// Template class rb_tree_primitive_iterator
	template <class Tree,bool IsConst>
	class rb_tree_primitive_iterator
	{
	public:
		// types:

		typedef rb_tree_primitive_iterator<Tree,IsConst>                    iterator_type;
		typedef std::bidirectional_iterator_tag                             iterator_category;

		typedef typename rb_tree_type_traits<Tree,IsConst>::node_type       node_type;
		typedef typename rb_tree_type_traits<Tree,IsConst>::node_pointer    node_pointer;
		typedef typename rb_tree_type_traits<Tree,IsConst>::value_type      value_type;
		typedef typename rb_tree_type_traits<Tree,IsConst>::pointer         pointer;
		typedef typename rb_tree_type_traits<Tree,IsConst>::reference       reference;
		typedef typename rb_tree_type_traits<Tree,IsConst>::size_type       size_type;
		typedef typename rb_tree_type_traits<Tree,IsConst>::difference_type difference_type;

		// construct/copy/destroy:

		rb_tree_primitive_iterator(void) noexcept
			: node(nullptr)
			,state(rb_tree_state_root)
		{}
		explicit rb_tree_primitive_iterator(const node_pointer ptr) noexcept
			: node(ptr)
			,state(rb_tree_state_root)
		{}
		rb_tree_primitive_iterator(const rb_tree_primitive_iterator<Tree,IsConst>& x) noexcept
			: node(x.get_pointer())
			,state(x.get_state())
		{}

		rb_tree_primitive_iterator<Tree,IsConst>& operator=(const rb_tree_primitive_iterator<Tree,IsConst>& x) noexcept
		{
			if(this!=&x)
			{
				node=x.get_pointer();
				state=x.get_state();
			}
			return (*this);
		}

		operator rb_tree_primitive_iterator<Tree,true>(void) const noexcept
		{
			return rb_tree_primitive_iterator<Tree,true>(node);
		}

		// rb_tree_primitive_iterator operations:

		node_pointer get_parent(void) noexcept
		{
			return node->parent;
		}
		const node_pointer get_parent(void) const noexcept
		{
			return node->parent;
		}

		node_pointer get_pointer(void) noexcept
		{
			return node;
		}
		const node_pointer get_pointer(void) const noexcept
		{
			return node;
		}

		rb_tree_node_state get_state(void) const noexcept
		{
			return state;
		}

		intptr_t get_depth(void) const noexcept
		{
			return static_cast<intptr_t>(state>>4);
		}

		rb_tree_color_type get_color(void) const noexcept
		{
			return node->color;
		}

		reference operator*(void) const noexcept
		{
			return node->data;
		}
		pointer operator->(void) const noexcept
		{
			return &(operator*());
		}

		// increment / decrement

		rb_tree_primitive_iterator<Tree,IsConst>& operator++(void) noexcept
		{
			if(state!=rb_tree_state_parent)
			{
				if(node->left!=nullptr)
				{
					node=node->left;
					state=rb_tree_state_left;
				}
				else if(node->right!=nullptr)
				{
					node=node->right;
					state=rb_tree_state_right;
				}
				else if(node!=node->parent->parent&&node!=node->parent->right)
				{
					node=node->parent->right;
					state=rb_tree_state_sibling;
				}
				else
				{
					node=node->parent;
					state=rb_tree_state_parent;
				}
			}
			else
			{
				if(node!=node->parent->parent&&node!=node->parent->right)
				{
					node=node->parent->right;
					state=rb_tree_state_sibling;
				}
				else
					node=node->parent;
			}
			return *this;
		}
		rb_tree_primitive_iterator<Tree,IsConst>& operator--(void) noexcept
		{
			if(state!=rb_tree_state_parent)
			{
				if(node->right!=nullptr)
				{
					node=node->right;
					state=rb_tree_state_right;
				}
				else if(node->left!=nullptr)
				{
					node=node->left;
					state=rb_tree_state_left;
				}
				else if(node!=node->parent->parent&&node!=node->parent->left)
				{
					node=node->parent->left;
					state=rb_tree_state_sibling;
				}
				else
				{
					node=node->parent;
					state=rb_tree_state_parent;
				}
			}
			else
			{
				if(node!=node->parent->parent&&node!=node->parent->left)
				{
					node=node->parent->left;
					state=rb_tree_state_sibling;
				}
				else
					node=node->parent;
			}
			return *this;
		}
		rb_tree_primitive_iterator<Tree,IsConst> operator++(int) noexcept
		{
			iterator_type tmp(*this);
			this->operator++();
			return tmp;
		}
		rb_tree_primitive_iterator<Tree,IsConst> operator--(int) noexcept
		{
			iterator_type tmp(*this);
			this->operator--();
			return tmp;
		}

		// relational operators:

		template <bool b>
		bool operator==(const rb_tree_primitive_iterator<Tree,b>& rhs) const noexcept
		{
			return (node==rhs.get_pointer());
		}
		template <bool b>
		bool operator!=(const rb_tree_primitive_iterator<Tree,b>& rhs) const noexcept
		{
			return (node!=rhs.get_pointer());
		}
	private:
		node_pointer       node;
		rb_tree_node_state state;
	};

	// Template class rb_tree_node_allocator
	template <class T,class Allocator>
	class rb_tree_node_allocator: public Allocator
	{
	public:
		// types:

		typedef Allocator                                                  allocator_type;
		typedef typename rb_tree_node<T>::node_type                        tree_node_type;
		typedef typename Allocator::template rebind<tree_node_type>::other node_allocator_type;
		typedef ::std::allocator_traits<allocator_type>                    allocator_traits_type;
		typedef ::std::allocator_traits<node_allocator_type>               node_allocator_traits_type;

		typedef typename node_allocator_traits_type::size_type             node_size_type;
		typedef typename node_allocator_traits_type::difference_type       node_difference_type;
		typedef typename node_allocator_traits_type::value_type            node_type;
		typedef typename node_allocator_traits_type::pointer               node_pointer;

		// construct/copy/destroy:

		rb_tree_node_allocator(void)
			: Allocator()
		{}
		explicit rb_tree_node_allocator(const Allocator& alloc)
			: Allocator(alloc)
		{}
		explicit rb_tree_node_allocator(Allocator&& alloc)
			: Allocator(::std::forward<Allocator>(alloc))
		{}
		~rb_tree_node_allocator(void)
		{}

		// rb_tree_node_allocator operations:

		allocator_type get_allocator(void) const noexcept
		{
			return *static_cast<const allocator_type*>(this);
		}
		node_size_type max_size(void) const noexcept
		{
			return node_alloc.max_size();
		}
	protected:
		template<class ...Args>
		node_pointer create_node(Args&&... args)
		{
			node_pointer p=node_alloc.allocate(1);
			get_allocator().construct(::std::addressof(p->data),::std::forward<Args>(args)...);
			return p;
		}
		void destroy_node(const node_pointer p)
		{
			get_allocator().destroy(::std::addressof(p->data));
			node_alloc.deallocate(p,1);
		}
	private:
		node_allocator_type node_alloc;
	};

	// Template class rb_tree
	template <class Key,class Value,class KeyOfValue,class KeyCompare=::std::less<Key>,class Allocator=::std::allocator<Value> >
	class rb_tree: public rb_tree_node_allocator<Value,Allocator>
	{
	public:
		// types:

		typedef Key                                                    key_type;
		typedef Allocator                                              allocator_type;
		typedef rb_tree_node_allocator<Value,Allocator>                node_allocator_type;
		typedef rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>	   tree_type;
		typedef typename node_allocator_type::node_type                node_type;
		typedef typename node_allocator_type::node_pointer             node_pointer;
		typedef typename allocator_type::value_type                    value_type;
		typedef typename allocator_type::pointer                       pointer;
		typedef typename allocator_type::const_pointer                 const_pointer;
		typedef typename allocator_type::reference                     reference;
		typedef typename allocator_type::const_reference               const_reference;
		typedef typename allocator_type::size_type                     size_type;
		typedef typename allocator_type::difference_type               difference_type;

		typedef rb_tree_iterator<tree_type,false>                      iterator;
		typedef rb_tree_iterator<tree_type,true>                       const_iterator;
		typedef rb_tree_primitive_iterator<tree_type,false>            primitive_iterator;
		typedef rb_tree_primitive_iterator<tree_type,true>             const_primitive_iterator;
		typedef ::std::reverse_iterator<iterator>                      reverse_iterator;
		typedef ::std::reverse_iterator<const_iterator>                const_reverse_iterator;
		typedef ::std::reverse_iterator<primitive_iterator>            reverse_primitive_iterator;
		typedef ::std::reverse_iterator<const_primitive_iterator>      const_reverse_primitive_iterator;

		// construct/copy/destroy:

		explicit rb_tree(const Allocator& alloc=Allocator())
			: node_allocator_type(alloc)
		{
			create_header();
		}
		explicit rb_tree(size_type n,const Allocator& alloc=Allocator())
			: node_allocator_type(alloc)
		{
			create_header();
			for(; n>0; --n)
				insert_equal(value_type());
		}
		rb_tree(size_type n,const Value& value,const Allocator& alloc=Allocator())
			: node_allocator_type(alloc)
		{
			create_header();
			for(; n>0; --n)
				insert_equal(value);
		}
		rb_tree(const_iterator first,const_iterator last,const Allocator& alloc=Allocator())
			: node_allocator_type(alloc)
		{
			create_header();
			for(; first!=last; ++first)
				insert_equal(*first);
		}
		rb_tree(const rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>& x)
			: node_allocator_type(x.get_allocator())
		{
			create_header();
			if(x.header->parent!=nullptr)
				copy_root(x.header->parent);
		}
		rb_tree(const rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>& x,const Allocator& alloc)
			: node_allocator_type(alloc)
		{
			create_header();
			if(x.header->parent!=nullptr)
				copy_root(x.header->parent);
		}
		rb_tree(rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>&& x)
			: node_allocator_type(x.get_allocator())
		{
			create_header();
			if(this!=&x)
			{
				node_pointer p=header;
				header=x.header;
				x.header=p;
			}
		}
		rb_tree(rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>&& x,const Allocator& alloc)
			: node_allocator_type(alloc)
		{
			create_header();
			if(this!=&x)
			{
				node_pointer p=header;
				header=x.header;
				x.header=p;
			}
		}
		rb_tree(::std::initializer_list<Value> ilist,const Allocator& alloc=Allocator())
			: node_allocator_type(alloc)
		{
			create_header();
			typename ::std::initializer_list<Value>::iterator i=ilist.begin();
			while(i!=ilist.end())
				insert_equal(*i++);
		}
		virtual ~rb_tree(void)
		{
			clear();
			destroy_header();
		}
		rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>& operator=(const rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>& x)
		{
			if(this!=&x)
			{
				clear();
				if(x.header->parent!=nullptr)
					copy_root(x.header->parent);
			}
			return (*this);
		}
		rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>& operator=(rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>&& x)
		{
			if(this!=&x)
			{
				node_pointer p=header;
				header=x.header;
				x.header=p;
			}
			return (*this);
		}
		rb_tree& operator=(::std::initializer_list<Value> ilist)
		{
			clear();
			typename ::std::initializer_list<Value>::iterator i=ilist.begin();
			while(i!=ilist.end())
				insert_equal(*i++);
			return (*this);
		}
		void assign(const_iterator first,const_iterator last)
		{
			clear();
			for(; first!=last; ++first)
				insert_equal(*first);
		}
		void assign(size_type n,const Value& value)
		{
			clear();
			for(; n>0; --n)
				insert_equal(value);
		}
		void assign(::std::initializer_list<Value> ilist)
		{
			clear();
			typename ::std::initializer_list<Value>::iterator i=ilist.begin();
			while(i!=ilist.end())
				insert_equal(*i++);
		}
		void assign_unique(const_iterator first,const_iterator last)
		{
			clear();
			for(; first!=last; ++first)
				insert_unique(*first);
		}
		void assign_unique(const Value& value)
		{
			clear();
			insert_unique(value);
		}
		void assign_unique(::std::initializer_list<Value> ilist)
		{
			clear();
			typename ::std::initializer_list<Value>::iterator i=ilist.begin();
			while(i!=ilist.end())
				insert_unique(*i++);
		}

		// iterators:

		iterator begin(void) noexcept
		{
			return iterator(header->left);
		}
		const_iterator begin(void) const noexcept
		{
			return const_iterator(header->left);
		}
		const_iterator cbegin(void) const noexcept
		{
			return const_iterator(header->left);
		}
		iterator end(void) noexcept
		{
			return iterator(header);
		}
		const_iterator end(void) const noexcept
		{
			return const_iterator(header);
		}
		const_iterator cend(void) const noexcept
		{
			return const_iterator(header);
		}
		reverse_iterator rbegin(void) noexcept
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator rbegin() const noexcept
		{
			return const_reverse_iterator(end());
		}
		const_reverse_iterator crbegin() const noexcept
		{
			return const_reverse_iterator(cend());
		}
		reverse_iterator rend(void) noexcept
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator rend(void) const noexcept
		{
			return const_reverse_iterator(begin());
		}
		const_reverse_iterator crend(void) const noexcept
		{
			return const_reverse_iterator(cbegin());
		}

		primitive_iterator pbegin(void) noexcept
		{
			return primitive_iterator(get_root());
		}
		const_primitive_iterator pbegin(void) const noexcept
		{
			return const_primitive_iterator(get_root());
		}
		const_primitive_iterator cpbegin(void) const noexcept
		{
			return const_primitive_iterator(get_root());
		}
		primitive_iterator pend(void) noexcept
		{
			return primitive_iterator(header);
		}
		const_primitive_iterator pend(void) const noexcept
		{
			return const_primitive_iterator(header);
		}
		const_primitive_iterator cpend(void) const noexcept
		{
			return const_primitive_iterator(header);
		}
		reverse_primitive_iterator rpbegin(void) noexcept
		{
			return reverse_primitive_iterator(pend());
		}
		const_reverse_primitive_iterator rpbegin(void) const noexcept
		{
			return const_reverse_primitive_iterator(pend());
		}
		const_reverse_primitive_iterator crpbegin(void) const noexcept
		{
			return const_reverse_primitive_iterator(cpend());
		}
		reverse_primitive_iterator rpend(void) noexcept
		{
			return reverse_primitive_iterator(pbegin());
		}
		const_reverse_primitive_iterator rpend(void) const noexcept
		{
			return const_reverse_primitive_iterator(pbegin());
		}
		const_reverse_primitive_iterator crpend(void) const noexcept
		{
			return const_reverse_primitive_iterator(cpbegin());
		}

		// capacity:

		size_type size(void) const noexcept
		{
			size_type count=0;
			const_iterator first=cbegin();
			const_iterator last=cend();
			for(; first!=last; ++first)
				++count;
			return count;
		}

		bool empty(void) const noexcept
		{
			return (header->parent==nullptr);
		}

		// modifiers:

		template <class... Args>
		iterator emplace_equal(Args&&... args)
		{
			return iterator(insert_equal_node(::std::forward<Args>(args)...));
		}

		iterator insert_equal(const value_type& value)
		{
			return iterator(insert_equal_node(value));
		}

		iterator insert_equal(value_type&& value)
		{
			return iterator(insert_equal_node(::std::forward<value_type>(value)));
		}

		void insert_equal(size_type n,const value_type& value)
		{
			for(; n>0; --n)
				insert_equal_node(value);
		}

		template <class InputIterator>
		void insert_equal(InputIterator first,InputIterator last)
		{
			for(; first!=last; ++first)
				insert_equal_node(*first);
		}

		template <class InputIterator>
		void insert_equal(::std::initializer_list<value_type> ilist)
		{
			insert_equal(ilist.begin(),ilist.end());
		}

		template <class... Args>
		iterator emplace_unique(Args&&... args)
		{
			return iterator(insert_unique_node(::std::forward<Args>(args)...));
		}

		iterator insert_unique(const value_type& value)
		{
			return iterator(insert_unique_node(value));
		}

		iterator insert_unique(value_type&& value)
		{
			return iterator(insert_unique_node(::std::forward<value_type>(value)));
		}

		template <class InputIterator>
		void insert_unique(InputIterator first,InputIterator last)
		{
			for(; first!=last; ++first)
				insert_unique_node(*first);
		}

		void insert_unique(::std::initializer_list<value_type> ilist)
		{
			insert_unique(ilist.begin(),ilist.end());
		}

		void erase(iterator position)
		{
			erase_node(position.get_pointer(),header->parent);
		}

		void erase(iterator first,iterator last)
		{
			if(first==begin()&&last==end())
				clear();
			else
				while(first!=last)
					erase(first++);
		}

		size_type erase(const key_type& key)
		{
			size_type count=0;
			iterator first=lower_bound(key);
			iterator last=upper_bound(key);
			while(first!=last)
			{
				++count;
				erase(first++);
			}
			return count;
		}

		void swap(rb_tree<Key,Value,KeyOfValue,KeyCompare,Allocator>& rhs) noexcept
		{
			if(this!=&rhs)
			{
				node_pointer p=header;
				header=rhs.header;
				rhs.header=p;
			}
		}

		void clear(void)
		{
			if(header->parent!=nullptr)
			{
				erase_root();
				header->parent=nullptr;
				header->left=header;
				header->right=header;
			}
		}

		// Operations:

		iterator find(const key_type& key) noexcept
		{
			return iterator(find_node(key));
		}
		const_iterator find(const key_type& key) const noexcept
		{
			return const_iterator(find_node(key));
		}

		iterator lower_bound(const key_type& key) noexcept
		{
			return iterator(lower_bound_node(key));
		}
		const_iterator lower_bound(const key_type& key) const noexcept
		{
			return const_iterator(lower_bound_node(key));
		}

		iterator upper_bound(const key_type& key) noexcept
		{
			return iterator(upper_bound_node(key));
		}
		const_iterator upper_bound(const key_type& key) const noexcept
		{
			return const_iterator(upper_bound_node(key));
		}
	private:
		node_pointer get_root(void) const noexcept
		{
			return (header->parent!=nullptr ? header->parent : header);
		}

		node_pointer get_leftmost(const node_pointer parent) const noexcept
		{
			node_pointer leftmost=parent;
			while(leftmost->left)
				leftmost=leftmost->left;
			return leftmost;
		}

		node_pointer get_rightmost(const node_pointer parent) const noexcept
		{
			node_pointer rightmost=parent;
			while(rightmost->right)
				rightmost=rightmost->right;
			return rightmost;
		}

		void create_header(void)
		{
			header=this->create_node(value_type());
			header->color=rb_tree_red;
			header->parent=nullptr;
			header->left=header;
			header->right=header;
		}

		void destroy_header(void)
		{
			this->destroy_node(header);
		}

		node_pointer find_node(const key_type& key) const noexcept
		{
			node_pointer pre=header;
			node_pointer cur=header->parent;
			while(cur!=nullptr)
			{
				if(!KeyCompare()(KeyOfValue()(cur->data),key))
				{
					pre=cur;
					cur=cur->left;
				}
				else
					cur=cur->right;
			}
			if(pre==header||KeyCompare()(key,KeyOfValue()(pre->data)))
			{
				pre=header;
			}
			return pre;
		}

		node_pointer lower_bound_node(const key_type& key) const noexcept
		{
			node_pointer pre=header;
			node_pointer cur=header->parent;
			while(cur!=nullptr)
			{
				if(!KeyCompare()(KeyOfValue()(cur->data),key))
				{
					pre=cur;
					cur=cur->left;
				}
				else
					cur=cur->right;
			}
			return pre;
		}

		node_pointer upper_bound_node(const key_type& key) const noexcept
		{
			node_pointer pre=header;
			node_pointer cur=header->parent;
			while(cur!=nullptr)
			{
				if(KeyCompare()(key,KeyOfValue()(cur->data)))
				{
					pre=cur;
					cur=cur->left;
				}
				else
					cur=cur->right;
			}
			return pre;
		}

		template<class ...Args>
		node_pointer insert_equal_node(Args&&... args)
		{
			node_pointer node=this->create_node(::std::forward<Args>(args)...);
			node->left=nullptr;
			node->right=nullptr;
			if(header->parent==nullptr)
			{
				node->parent=header;
				header->parent=node;
				header->left=node;
				header->right=node;
			}
			else
			{
				node_pointer ptr=header->parent;
				while(ptr!=nullptr)
				{
					if(KeyCompare()(KeyOfValue()(node->data),KeyOfValue()(ptr->data)))
					{
						if(ptr->left!=nullptr)
							ptr=ptr->left;
						else
						{
							node->parent=ptr;
							ptr->left=node;
							if(ptr==header->left)
								header->left=node;
							ptr=nullptr;
						}
					}
					else
					{
						if(ptr->right!=nullptr)
							ptr=ptr->right;
						else
						{
							node->parent=ptr;
							ptr->right=node;
							if(ptr==header->right)
								header->right=node;
							ptr=nullptr;
						}
					}
				}
				insert_rebalance(node,header->parent);
			}
			return node;
		}

		template<class ...Args>
		node_pointer insert_unique_node(Args&&... args)
		{
			node_pointer node=nullptr;
			value_type value=value_type(::std::forward<Args>(args)...);
			if(header->parent==nullptr)
			{
				node=this->create_node(::std::forward<value_type>(value));
				node->parent=header;
				node->left=nullptr;
				node->right=nullptr;
				header->parent=node;
				header->left=node;
				header->right=node;
			}
			else
			{
				node_pointer ptr=header->parent;
				while(ptr!=nullptr)
				{
					if(KeyCompare()(KeyOfValue()(value),KeyOfValue()(ptr->data)))
					{
						if(ptr->left!=nullptr)
							ptr=ptr->left;
						else
						{
							node=this->create_node(::std::forward<value_type>(value));
							node->parent=ptr;
							node->left=nullptr;
							node->right=nullptr;
							ptr->left=node;
							if(ptr==header->left)
								header->left=node;
							ptr=nullptr;
						}
					}
					else
					{
						if(!KeyCompare()(KeyOfValue()(ptr->data),KeyOfValue()(value)))
							return ptr;
						if(ptr->right!=nullptr)
							ptr=ptr->right;
						else
						{
							node=this->create_node(::std::forward<value_type>(value));
							node->parent=ptr;
							node->left=nullptr;
							node->right=nullptr;
							ptr->right=node;
							if(ptr==header->right)
								header->right=node;
							ptr=nullptr;
						}
					}
				}
				insert_rebalance(node,header->parent);
			}
			return node;
		}

		void erase_node(node_pointer position,node_pointer& root)
		{
			rb_tree_color_type color;
			node_pointer p=nullptr;
			node_pointer x=nullptr;
			node_pointer y=position;
			node_pointer z=position;
			if(y->left==nullptr||y->right==nullptr)
			{
				p=y->parent;
				x=(y->left==nullptr) ? y->right : y->left;
				if(x!=nullptr)
					x->parent=y->parent;
				if(z==root)
					root=x;
				else if(z==z->parent->left)
					z->parent->left=x;
				else
					z->parent->right=x;
				if(z==header->left)
					header->left=(z->right==nullptr) ? z->parent : get_leftmost(x);
				if(z==header->right)
					header->right=(z->left==nullptr) ? z->parent : get_rightmost(x);
			}
			else
			{
				y=z->right;
				while(y->left!=nullptr)
					y=y->left;
				x=y->right;
				if(y==z->right)
				{
					p=y;
					z->left->parent=y;
					y->left=z->left;
				}
				else
				{
					p=y->parent;
					z->left->parent=y;
					z->right->parent=y;
					y->left=z->left;
					y->right=z->right;
					y->parent->left=x;
					if(x!=nullptr)
						x->parent=y->parent;
				}
				if(z==root)
					root=y;
				else if(z==z->parent->left)
					z->parent->left=y;
				else
					z->parent->right=y;
				y->parent=z->parent;
				color=y->color;
				y->color=z->color;
				z->color=color;
				y=z;
			}
			if(y->color!=rb_tree_red)
				erase_rebalance(p,x,root);
			this->destroy_node(y);
		}

		void copy_root(const node_pointer root)
		{
			node_pointer src=root;
			node_pointer dst=header;
			rb_tree_node_state state=rb_tree_state_root;
			node_pointer node=this->create_node(root->data);
			node->color=root->color;
			node->parent=dst;
			node->left=nullptr;
			node->right=nullptr;
			dst->parent=node;
			dst=node;
			do
			{
				if(state!=rb_tree_state_parent)
				{
					if(src->left!=nullptr)
					{
						src=src->left;
						state=rb_tree_state_left;
						node=this->create_node(src->data);
						node->color=src->color;
						node->parent=dst;
						node->left=nullptr;
						node->right=nullptr;
						dst->left=node;
						dst=node;
					}
					else if(src->right!=nullptr)
					{
						src=src->right;
						state=rb_tree_state_right;
						node=this->create_node(src->data);
						node->color=src->color;
						node->parent=dst;
						node->left=nullptr;
						node->right=nullptr;
						dst->right=node;
						dst=node;
					}
					else if(src!=src->parent->parent&&src!=src->parent->right)
					{
						src=src->parent->right;
						state=rb_tree_state_sibling;
						node=this->create_node(src->data);
						node->color=src->color;
						node->parent=dst->parent;
						node->left=nullptr;
						node->right=nullptr;
						dst->parent->right=node;
						dst=node;
					}
					else
					{
						src=src->parent;
						dst=dst->parent;
						state=rb_tree_state_parent;
					}
				}
				else
				{
					if(src!=src->parent->parent&&src!=src->parent->right)
					{
						src=src->parent->right;
						state=rb_tree_state_sibling;
						node=this->create_node(src->data);
						node->color=src->color;
						node->parent=dst->parent;
						node->left=nullptr;
						node->right=nullptr;
						dst->parent->right=node;
						dst=node;
					}
					else
					{
						src=src->parent;
						dst=dst->parent;
					}
				}
			} while(src!=root);
			header->left=get_leftmost(header->parent);
			header->right=get_rightmost(header->parent);
		}

		void erase_root(void)
		{
			node_pointer next=nullptr;
			node_pointer cur=header->parent;
			do
			{
				while(cur->left!=nullptr)
					cur=cur->left;
				if(cur->right!=nullptr)
					cur=cur->right;
				else
				{
					next=cur->parent;
					if(cur==next->left)
						next->left=nullptr;
					else
						next->right=nullptr;
					this->destroy_node(cur);
					cur=next;
				}
			} while(cur!=header);
		}

		void rotate_left(node_pointer x,node_pointer& root) const noexcept
		{
			node_pointer y=x->right;
			x->right=y->left;
			if(y->left!=nullptr)
				y->left->parent=x;
			y->parent=x->parent;
			if(x==root)
				root=y;
			else if(x==x->parent->left)
				x->parent->left=y;
			else
				x->parent->right=y;
			y->left=x;
			x->parent=y;
		}

		void rotate_right(node_pointer x,node_pointer& root) const noexcept
		{
			node_pointer y=x->left;
			x->left=y->right;
			if(y->right!=nullptr)
				y->right->parent=x;
			y->parent=x->parent;
			if(x==root)
				root=y;
			else if(x==x->parent->right)
				x->parent->right=y;
			else
				x->parent->left=y;
			y->right=x;
			x->parent=y;
		}

		void insert_rebalance(node_pointer x,node_pointer& root) const noexcept
		{
			x->color=rb_tree_red;
			while(x!=root&&x->parent->color==rb_tree_red)
			{
				if(x->parent==x->parent->parent->left)
				{
					node_pointer y=x->parent->parent->right;
					if(y&&y->color==rb_tree_red)
					{
						x->parent->color=rb_tree_black;
						y->color=rb_tree_black;
						x->parent->parent->color=rb_tree_red;
						x=x->parent->parent;
					}
					else
					{
						if(x==x->parent->right)
						{
							x=x->parent;
							rotate_left(x,root);
						}
						x->parent->color=rb_tree_black;
						x->parent->parent->color=rb_tree_red;
						rotate_right(x->parent->parent,root);
					}
				}
				else
				{
					node_pointer y=x->parent->parent->left;
					if(y&&y->color==rb_tree_red)
					{
						x->parent->color=rb_tree_black;
						y->color=rb_tree_black;
						x->parent->parent->color=rb_tree_red;
						x=x->parent->parent;
					}
					else
					{
						if(x==x->parent->left)
						{
							x=x->parent;
							rotate_right(x,root);
						}
						x->parent->color=rb_tree_black;
						x->parent->parent->color=rb_tree_red;
						rotate_left(x->parent->parent,root);
					}
				}
			}
			root->color=rb_tree_black;
		}

		void erase_rebalance(node_pointer p,node_pointer x,node_pointer& root) const noexcept
		{
			while(x!=root&&(x==nullptr||x->color==rb_tree_black))
			{
				if(x==p->left)
				{
					node_pointer s=p->right;
					if(s->color==rb_tree_red)
					{
						s->color=rb_tree_black;
						p->color=rb_tree_red;
						rotate_left(p,root);
						s=p->right;
					}
					if((s->left==nullptr||s->left->color==rb_tree_black)&&
						(s->right==nullptr||s->right->color==rb_tree_black))
					{
						s->color=rb_tree_red;
						x=p;
						p=x->parent;
					}
					else
					{
						if(s->right==nullptr||s->right->color==rb_tree_black)
						{
							s->color=rb_tree_red;
							s->left->color=rb_tree_black;
							rotate_right(s,root);
							s=p->right;
						}
						s->color=p->color;
						p->color=rb_tree_black;
						s->right->color=rb_tree_black;
						rotate_left(p,root);
						break;
					}
				}
				else
				{
					node_pointer s=p->left;
					if(s->color==rb_tree_red)
					{
						s->color=rb_tree_black;
						p->color=rb_tree_red;
						rotate_right(p,root);
						s=p->left;
					}
					if((s->right==nullptr||s->right->color==rb_tree_black)&&
						(s->left==nullptr||s->left->color==rb_tree_black))
					{
						s->color=rb_tree_red;
						x=p;
						p=x->parent;
					}
					else
					{
						if(s->left==nullptr||s->left->color==rb_tree_black)
						{
							s->color=rb_tree_red;
							s->right->color=rb_tree_black;
							rotate_left(s,root);
							s=p->left;
						}
						s->color=p->color;
						p->color=rb_tree_black;
						s->left->color=rb_tree_black;
						rotate_right(p,root);
						break;
					}
				}
			}
			if(x!=nullptr)
				x->color=rb_tree_black;
		}
	private:
		node_pointer header;
	};

} // namespace core

#endif



///*
////RB-tree���е���ɫ����
//typedef bool __rb_tree_color_type;
//const __rb_tree_color_type __rb_tree_red=false;  //��ɫ������Ϊ0
//const __rb_tree_color_type __rb_tree_black=true;  //��ɫ������Ϊ1 
////RB-tree�ڵ�����ṹ
//struct __rb_tree_node_base {
//	typedef __rb_tree_color_type  color_type;
//	typedef __rb_tree_node_base* base_ptr;
//
//	color_type color;        // �ڵ���ɫ,�Ǻڼ���
//	base_ptr parent;        // ָ�򸸽ڵ�,����RB-treeʱ��Ҫ�����丸�ڵ�
//	base_ptr left;         // ָ�����ӽڵ�
//	base_ptr right;        // ָ�����ӽڵ�
//
//	// һֱ�����ߣ������ҵ����������Сֵ�ڵ�
//	// ����������������
//	static base_ptr minimum(base_ptr x)
//	{
//		while(x->left!=0) x=x->left;
//		return x;
//	}
//	// һֱ�����ߣ������ҵ�����������ֵ�ڵ�
//	// ����������������
//	static base_ptr maximum(base_ptr x)
//	{
//		while(x->right!=0) x=x->right;
//		return x;
//	}
//};
//
//// �����Ľڵ㶨�壬����˫��ڵ�ṹ
//// �����в�����ģ�����
//template <class Value>
//struct __rb_tree_node: public __rb_tree_node_base
//{
//	typedef __rb_tree_node<Value>* link_type;
//	Value value_field;    // ���ڵ�ֵ
//};
//
//
//struct __rb_tree_base_iterator
//{
//	typedef __rb_tree_node_base::base_ptr base_ptr;
//	typedef bidirectional_walker_tag iterator_category;
//	typedef int difference_type;
//
//	base_ptr node;    // �������Ӻ�����Ľڵ�
//
//	// Ѱ�Ҹýڵ�ĺ�̽ڵ���
//	void increment()
//	{
//		if(node->right!=0) { // ����������ӽڵ�
//			node=node->right;       // ֱ���������ӽڵ���
//			while(node->left!=0) // Ȼ��һֱ���������ߣ�ֱ��������Ϊ��
//				node=node->left;
//		}
//		else {                    // û�����ӽڵ�
//			base_ptr y=node->parent;    // �ҳ����ڵ�
//			while(node==y->right) {    // ����ýڵ�һֱΪ���ĸ��ڵ�����ӽڵ�
//				node=y;                       // ��һֱ�����ң�ֱ����Ϊ���ӽڵ�Ϊֹ
//				y=y->parent;
//			}
//			if(node->right!=y)      // ����ʱ�ýڵ㲻Ϊ���ĸ��ڵ�����ӽڵ�
//				node=y;                // ��ʱ�ĸ��ڵ㼴ΪҪ�ҵĺ�̽ڵ�
//								// �����ʱ��node��ΪҪ�ҵĺ�̽ڵ㣬��Ϊ�������������
//								// ����ҪѰ�Ҹ��ڵ����һ���ڵ㣬�����ڵ�û�����ӽڵ�
//								// ���������Ҫ���rbtree��header�ڵ��������ƣ�����ὲ��
//		}
//	}
//
//	// Ѱ�Ҹýڵ����ǰ�ýڵ�
//	void decrement()
//	{
//		if(node->color==__rb_tree_red&& // ����˽ڵ��Ǻ�ڵ�
//			node->parent->parent==node)       // �Ҹ��ڵ�ĸ��ڵ�����Լ�
//			node=node->right;                               // �������ӽڵ㼴Ϊ��ǰ�ýڵ�
//// �������������nodeΪheaderʱ����nodeΪend()ʱ
//// ע�⣺header�����ӽڵ�Ϊmostright��ָ����������max�ڵ㣬������н���
//		else if(node->left!=0) {                 // ����������ӽڵ�
//			base_ptr y=node->left;            // �������ӽڵ���
//			while(y->right!=0)               // Ȼ��һֱ�����ң�֪��������Ϊ��
//				y=y->right;
//			node=y;                          // ���ҵ�ǰ�ýڵ�
//		}
//		else {                                   // ����ýڵ㲻�������ӽڵ�
//			base_ptr y=node->parent;         // �������ĸ��ڵ���
//			while(node==y->left) {          // ������������ĸ��ӽڵ�����ӽڵ�
//				node=y;                   // ��һֱ���ϲ�
//				y=y->parent;
//			}                               // ֱ������Ϊ���ڵ�����ӽڵ�δ֪
//			node=y;                       // ��ʱ���ĸ��ڵ㼴ΪҪ�ҵ�ǰ���ڵ�
//		}
//	}
//};
//
//template <class Value,class Ref,class Ptr>
//struct __rb_tree_iterator: public __rb_tree_base_iterator{
//	//�ͱ�����
//	typedef Value value_type;
//	typedef Ref reference;
//	typedef Ptr pointer;
//	typedef __rb_tree_iterator<Value,Value&,Value*> iterator;
//	typedef __rb_tree_iterator<Value,const Value&,const Vallue*> const_iterator;
//	typedef __rb_tree_iterator<Value,Ref,Ptr> self;
//	typedef __rb_tree_node<Value>* link_type;
//
//	// �������Ĺ��캯��
//	__rb_tree_iterator() {}
//	__rb_tree_iterator(link_type x) {
//		node=x;
//	}
//	__rb_tree_iterator(const iterator& it) {
//		node=it.node;
//	}
//	// ����ͳ�Ա���ʺ�����������*��->������
//	reference operator*() const {
//		return link_type(node)->value_field;
//	}
//	pointer operator->() const {
//		return &(operator*());
//	}
//	// ǰ��++�ͺ���++
//	self& operator++() {
//		increment(); return *this;
//	}
//	self operator++(int) {
//		self tmp=*this;
//		increment();        // ֱ�ӵ���increment����
//		return tmp;
//	}
//	// ǰ��--�ͺ���--
//	self& operator--() {
//		decrement(); return *this;
//	}
//	self operator--(int) {
//		self tmp=*this;
//		decrement();        // ֱ�ӵ���decrement����
//		return tmp;
//	}
//};
//
//
//template <class Key,class Value,class KeyOfValue,class Compare>
//	class rb_tree {
//	protected:
//		typedef void* void_pointer;
//		typedef __rb_tree_node_base* base_ptr;
//		typedef __rb_tree_node<Value> rb_tree_node;
//		
//		typedef __rb_tree_color_type color_type;
//	public:
//		// һЩ��������
//		typedef Key key_type;
//		typedef Value value_type;
//		typedef value_type* pointer;
//		typedef const value_type* const_pointer;
//		typedef value_type& reference;
//		typedef const value_type& const_reference;
//		typedef rb_tree_node* link_type;
//		typedef size_t size_type;
//		typedef int difference_type;
//	protected:
//		//...........................................
//	protected:
//		// RB-tree�����ݽṹ
//		size_type node_count; // ��¼���Ľڵ����
//		link_type header;         // header�ڵ����
//		Compare key_compare;  // �ڵ��ļ�ֵ��С�Ƚ�׼��
//
//		// ����������������ȡ��header�ĳ�Ա
//		link_type& root() const {
//			return (link_type&)header->parent;
//		}
//		link_type& leftmost() const {
//			return (link_type&)header->left;
//		}
//		link_type& rightmost() const {
//			return (link_type&)header->right;
//		}
//
//		// ����������������ȡ�ýڵ�ĳ�Ա
//		static link_type& left(link_type x) {
//			return (link_type&)(x->left);
//		}
//		static link_type& right(link_type x) {
//			return (link_type&)(x->right);
//		}
//		static link_type& parent(link_type x) {
//			return (link_type&)(x->parent);
//		}
//		static reference value(link_type x) {
//			return x->value_field;
//		}
//		static const Key& key(link_type x) {
//			return KeyOfValue()(value(x));
//		}
//		static color_type& color(link_type x) {
//			return (color_type&)(x->color);
//		}
//
//		// ����������������ȡ�ýڵ�ĳ�Ա������˫����ƣ�����������Ҫ��������
//		static link_type& left(base_ptr x) {
//			return (link_type&)(x->left);
//		}
//		static link_type& right(base_ptr x) {
//			return (link_type&)(x->right);
//		}
//		static link_type& parent(base_ptr x) {
//			return (link_type&)(x->parent);
//		}
//		static reference value(base_ptr x) {
//			return ((link_type)x)->value_field;
//		}
//		static const Key& key(base_ptr x) {
//			return KeyOfValue()(value(link_type(x)));
//		}
//		static color_type& color(base_ptr x) {
//			return (color_type&)(link_type(x)->color);
//		}
//
//		// ��ȡ����ֵ�ͼ�Сֵ������ֱ�ӵ��ýڵ�ṹ�ĺ�������
//		static link_type minimum(link_type x) {
//			return (link_type)__rb_tree_node_base::minimum(x);
//		}
//		static link_type maximum(link_type x) {
//			return (link_type)__rb_tree_node_base::maximum(x);
//		}
//	public:
//		// RBTree�ĵ���������
//		typedef __rb_tree_iterator<value_type,reference,pointer> iterator;
//		typedef __rb_tree_iterator<value_type,const_reference,const_pointer>
//			const_iterator;
//	private:
//		//............................................
//		void init() {   //����һ����tree
//			header=get_node();   //����һ���ڵ�ռ䣬��headerָ����
//			color(header)=__rb_tree_red;  //��headerΪ��ɫ��������
//											//root��header���ֿ�
//			root()=0;
//			leftmost()=header;       //header�����ӽڵ�Ϊ�Լ�
//			rightmost()=header;      //header�����ӽڵ�Ϊ�Լ�
//		}
//	public:
//		rb_tree(const Compare& comp=Compare()):node_count(0),key_compare(comp){init();}
//	public:
//		Compare key_comp() const {
//			return key_compare;
//		}  // ���ں�����Դ������ܣ����Ա��봫��һ���Ƚ�������
//		iterator begin() {
//			return leftmost();
//		}        // RBTree����ʼ�ڵ�Ϊ�����Сֵ�ڵ�
//		const_iterator begin() const {
//			return leftmost();
//		}
//		iterator end() {
//			return header;
//		}                         // RBTree����ֹ�ڵ�Ϊ�ұ����ֵ�ڵ�
//		const_iterator end() const {
//			return header;
//		}
//		bool empty() const {
//			return node_count==0;
//		}    // �жϺ�����Ƿ�Ϊ��    
//		size_type size() const {
//			return node_count;
//		}     // ��ȡ������Ľڵ����
//		size_type max_size() const {
//			return size_type(-1);
//		}  // ��ȡ����������ڵ������
//								  // û�������ĸ����Ϊsizetype���ֵ
//};
//
//
//
//*/




