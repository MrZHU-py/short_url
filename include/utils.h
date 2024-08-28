// utils.h

#ifndef UTILS_H
#define UTILS_H

/**
 * 获取一个随机字符串。
 * 
 * @return 返回一个随机生成的字符串指针。
 */
char *get_random();

/**
 * 获取用户的选择。
 * 
 * @return 返回用户的选择，作为一个整数。
 */
int get_choice();

/**
 * 获取当前的时间字符串。
 * 
 * @return 返回一个表示当前时间的字符串指针。
 */
char *get_time();

#endif // UTILS_H