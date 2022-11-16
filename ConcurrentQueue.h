#include <memory>
#include <queue>
#include <thread>

template<typename T>
class ThreadSafeConcurrentQueue {

	private :
		mutable std::mutex mut ;
		std::queue<std::shared_ptr<T>> q ;
		std::condition_variable data_cond ;
	public :
		ThreadSafeConcurrentQueue () {}

		bool try_pop( T & value ) ;
		void push ( T new_value );
		bool empty() const ;
};