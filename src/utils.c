// utils.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

/**
 * 生成随机文件名
 *
 * 该函数用于生成一个独特的随机文件名，以特定的前缀开始，并包含一个时间戳部分，
 * 以确保文件名的唯一性。
 *
 * 返回:
 *     返回一个指向生成的随机文件名字符串的指针。
 */
char *get_random()
{
    int length = 13;
    char *buff = malloc(sizeof(char) * length + 1);
    buff[0] = 'Z';
    buff[1] = 'P';
    buff[2] = 'Y';
    buff[3] = '.';
    buff[4] = 'c';
    buff[5] = 'n';
    buff[6] = '/';
    char metachar[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    srand((unsigned)time(NULL));
    for (int i = 7; i < length; i++)
    {
        buff[i] = metachar[rand() % 62];
    }

    buff[length] = '\0';
    return buff;
}

/**
 * 获取用户选择
 *
 * 该函数提示用户输入一个序号，以选择要执行的功能。
 *
 * 返回:
 *     用户输入的序号。
 */
int get_choice()
{
    int choice;
    printf("请输入序号使用功能：\n");
    scanf("%d", &choice);
    return choice;
}

/**
 * 获取当前时间字符串
 *
 * 该函数返回当前时间的字符串表示，格式为"年-月-日 时:分:秒"。
 *
 * 返回:
 *     返回一个指向当前时间字符串的指针。
 */
char *get_time()
{
    time_t t;
    struct tm *tmp;
    char buf[64];

    time(&t);
    tmp = localtime(&t);
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", tmp);

    return strdup(buf);
}
