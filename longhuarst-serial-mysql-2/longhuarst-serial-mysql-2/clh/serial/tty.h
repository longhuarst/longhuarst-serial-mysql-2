#ifndef __tty_H
#define __tty_H

#include <iostream>



namespace clh {


	namespace serial {



		class tty
		{
		public:
			tty();
			virtual ~tty();

			int setSpeed(int speed);
			int setParity(int parity);
			int setDatabits(int databits);
			int setStopbits(int stopbits);
			int openfd(std::string filepath);
			int closefd();

			int getfd();
		protected:
		private:



			int fd;


		};





	}









}

#endif //__tty_H