// redis_handler.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "redis_handler.h"
#include "utils.h"

/**
 * 连接到Redis服务器。
 * @param hostname Redis服务器的主机名。
 * @param port Redis服务器的端口。
 * @return 返回redisContext指针，用于后续的Redis操作；如果连接失败，返回NULL。
 */
redisContext* connect_redis(const char *hostname, int port)
{
    redisContext *conn = redisConnect(hostname, port);
    if (conn == NULL || conn->err)
    {
        if (conn)
        {
            printf("Redis connection error: %s\n", conn->errstr);
            redisFree(conn);
        }
        else
        {
            printf("Can't allocate redis context\n");
        }
        return NULL;
    }
    return conn;
}

/**
 * 在Redis中插入一个URL记录。
 * @param conn 到Redis服务器的连接。
 * @param short_url 短URL。
 * @param origin_url 原始URL。
 */
void insert_url(redisContext *conn, const char *short_url, const char *origin_url)
{
    redisReply *reply = redisCommand(conn, "HSET %s origin_url %s create_time %s", short_url, origin_url, get_time());
    freeReplyObject(reply);
}

/**
 * 获取URL的点击计数。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 * @return 返回URL的点击计数。
 */
int get_url_count(redisContext *conn, const char *url)
{
    redisReply *reply = redisCommand(conn, "HGET %s count", url);
    int count = atoi(reply->str);
    freeReplyObject(reply);
    return count;
}

/**
 * 获取URL的访问次数。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 * @return 返回URL的访问次数。
 */
int get_url_times(redisContext *conn, const char *url)
{
    redisReply *reply = redisCommand(conn, "HGET %s times", url);
    int times = atoi(reply->str);
    freeReplyObject(reply);
    return times;
}

/**
 * 增加URL的访问次数。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 */
void incr_url_times(redisContext *conn, const char *url)
{
    redisReply *reply = redisCommand(conn, "HINCRBY %s times 1", url);
    freeReplyObject(reply);
}

/**
 * 获取URL的原始地址。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 * @return 返回URL的原始地址。
 */
char* get_origin_url(redisContext *conn, const char *url)
{
    redisReply *reply = redisCommand(conn, "HGET %s origin_url", url);
    char *origin_url = strdup(reply->str);
    freeReplyObject(reply);
    return origin_url;
}

/**
 * 增加URL的点击计数。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 * @param count 要增加的点击计数。
 */
void incr_url_count(redisContext *conn, const char *url, int count)
{
    redisReply *reply = redisCommand(conn, "HINCRBY %s count %d", url, count);
    freeReplyObject(reply);
}

/**
 * 为URL设置过期时间。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 * @param ttl 过期时间（秒）。
 */
void set_url_expiration(redisContext *conn, const char *url, long ttl)
{
    redisReply *reply = redisCommand(conn, "EXPIRE %s %ld", url, ttl);
    freeReplyObject(reply);
}

/**
 * 获取URL的剩余生存时间。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 * @return 返回URL的剩余生存时间（秒）；如果URL不存在或没有设置过期时间，则返回-1。
 */
long get_url_ttl(redisContext *conn, const char *url)
{
    redisReply *reply = redisCommand(conn, "TTL %s", url);
    long ttl = reply->integer;
    freeReplyObject(reply);
    return ttl;
}

/**
 * 显示URL的所有信息。
 * @param conn 到Redis服务器的连接。
 * @param url URL地址。
 */
void display_url_info(redisContext *conn, const char *url)
{
    redisReply *reply = redisCommand(conn, "HGETALL %s", url);
    if (reply->type == REDIS_REPLY_ARRAY) 
    {
        for (int i = 0; i < reply->elements; i += 2) 
        {
            printf("%s : %s\n", reply->element[i]->str, reply->element[i + 1]->str);
        }
    } 
    else 
    {
        printf("No data found for key '%s'.\n", url);
    }
    freeReplyObject(reply);
}

/**
 * 显示所有URL的键。
 * @param conn 到Redis服务器的连接。
 */
void display_all_urls(redisContext *conn)
{
    redisReply *reply = redisCommand(conn, "KEYS ZPY.cn/*");
    if (reply->type == REDIS_REPLY_ARRAY) 
    {
        for (int i = 0; i < reply->elements; i ++) 
        {
            printf("%s\n", reply->element[i]->str);
        }
    }
    freeReplyObject(reply);
}