#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>
#include "utils.h"
#include "redis_handler.h"
#include "url_generator.h"

void handle_error(const char *msg) {
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

void resolve_short_url(const char* short_code) {
    redisContext *c = redisConnect("127.0.0.1", 6379);
    if (c == NULL || c->err) {
        printf("Content-Type: text/html\r\n\r\n");
        printf("<html><body>Failed to connect to Redis!</body></html>");
        return;
    }

    redisReply *reply = redisCommand(c, "GET %s", short_code);
    if (reply == NULL) {
        printf("Content-Type: text/html\r\n\r\n");
        printf("<html><body>Redis command failed!</body></html>");
        redisFree(c);
        return;
    }

    if (reply->type == REDIS_REPLY_STRING) {
        printf("Status: 302 Moved Temporarily\r\n");
        printf("Location: %s\r\n\r\n", reply->str);
    } else {
        printf("Content-Type: text/html\r\n\r\n");
        printf("<html><body>URL not found!</body></html>");
    }

    freeReplyObject(reply);
    redisFree(c);
}

void process_request(const char *action, const char *data) {
    redisContext *conn = connect_redis("127.0.0.1", 6379);
    if (conn == NULL) {
        handle_error("Redis connection failed.");
    }

    char response[1024];
    memset(response, 0, sizeof(response)); 

    if (strcmp(action, "create_url") == 0) {
        char *origin_url = strstr(data, "origin_url=");
        if (origin_url) {
            origin_url += strlen("origin_url=");
            char *end = strchr(origin_url, '&');
            if (end) *end = '\0';

            char *short_url = get_random();
            insert_url(conn, short_url, origin_url);
            incr_count(conn, short_url, 5000);
            set_url_expiration(conn, short_url, 604800);
            snprintf(response, sizeof(response), "生成的短地址是: %s", short_url);
            free(short_url);
        } else {
            snprintf(response, sizeof(response), "原地址缺失");
        }
    } else if (strcmp(action, "select_url") == 0) {
        char *short_url = strstr(data, "short_url=");
        if (short_url) {
            short_url += strlen("short_url=");
            char *end = strchr(short_url, '&');
            if (end) *end = '\0';
            char *origin_url = get_origin_url(conn, short_url);
            if (origin_url) {
                snprintf(response, sizeof(response), "原地址是: %s", origin_url);
                free(origin_url);
            } else {
                snprintf(response, sizeof(response), "未找到该短地址");
            }
        } else {
            snprintf(response, sizeof(response), "短地址缺失");
        }
    } else if (strcmp(action, "extend_time") == 0) {
        char *short_url = strstr(data, "short_url=");
        if (short_url) {
            short_url += strlen("short_url=");
            char *end = strchr(short_url, '&');
            if (end) *end = '\0';
            set_url_expiration(conn, short_url, 604800);
            snprintf(response, sizeof(response), "短地址的有效期已延长");
        } else {
            snprintf(response, sizeof(response), "短地址缺失");
        }
    } else if (strcmp(action, "incr_count_user") == 0) {
        char *short_url = strstr(data, "short_url=");
        if (short_url) {
            short_url += strlen("short_url=");
            char *end = strchr(short_url, '&');
            if (end) *end = '\0';
            incr_count(conn, short_url, 1);
            snprintf(response, sizeof(response), "短地址的点数已增加");
        } else {
            snprintf(response, sizeof(response), "短地址缺失");
        }
    } else if (strcmp(action, "show_all") == 0) {
        redisReply *reply = redisCommand(conn, "KEYS *");
        if (reply == NULL) {
            snprintf(response, sizeof(response), "获取数据失败");
        } else {
            snprintf(response, sizeof(response), "所有数据: ");
            for (size_t i = 0; i < reply->elements; i++) {
                redisReply *item = reply->element[i];
                strcat(response, item->str);
                if (i < reply->elements - 1) strcat(response, ", ");
            }
            freeReplyObject(reply);
        }
    } else if (strcmp(action, "display_all") == 0) {
        snprintf(response, sizeof(response), "统计信息显示");
    } else {
        snprintf(response, sizeof(response), "未知的操作");
    }

    printf("Content-Type: text/html\r\n\r\n");
    printf("<html><body>%s</body></html>", response);

    redisFree(conn);
}

int main() {
    char *method = getenv("REQUEST_METHOD");
    char *data = NULL;

    if (method && strcmp(method, "POST") == 0) {
        char *content_length = getenv("CONTENT_LENGTH");
        if (content_length) {
            long len = strtol(content_length, NULL, 10);
            data = malloc(len + 1);
            if (data) {
                fread(data, 1, len, stdin);
                data[len] = '\0';
            }
        }
    } else if (method && strcmp(method, "GET") == 0) {
        data = getenv("QUERY_STRING");
    }

    if (data) {
        char *action = strstr(data, "action=");
        if (action) {
            action += strlen("action=");
            char *end = strchr(action, '&');
            if (end) *end = '\0';

            process_request(action, data);
        } else {
            printf("Content-Type: text/html\r\n\r\n");
            printf("<html><body>操作类型缺失</body></html>");
        }

        free(data);
    } else {
        printf("Content-Type: text/html\r\n\r\n");
        printf("<html><body>无效的请求</body></html>");
    }

    return 0;
}
