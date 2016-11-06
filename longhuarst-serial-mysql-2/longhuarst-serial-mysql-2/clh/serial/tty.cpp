#include "tty.h"
//#include "clh/debug/syspf.h"

#include <iostream>
#include "errno.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#include "sys/stat.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/fcntl.h"
#include "fcntl.h"
#include "stdarg.h"
#include "termio.h"//POSIX 终端控制定义

namespace clh {

	namespace serial {

		tty::tty() :
			path(""),
			fd(0),
			baudrate(115200),
			databits(8),
			parity('n'),
			stopbits(1)
		{
			std::cout << "clh/serial/tty constructor" << std::endl;
		}



		tty::~tty()
		{
			std::cout << "clh/serial/tty destructor" << std::endl;
			close(fd);
		}


		int tty::init(std::string filepath, int speed, int databits, int stopbits, int parity) {
			
			this->path = filepath;
			this->baudrate = speed;
			this->databits = databits;
			this->stopbits = stopbits;
			this->parity = parity;

			this->openfd(this->path);

			





		}

		int tty::init(std::string filepath, const char *format, ...) {
			va_list ap;

			va_start(ap, format);//初始化变参指针

			while (*format) {
				switch (*format) {
				case '-':
					format++;
					switch (*format) {
					case 'b'://baudrate
						this->baudrate = va_arg(ap, int);
						break;
					case 'd'://databits
						this->databits = va_arg(ap, int);
						break;
					case 'p'://parity
						this->parity = va_arg(ap, int);
						break;
					case 's'://stopbits
						this->stopbits = va_arg(ap, int);
						break;
					default:format--;
					}
				default:;
				}
				format++;

			}

			va_end(ap);//清理
			this->path = filepath;
			this->openfd(this->path);
		}

		//int tty::init(map<std::string, std::string> paramMap) {
		//
		//}

		int tty::init(std::string filepath) {
			this->path = filepath;
			this->openfd(this->path);
		
		}

		int tty::init(std::string filepath, int speed){
			this->baudrate = speed;
			this->path = filepath;
			this->openfd(this->path);
		}








		int tty::setSpeed(int speed) {
			struct termios Opt;

			long vdisable;
			int speed_arr[] = { B115200, B57600, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
			int name_arr[] = { 115200, 57600, 38400, 19200, 9600, 4800, 2400, 1200, 300, };

			int i;
			int status;

			//获取串口的termios信息
			tcgetattr(fd, &Opt);

			int arr_size = sizeof(speed_arr) / sizeof(int);

			for (i = 0; i < arr_size; i++) {
				if (speed == name_arr[i]) {
					//刷新输入输出数据，但是不读也不传送
					tcflush(fd, TCIOFLUSH);
					//设置输入波特率
					cfsetispeed(&Opt, speed_arr[i]);
					//设置输出波特率
					cfsetospeed(&Opt, speed_arr[i]);
					//用于设置终端的相关参数
					//TCSANOW：不等数据传输完毕就立即改变属性
					status = tcsetattr(fd, TCSANOW, &Opt);

					if (status != 0) {
						perror("tcsetattr fd");
						exit(EXIT_FAILURE);
					}
					tcflush(fd, TCIOFLUSH);
				}
			}


			tcflush(fd, TCIOFLUSH);

			/* 屏蔽控制字符 */
			if ((vdisable = fpathconf(STDIN_FILENO, _PC_VDISABLE)) < 0)
				printf("fpathconf error or _POSIX_VDISABLE not in effect\n");

			Opt.c_cc[VTIME] = 500; 	/* 设置超时50 seconds*/
			Opt.c_cc[VMIN] = 0; 	/* 在计时器超时前，read返回[1,nbytes];若计时器超时，read返回0 */

			Opt.c_cc[VINTR] = vdisable;   /**//* Ctrl-c */
			Opt.c_cc[VQUIT] = vdisable;   /**//* Ctrl- */
			Opt.c_cc[VERASE] = vdisable;   /**//* del */
			Opt.c_cc[VKILL] = vdisable;   /**//* @ */
			Opt.c_cc[VEOF] = vdisable;   /**//* Ctrl-d */
			Opt.c_cc[VSWTC] = vdisable;   /**//* '' */
			Opt.c_cc[VSTART] = vdisable;   /**//* Ctrl-q */
			Opt.c_cc[VSTOP] = vdisable;   /**//* Ctrl-s */
			Opt.c_cc[VSUSP] = vdisable;   /**//* Ctrl-z */
			Opt.c_cc[VEOL] = vdisable;   /**//* '' */
			Opt.c_cc[VREPRINT] = vdisable;   /**//* Ctrl-r */
			Opt.c_cc[VDISCARD] = vdisable;   /**//* Ctrl-u */
			Opt.c_cc[VWERASE] = vdisable;   /**//* Ctrl-w */
			Opt.c_cc[VLNEXT] = vdisable;   /**//* Ctrl-v */
			Opt.c_cc[VEOL2] = vdisable;   /**//* '' */

			Opt.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
			//Opt.c_lflag |= (ICANON | ECHO | ECHOE);


			Opt.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);

			Opt.c_oflag &= ~(OPOST);

			if (tcsetattr(fd, TCSANOW, &Opt) != 0) {
				perror("SetupSerial 3");
				return (-1);
			}

			return (0);
		}



