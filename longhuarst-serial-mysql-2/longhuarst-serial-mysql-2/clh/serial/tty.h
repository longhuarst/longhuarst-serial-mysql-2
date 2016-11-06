#ifndef __tty_H
#define __tty_H

#include <iostream>
#include <map>


namespace clh {


	namespace serial {



		class tty
		{
		public:
			tty();
			virtual ~tty();


			int init(std::string filepath, int speed, int databits, int stopbits, int parity);
			int init(std::string filepath, const char *format = "", ...);
			//int init(map<std::string, std::string> paramMap);
			int init(std::string filepath);
			int init(std::string filepath, int speed);

			int setSpeed(int speed);
			int setParity(int parity);
			int setDatabits(int databits);
			int setStopbits(int stopbits);
			int openfd(std::string filepath);
			int closefd();

			int getfd();
			int getBaudrate();
			int getDatabits();
			int getParity();
			int getStopbits();
		protected:
		private:

			//int _open();

			std::string path;
			int fd;
			int baudrate;
			int databits;
			int parity;
			int stopbits;


		};





	}









}

#endif //__tty_H