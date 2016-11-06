#ifndef __epoll_H
#define __epoll_H

namespace clh{

	namespace epoll{
		
		class epoll
		{
		public:
			epoll();
			virtual ~epoll();

			int init();



		protected:
		private:
			int epfd;
			int nfds;
			vector<epoll_event> *events;



		};


	
	}
}


#endif //__epoll_H