		int tty::setParity(int parity) {
			struct termios Opt;

			if (tcgetattr(fd, &Opt) != 0) {
				perror("setup serial 1");
				return -1;
			}
			switch (parity) {
			case 'n':
			case 'N':
				Opt.c_cflag &= ~PARENB;   /* Clear parity enable */
				Opt.c_iflag &= ~INPCK;     /* Disable parity checking */
				break;

			case 'o':
			case 'O':
				Opt.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
				Opt.c_iflag |= INPCK;             /* Enable parity checking */
				break;

			case 'e':
			case 'E':
				Opt.c_cflag |= PARENB;     /* Enable parity */
				Opt.c_cflag &= ~PARODD;   /* 转换为偶效验*/
				Opt.c_iflag |= INPCK;       /* Enable parity checking */
				break;

			default:
				fprintf(stderr, "Unsupported parity\n");
				return (-1);
			}

			tcflush(fd, TCIOFLUSH);
		}



		int tty::setDatabits(int databits){

			struct termios Opt;

			long vdisable;

			if (tcgetattr(fd, &Opt) != 0) {
				perror("setup serial 1");
				return -1;
			}

			Opt.c_cflag &= ~CSIZE;

			switch (databits) {
			case 5:
				Opt.c_cflag |= CS5;
				break;

			case 6:
				Opt.c_cflag |= CS6;
				break;

			case 7:
				Opt.c_cflag |= CS7;
				break;

			case 8:
				Opt.c_cflag |= CS8;
				break;

			default:
				fprintf(stderr, "Unsupported data size\n");
				return (-1);
			}

			tcflush(fd, TCIOFLUSH);
		}


		int tty::setStopbits(int stopbits) {
			struct termios Opt;

			if (tcgetattr(fd, &Opt) != 0) {
				perror("setup serial 1");
				return -1;
			}

			switch (stopbits) {
			case 1:
				Opt.c_cflag &= ~CSTOPB;
				break;

			case 2:
				Opt.c_cflag |= CSTOPB;
				break;

			default:
				fprintf(stderr, "Unsupported stop bits\n");
				return (-1);
			}

			tcflush(fd, TCIOFLUSH);
		}


		int tty::openfd(std::string filepath){
			std::cout << "open file path = " << filepath.data() << std::endl;
			this->fd = open(filepath.data(), O_RDWR);

			if (this->fd < 0) {
				perror("open");
				exit(EXIT_FAILURE);
			}
			std::cout << "serial open success" << std::endl;

			setSpeed(this->baudrate);
			setDatabits(this->databits);
			setParity(this->parity);
			setStopbits(this->stopbits);
			return 0;
		}


		int tty::closefd() {
			close(this->fd);
		}


		int tty::getfd(){
			return this->fd;
		}



		int tty::getBaudrate() {
			return this->baudrate;
		}

		int tty::getDatabits() {
			return this->databits;
		}
		int tty::getParity() {
			return this->parity;
		}
		int tty::getStopbits() {
			return this->stopbits;
		}


	}


}
