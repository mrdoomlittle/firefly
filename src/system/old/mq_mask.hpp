# ifndef __mq__mask__hpp
# define __mq__mask__hpp
# include <queue>
# include <boost/thread/mutex.hpp>

// not permanent
namespace mdl {
namespace firefly {
namespace system {
template<typename _T>
struct mq_mask {
	boost::mutex mutex;
	std::queue<_T> queue;
	void __inline__ push(_T __data) {
		this-> mutex.lock();
		this-> queue.push(__data);
		this-> mutex.unlock();
	}

	void __inline__ pop() {
		this-> mutex.lock();
		this-> queue.pop();
		this-> mutex.unlock();
	}
	_T __inline__ front() {
		return this-> queue.front();
	}
	bool __inline__ empty() {return this-> queue.empty();}

	std::size_t __inline__ size() {return this-> queue.size();}
};
}
}
}

# endif /*__mq__mask__hpp*/
