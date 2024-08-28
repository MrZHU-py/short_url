// redis_handler.h

// 防止头文件被重复引用
#ifndef REDIS_HANDLER_H
#define REDIS_HANDLER_H

// 包含hiredis库的头文件
#include <hiredis/hiredis.h>

/**
 * 连接到Redis服务器
 * @param hostname Redis服务器的主机名
 * @param port Redis服务器的端口号
 * @return 返回连接上下文，用于后续的Redis操作
 */
redisContext* connect_redis(const char *hostname, int port);

/**
 * 向Redis中插入一个URL映射
 * @param conn Redis连接上下文
 * @param short_url 短URL
 * @param origin_url 原始URL
 */
void insert_url(redisContext *conn, const char *short_url, const char *origin_url);

/**
 * 获取URL被访问的次数
 * @param conn Redis连接上下文
 * @param url 需要查询的URL
 * @return 返回URL的访问次数
 */
int get_url_count(redisContext *conn, const char *url);

/**
 * 获取URL被访问的总次数
 * @param conn Redis连接上下文
 * @param url 需要查询的URL
 * @return 返回URL的总访问次数
 */
int get_url_times(redisContext *conn, const char *url);

/**
 * 增加URL的访问次数
 * @param conn Redis连接上下文
 * @param url 需要增加访问次数的URL
 */
void incr_url_times(redisContext *conn, const char *url);

/**
 * 通过短URL获取原始URL
 * @param conn Redis连接上下文
 * @param url 需要查询的短URL
 * @return 返回对应的原始URL
 */
char* get_origin_url(redisContext *conn, const char *url);

/**
 * 增加URL的访问次数
 * @param conn Redis连接上下文
 * @param url 需要增加访问次数的URL
 * @param count 增加的访问次数
 */
void incr_url_count(redisContext *conn, const char *url, int count);

/**
 * 为URL设置过期时间
 * @param conn Redis连接上下文
 * @param url 需要设置过期时间的URL
 * @param ttl 过期时间（秒）
 */
void set_url_expiration(redisContext *conn, const char *url, long ttl);

/**
 * 获取URL的剩余过期时间
 * @param conn Redis连接上下文
 * @param url 需要查询的URL
 * @return 返回URL的剩余过期时间（秒）
 */
long get_url_ttl(redisContext *conn, const char *url);

/**
 * 显示指定URL的所有信息
 * @param conn Redis连接上下文
 * @param url 需要显示信息的URL
 */
void display_url_info(redisContext *conn, const char *url);

/**
 * 显示所有URL的信息
 * @param conn Redis连接上下文
 */
void display_all_urls(redisContext *conn);

// 结束防止头文件被重复引用的宏定义
#endif // REDIS_HANDLER_H