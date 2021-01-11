#pragma once
#ifndef WALKER_H
#define WALKER_H

#include"stdafx.h"


////walker_category
//struct input_walker_tag{};
//
//struct output_walker_tag{};
//
//struct forward_walker_tag:public input_walker_tag{};
//
//struct bidirectional_walker_tag:public forward_walker_tag{};
//
//struct random_access_walker_tag:public bidirectional_walker_tag{};


//walker_traits
template<typename _Walker>
struct walker_traits{
	typedef typename _Walker::iterator_category	iterator_category;			//����������
	typedef typename _Walker::value_type		value_type;					//��������ָ��Ԫ�ص�����
	typedef typename _Walker::difference_type	difference_type;			//��ʾ����������֮����������
	typedef typename _Walker::pointer			pointer;					//��������ָ��Ԫ�ص�ָ������
	typedef typename _Walker::reference			reference;					//��������ָ��Ԫ�ص���������
};

//Ϊnative pointer��Ƶ�walker_traitsƫ�ػ��汾
template<typename _Walker>
struct walker_traits<_Walker*>{
	typedef typename random_access_iterator_tag	iterator_category;
	typedef typename _Walker					value_type;					
	typedef typename int						difference_type;			
	typedef typename _Walker*					pointer;					
	typedef typename _Walker&					reference;					
};

//Ϊconst native pointer��Ƶ�walker_traitsƫ�ػ��汾
template<typename _Walker>
struct walker_traits<const _Walker*>{
	typedef typename random_access_iterator_tag	iterator_category;
	typedef typename _Walker					value_type;					
	typedef typename int						difference_type;			
	typedef typename const _Walker*				pointer;					
	typedef typename const _Walker&				reference;					
};





#endif // !WALKER_H