// url_generator.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "url_generator.h"
#include "redis_handler.h"
#include "utils.h"

/**
 * 创建短网址
 * 该函数通过用户输入的原地址和随机生成的短地址来创建一个新的网址映射，并在Redis中设置相应的信息
 * 包括默认的有效期、赠送的访问次数以及访问次数的限制
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 */
void create_url(redisContext *conn)
{
    char origin_url[1024];
    printf("请输入原地址：\n");
    scanf("%s", origin_url);
    char *short_url = get_random();
    printf("生成的短地址是：%s，默认有效期：7天，赠送：5000次访问\n", short_url);

    insert_url(conn, short_url, origin_url);
    incr_count(conn, short_url, 5000);
    set_url_expiration(conn, short_url, 604800);
    display_url_info(conn, short_url);
    free(short_url);
}

/**
 * 查询短网址的信息
 * 该函数根据用户输入的短地址，查询并打印出对应的原地址信息，同时处理短网址失效的情况
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 */
void select_url(redisContext *conn)
{
    char url[128];
    printf("请输入要查询的短地址：\n");
    scanf("%s", url);

    int count = get_url_count(conn, url);
    int times = get_url_times(conn, url);

    if (times >= count)
    {
        printf("该短地址已失效\n");
        return;
    }
    else
    {
        incr_url_times(conn, url);
        char *origin_url = get_origin_url(conn, url);
        printf("原网址为：%s\n", origin_url);
        free(origin_url);
    }
}

/**
 * 延长短网址的有效期
 * 该函数允许用户输入短地址和延期的时长，计算新的有效期，并在Redis中更新短网址的过期时间
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 */
void extend_time(redisContext *conn)
{
    char url[128];
    int days;

    printf("请输入要延期的短地址：\n");
    scanf("%s", url);
    printf("请输入要延期的时长：（单位：天）\n");
    scanf("%d", &days);

    long ttl = get_url_ttl(conn, url);
    ttl += days * 86400;
    set_url_expiration(conn, url, ttl);
    printf("短地址 %s 延期成功，剩余有效期：%ld天\n", url, ttl / 86400);
}

/**
 * 增加短网址的访问次数限制
 * 该函数允许用户为指定的短地址增加访问次数限制，并在Redis中更新访问次数的信息
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 */
void incr_count_user(redisContext *conn)
{
    char url[128];
    int count;

    printf("请输入要增加访问次数的短地址：\n");
    scanf("%s", url);
    printf("请输入要增加的访问次数：\n");
    scanf("%d", &count);

    incr_url_count(conn, url, count);
    printf("短地址 %s 的访问次数增加了 %d 次\n", url, count);
}

/**
 * 内部函数，用于增加短网址的访问次数
 * 该函数通过调用incr_url_count函数来实现对指定短地址访问次数的增加
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 * @param url 要增加访问次数的短地址
 * @param count 要增加的访问次数
 */
void incr_count(redisContext *conn, const char *url, int count)
{
    incr_url_count(conn, url, count);
}

/**
 * 显示短网址的详细信息
 * 该函数允许用户输入短地址，并展示该短地址的所有信息，包括访问次数、有效期等
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 */
void show_all(redisContext *conn)
{
    char url[128];
    printf("请输入要查询的短地址：\n");
    scanf("%s", url);
    display_url_info(conn, url);
}

/**
 * 显示所有短网址的信息
 * 该函数展示系统中所有短地址的详细信息，用于管理员查看和管理
 *
 * @param conn Redis连接上下文，用于与Redis数据库交互
 */
void display_all(redisContext *conn)
{
    display_all_urls(conn);
}