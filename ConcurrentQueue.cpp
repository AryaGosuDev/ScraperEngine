#include "ConcurrentQueue.h"

using namespace std ;

template<typename T>
bool ThreadSafeConcurrentQueue<T>::try_pop( T & value ) {
	lock_guard<mutex> lk ( mut) ;
	if ( q.empty() ) return false;
	value = move ( * q.front());
	q.pop() ;
	return true ;
}

template<typename T>
void ThreadSafeConcurrentQueue<T>::push ( T new_value ) {
	shared_ptr<T> data ( make_shared<T> ( move(new_value))) ;
	lock_guard<mutex> lk ( mut );
	if ( hashT.find ( new_value ) == end ( hashT ) ) 
		q.push(data);
	data_cond.notify_one() ;
}

template<typename T>
bool ThreadSafeConcurrentQueue<T>::empty() const {
	lock_guard<mutex> lk ( mut) ;
	return q.empty() ;
}