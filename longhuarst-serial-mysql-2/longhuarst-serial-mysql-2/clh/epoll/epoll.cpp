#include "epoll.h"

#include <iostream>
#include "stdlib.h"
#include "errno.h"
#include "stdio.h"

#include "sys/epoll.h"





namespace clh {

	namespace epoll {

		epoll::epoll() :
			epfd(0),
			nfds(0)
		{
			std::cout << "epoll constructor" << std::endl;
		}


		epoll::~epoll()
		{
			std::cout << "epoll distructor" << std::endl;
		}


		int epoll::init()
		{
			std::cout << "epoll init" << std::endl;
		
			this->epfd = epoll_create1(EPOLL_CLOEXEC);

			if (this->epfd < 0) {
				perror("epoll_create1");
				exit(EXIT_FAILURE);
			}
			
			return 0;
		}


		int epoll::wait() {
			
			this->events = new vector<epoll_event>(10);

			while (1) {
				this->nfds = epoll_wait(this->epfd,this->events->data(),this->events->size(),-1);
				if (this->nfds < 0) {
					if (errno ==EINTR)
						continue;
					perror("epoll_wait");
					exit(EXIT_FAILURE);
				}
				if (this->nfds == this->events->size())
				{
					this->events->resize(this->events->size() * 2);
				}

				for (int i = 0; i < nfds;++i) {
					
				}
			}
		}

		





	}
}
