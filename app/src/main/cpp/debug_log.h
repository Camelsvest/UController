//
// Created by Administrator on 2018/3/12.
//
#ifndef UCONTROLLER_DEBUG_LOG_H_H
#define UCONTROLLER_DEBUG_LOG_H_H

#ifdef __cplusplus 
extern "C" {
#endif

//It should defined in MAKEFILE
//#define
//#define  LINUX_DEBUG

#if defined(ANDROID_DEBUG)
    #include <android/log.h>
#endif

#if defined(ANDROID_DEBUG)
	    #define LOG_DEBUG(msg,args...) \
	    __android_log_print(ANDROID_LOG_DEBUG,"UCONTROLLER-NATIVE", msg, ##args)

        #define LOG_WARN(msg, args...) \
        __android_log_print(ANDROID_LOG_WARN, "UCONTROLLER-NATIVE", msg, ##args)

        #define LOG_INFO(msg, args...) \
	    __android_log_print(ANDROID_LOG_INFO, "UCONTROLLER-NATIVE", msg, ##args)

        #define LOG_ERROR(msg, args...) \
        __android_log_print(ANDROID_LOG_ERROR, "UCONTROLLER-NATIVE", msg, ##args)

        #define LOG_DEBUG_DUMP(buf, length)

#elif defined(LINUX_DEBUG)

        extern unsigned int verboslevel;

        #define LOG_DEBUG_DUMP(buf, length) \
                if (verboslevel > 10) { \
                    do { \
                        unsigned int index; \
                        for (index = 0; index < length; index++) { \
                           fprintf(stdout, "0x%02X ", *(buf+index)); \
                           if ((index + 1) % 16 == 0) fprintf(stdout, "\r\n"); \
                        } \
                    } while (0); \
                    fprintf(stdout, "\r\n"); \
                }
	
	#define LOG_DEBUG(msg,args...) \
                { \
                    if (verboslevel > 10) fprintf(stdout, msg, ##args); \
                } 

        #define LOG_INFO(msg, args...) \
                { \
                    if (verboslevel > 20) fprintf(stdout, msg, ##args); \
        		} 
	
        #define LOG_ERROR(msg, args...) \
                { \
                    if (verboslevel > 30) fprintf(stderr, msg, ##args); \
                }
		#define LOG_WARN(msg,args...) \
				{ \
					if (verboslevel > 10) fprintf(stdout, msg, ##args); \
				}
#else
	#define LOG_DEBUG(msg, args...)
	#define LOG_INFO(msg, args...)
	#define LOG_ERROR(msg, args...)
	#define LOG_WARN(msg,args...)
#endif



#define ENTER_FUNCTION	LOG_DEBUG("Enter function %s\r\n", __FUNCTION__)
#define EXIT_FUNCTION	LOG_DEBUG("Exit function %s\r\n", __FUNCTION__)

#ifdef __cplusplus
} // end of extern "C"
#endif

#endif //UCONTROLLER_DEBUG_LOG_H_H
