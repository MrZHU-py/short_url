// url_generator.h

#ifndef URL_GENERATOR_H
#define URL_GENERATOR_H


// 包含hiredis库，用于Redis数据库的操作
#include <hiredis/hiredis.h>

/**
 * 创建URL。
 * @param conn Redis数据库连接上下文。
 * 此函数负责在Redis数据库中生成和存储新的URL。
 */
void create_url(redisContext *conn);

/**
 * 选择URL。
 * @param conn Redis数据库连接上下文。
 * 此函数用于从Redis数据库中选择一个URL。
 */
void select_url(redisContext *conn);

/**
 * 延长URL的有效时间。
 * @param conn Redis数据库连接上下文。
 * 此函数用于更新Redis数据库中URL的过期时间。
 */
void extend_time(redisContext *conn);

/**
 * 增加用户计数。
 * @param conn Redis数据库连接上下文。
 * 此函数用于增加用户在Redis数据库中的使用计数。
 */
void incr_count_user(redisContext *conn);

/**
 * 增加URL的访问计数。
 * @param conn Redis数据库连接上下文。
 * @param url 要增加访问计数的URL。
 * @param count 要增加的访问计数。
 * 此函数用于增加特定URL在Redis数据库中的访问计数。
 */
void incr_count(redisContext *conn, const char *url, int count);

/**
 * 显示所有URL。
 * @param conn Redis数据库连接上下文。
 * 此函数用于从Redis数据库中获取并显示所有的URL。
 */
void show_all(redisContext *conn);

/**
 * 显示所有URL的详细信息。
 * @param conn Redis数据库连接上下文。
 * 此函数用于从Redis数据库中获取并显示所有URL的详细信息。
 */
void display_all(redisContext *conn);

#endif // URL_GENERATOR_H