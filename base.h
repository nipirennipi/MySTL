#pragma once

#ifndef BASE_H
#define BASE_H

//base class for walker
template<typename MSContainer>
class MS_walker_base{
	
};

//base class for MS_container
template<typename Ty>
class MS_container_base{
public:
	virtual MS_container_base<Ty>* address()const=0;			//address of MS_container
};



#endif // !BASE_H

